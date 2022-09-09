// Fill out your copyright notice in the Description page of Project Settings.


#include "RacerAIController.h"
#include "RacerGameModeBase.h"
#include "CheckPoint.h"
#include "ItemBox.h"
#include "Track_Spline.h"
#include "JumpPad.h"
#include "Kismet/GameplayStatics.h"
#include <Racer\Car.h>

ARacerAIController::ARacerAIController()
{
	GameMode = Cast<ARacerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	OverallBestTurn;
	OverallBestGear;
	OverallBestSpline;

	//CompletedLap;

}

void ARacerAIController::EndTurn()
{

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "EndTurn() for AIController has been reached");
	//How to get AI controlled pawn
	ACar* Car = Cast<ACar>(GetPawn());
	GameMode->EndTurn(Car);

}


void ARacerAIController::DecideMove() {

	ACar* Car = Cast<ACar>(GetPawn());

	

	if ((Car != NULL && !(Car->Crashed))) {

		

		//Car->ToggleFutureVisibility();

		//CompletedLap = false;
		OverallBestGear = 0;
		OverallBestTurn = 0;
		OverallBestSpline = 0.0;

		if (Car->GetInitialGear() == 0) {
			OverallBestGear = 1;
			Car->SetGear(1);
		}
		else if (Car->GetInitialGear() > 1 && Car->Drifting && Car->DriftPadReference != NULL && Car->DriftPadReference != nullptr){
			if (Car->HasNitro) {
				Car->UsePowerUp();
			}
			Car->SetGear(Car->GetInitialGear()+1);
		}
		else {
			CalculateMove(Car);
		}


		if (Car->FutureMesh->IsVisible()) {
			Car->ToggleFutureVisibility();
		}	
	}

	FVector RotationCheck = Car->GetCarForwardVector();
	RotationCheck.Normalize();
	float Dist = Car->MySpline->Spline->FindInputKeyClosestToWorldLocation(Car->GetActorLocation());
	if (Car->CalculateSplineOrientation(Dist, RotationCheck) < 0) {
		Car->TurnsInWrongDirection += 1;
	}
	else {
		Car->TurnsInWrongDirection = 0;
	}

	EndTurn();

}


void ARacerAIController::CalculateMove(ACar* Car) {
	float CurrentRelativeSpline;
	int TempLap = Car->CurrentLap;
	int BestTurn = 0;
	int BestGear = 0;
	int InitialGear = Car->GetInitialGear();
	FRotator InitialRotation = Car->GetActorRotation();

	//Reset orientation if driving in the wrong direction
	if (Car->TurnsInWrongDirection >= 1) {
		InitialRotation = Car->MySpline->Spline->FindRotationClosestToWorldLocation(Car->GetActorLocation(), ESplineCoordinateSpace::World);
		InitialRotation.Yaw -= 90.0;
		Car->SetActorRotation(InitialRotation);
		Car->TurnsInWrongDirection = 0;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Orientation Reset");
	}

	if (Car->HasPowerUp) {
		if (Car->HasNitro) {
			if (InitialGear <= 2) {
				Car->UsePowerUp();
			}
		}
		else {
			Car->UsePowerUp();
		}
	}

	
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Initial Rotation: " + InitialRotation.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Initial Gear" + FString::FromInt(InitialGear));

	//In case of no move being good
	FRotator BestRotation = InitialRotation;
	OverallBestGear = InitialGear;

	for (int i = -6; i < 7; i++) {
		for (int j = -1; j < 3; j++) {
			if (InitialGear + j > 6 || InitialGear + j <= 0) continue;
			else if (!(Car->DraftAvailable) && j == 2) continue;

			FRotator PossibleRotation = InitialRotation;
			PossibleRotation.Yaw += ((Car->GearToTurnMapping[InitialGear] * i)/2.0f);
			Car->SetActorRotation(PossibleRotation);

			FVector PossibleVector = Car->GetActorLocation();
			FVector Direction = Car->GetActorRightVector();
			PossibleVector += Direction * Car->GearToDistanceMapping[InitialGear + j];
			PossibleVector.Z = Car->HitBox->GetComponentLocation().Z;


			float SplineDist = Car->CalculateSplineDistance(PossibleVector);

			//float SplineDist = Cast<ATrack_Spline>(Car->OutActors[0])->Spline->FindInputKeyClosestToWorldLocation(PossibleVector);
			if ((SplineDist < 1.5f && Car->CurrentSplineLocation > 28.0f) && Car->Checkpoint1 && Car->Checkpoint2) {
				SplineDist += 50.0f;
			}
			else if (SplineDist > 20.0 && (!(Car->Checkpoint1) || !(Car->Checkpoint2))) {
				SplineDist -= 100;
			}



			TArray<FHitResult> HitResults;
			
			
			FComponentQueryParams Params;
			Params.TraceTag = FName("CheckCollisionTrace");
			Params.bTraceComplex = true;
			Params.AddIgnoredActor(Car);
			Params.AddIgnoredActor(Cast<ATrack_Spline>(Car->OutActors[0]));
			Params.AddIgnoredActors(GameMode->MyGameState->AllPowerUps);
			Params.AddIgnoredActors(GameMode->MyGameState->AllDriftPoints);
			//Params.AddIgnoredActors(GameMode->MyGameState->AllItemBoxes);
			for (AActor* Current : GameMode->MyGameState->AllCheckPoints) {
				ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
				Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
			}
			//Params.AddIgnoredActors(GameMode->MyGameState->AllJumpPads);


			if (!Car->UsedFireTires) {
				if (GameMode->MyGameState->AllOilSlicks.Num() > 0) {
					for (AOilSlick* Current : GameMode->MyGameState->AllOilSlicks) {
						AActor* ActorSlick = Cast<AActor>(Current);
						Params.AddIgnoredActor(ActorSlick);
					}
				}
				if (GameMode->MyGameState->AllFireTrails.Num() > 0) {
					for (AFireTrail* Current : GameMode->MyGameState->AllFireTrails) {
						AActor* ActorTrail = Cast<AActor>(Current);
						Params.AddIgnoredActor(ActorTrail);
					}
				}
			}
			
			
			CurrentRelativeSpline = SplineDist;
			
			
			
			(GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ HitResults, /*class UPrimitiveComponent* PrimComp*/ Car->HitBox, /*const FVector & Start*/ Car->HitBox->GetComponentLocation(),
				/*const FVector & End*/ PossibleVector, /*const FQuat & Rot*/ PossibleRotation, /*const FComponentQueryParams & Params*/ Params));
			if (HitResults.Num() > 0) {
				if (!(HitResults[0].Location.Equals(PossibleVector))) {
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, UKismetSystemLibrary::GetDisplayName(HitResults[0].Component.Get()));
					ACar* OtherCar = Cast<ACar>(HitResults[0].Actor);
					if (HitResults[0].Actor->IsA<AJumpPad>()) {
						SplineDist += 100.0f;
					}
					else if (HitResults[0].Actor->IsA<AItemBox>() && HitResults.Num() == 1) {
						SplineDist += 5.0;
					}
					//Hopefully this will make larger cars more aggressive
					else if (OtherCar != NULL && OtherCar != nullptr && Car->CowCatcher->IsVisible()) {
						SplineDist += 2.0f;
					}
					else {
						SplineDist -= 100.0f;
					}
				}
			}


			SplineDist += CalculateNextMove(Car, 1, PossibleVector, PossibleRotation, InitialGear+j, CurrentRelativeSpline);

			
			

			if (SplineDist > OverallBestSpline) {
				OverallBestSpline = SplineDist;
				OverallBestGear = InitialGear + j;
				OverallBestTurn = i;
				BestRotation = PossibleRotation;

			}
		}
	}

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Best Rotation: " + BestRotation.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Best Spline: " + FString::SanitizeFloat(OverallBestSpline));
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Blue, "Best Gear" + FString::FromInt(OverallBestGear));
	Car->SetActorRotation(BestRotation);
	Car->SetGear(OverallBestGear);
}


float ARacerAIController::CalculateNextMove(ACar* Car, int MoveCount, FVector Position, FRotator Rotation, int CurrentGear, float RelativeSpline) {

	if (MoveCount == 2) {
		return 0.0f;
	}

	float NewRelativeSpline;
	float BestSplineDist = 0.0;

	for (int i = -6; i < 7; i++) {
		for (int j = -1; j < 2; j++) {
			if (CurrentGear + j > 5 || CurrentGear + j <= 0) continue;


			FRotator PossibleRotation = Rotation;
			PossibleRotation.Yaw += ((Car->GearToTurnMapping[CurrentGear] * i)/2.0f);
			Car->SetActorRotation(PossibleRotation);

			FVector PossibleVector = Position;
			FVector Direction = Car->GetActorRightVector() + PossibleRotation.Vector();
			PossibleVector += Direction * Car->GearToDistanceMapping[CurrentGear + j];
			PossibleVector.Z = Car->HitBox->GetComponentLocation().Z;

			

			float SplineDist = Car->CalculateSplineDistance(PossibleVector);



			if ((SplineDist < 1.5f && Car->CurrentSplineLocation > 28.0f) && Car->Checkpoint1 && Car->Checkpoint2) {
				SplineDist += 50.0f;
			}
			else if (SplineDist > 20.0 && (!(Car->Checkpoint1) || !(Car->Checkpoint2))) {
				SplineDist -= 100;
			}

			TArray<FHitResult> HitResults;


			FComponentQueryParams Params;
			Params.TraceTag = FName("CheckCollisonTrace");
			Params.bTraceComplex = true;
			Params.AddIgnoredActor(Car);
			Params.AddIgnoredActor(Cast<ATrack_Spline>(Car->OutActors[0]));
			Params.AddIgnoredActors(GameMode->MyGameState->AllPowerUps);
			Params.AddIgnoredActors(GameMode->MyGameState->AllDriftPoints);
			Params.AddIgnoredActors(GameMode->MyGameState->AllItemBoxes);
			Params.AddIgnoredActors(GameMode->MyGameState->AllJumpPads);
			for (AActor* Current : GameMode->MyGameState->AllCheckPoints) {
				ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
				Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
			}
			if (GameMode->MyGameState->AllOilSlicks.Num() > 0) {
				for (AOilSlick* Current : GameMode->MyGameState->AllOilSlicks) {
					AActor* ActorSlick = Cast<AActor>(Current);
					Params.AddIgnoredActor(ActorSlick);
				}
			}

			NewRelativeSpline = SplineDist;

			GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ HitResults, /*class UPrimitiveComponent* PrimComp*/ Car->HitBox, /*const FVector & Start*/ Position,
				/*const FVector & End*/ PossibleVector, /*const FQuat & Rot*/ PossibleRotation, /*const FComponentQueryParams & Params*/ Params);

			if (HitResults.Num() > 0) {
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Looking Further Ahead: " + UKismetSystemLibrary::GetDisplayName(HitResults[0].Component.Get()));

				
				SplineDist -= 25.0f;
			}

			

			SplineDist += CalculateNextMove(Car, MoveCount+1, PossibleVector, PossibleRotation, CurrentGear + j, NewRelativeSpline);


			if (SplineDist > BestSplineDist) {
				BestSplineDist = SplineDist;

			}
		}

	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Recursive ");
	return BestSplineDist;
}

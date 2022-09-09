// Fill out your copyright notice in the Description page of Project Settings.

#include "RacerGameModeBase.h"
#include "RacerGameStateBase.h"
#include "RacerAIController.h"
#include "ItemBox.h"
#include "OilSlick.h"
#include "CheckPoint.h"
#include "RacerPlayerController.h"
#include "TrackCamera.h"
#include "Components/AudioComponent.h"
#include "TimerManager.h"
#include "Track_Spline.h"
#include "Kismet/GameplayStatics.h"
#include <Racer\Car.h>

ARacerGameModeBase::ARacerGameModeBase() 
{
	CurrentRound = 0;
	CurrentTurn = 0;
	HeadToHead;

    MovementTimerHandle;

	AIActive;

	ActiveCar;

	SortedCars;

	CurrentActiveCamera;
	
	MyGameState;

	//Console commands
	DrawTireDebugLineCommand = false;
	DrawCollisionDebugLineCommand = false;
}


void ARacerGameModeBase::BeginPlay() {

	MyGameState = GetWorld() != NULL ? GetWorld()->GetGameState<ARacerGameStateBase>() : NULL;

	for (AActor* Temp : MyGameState->OutActors) {
		SortedCars.Add(Cast<ACar>(Temp));
	}

	Algo::SortBy(SortedCars, &ACar::TotalDistance);
	Algo::Reverse(SortedCars);
	
	for (int i = 0; i < SortedCars.Num(); i++) {
		SortedCars[i]->CarRank = i;
	}

	CurrentCarIndex = 0;

	CurrentActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[0]);
	CurrentActiveCamera->CarToFollow = MyGameState->OutActors[0];
	CurrentActiveCamera->ActiveCamera = true;

	//CalculateSplineDist();

}

int ARacerGameModeBase::GetMovementState() {

	return MyGameState->MovementState;
}

void ARacerGameModeBase::SetMovementState(int NewState) {

	MyGameState->MovementState = NewState;
}



void ARacerGameModeBase::TurnCycle() {

	CurrentRound++;

	for (ACar* Current : SortedCars) {
		Current->CheckRoundsSincePowerUp();
	}
	RespawnItemBoxes();
	DespawnFireTrails();


	if (MyGameState->AIControllerActive) {
		AIActiveTurnCycle();
	}
	else 
	{
		AINotActiveTurnCycle();
	}
}


void ARacerGameModeBase::AIActiveTurnCycle() {
		
	ARacerAIController* CarAIController = Cast<ARacerAIController>(ActiveCar->GetController());
	if (CarAIController == NULL || CarAIController == nullptr) {
		//Player controlled car turn
		AIActive = false;
		if (!(ActiveCar->FutureMesh->IsVisible())) {
			ActiveCar->ToggleFutureVisibility();
		}
	}
	else {
		CarAIController->DecideMove();
		AIActive = true;
	}

	
}

void ARacerGameModeBase::AINotActiveTurnCycle() {

	ARacerPlayerController* RacerPlayer = Cast<ARacerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));


	if (ActiveCar->GetController() != RacerPlayer) {
		RacerPlayer->Possess(ActiveCar);
		
	}
	if (!(ActiveCar->FutureMesh->IsVisible())) {
		ActiveCar->ToggleFutureVisibility();
	}
	
	
}


bool ARacerGameModeBase::IsEndOfRound() {

	return (CurrentTurn % SortedCars.Num()) == 0;
}

bool ARacerGameModeBase::IsActiveCarNull() {

	return (ActiveCar == NULL || ActiveCar == nullptr);
}



void ARacerGameModeBase::CalculateSplineDist() {

	for (ACar* Temp: SortedCars) {
		Temp->TotalDistance = (Temp->CalculateSplineDistance(Temp->HitBox->GetComponentLocation()));
		Temp->HadTurn = false;
		Temp->TotalDistance += Temp->CurrentLap*30.0;
	}

	Algo::SortBy(SortedCars, &ACar::TotalDistance);
	Algo::Reverse(SortedCars);

	for (int i = 0; i < SortedCars.Num(); i++) {
		SortedCars[i]->CarRank = i;
		if (SortedCars[i]->UsedFireTires) {
			SortedCars[i]->UsedFireTires = false;
			if (SortedCars[i]->TireFire1->IsVisible()) {
				SortedCars[i]->TireFire1->ToggleVisibility();
				SortedCars[i]->TireFire2->ToggleVisibility();
				SortedCars[i]->TireFire3->ToggleVisibility();
				SortedCars[i]->TireFire4->ToggleVisibility();
			}
			
		}
	}

}


void ARacerGameModeBase::SelectTrackCamera() {

	CurrentActiveCamera->ActiveCamera = false;
	AActor* Temp = MyGameState->OutActors[0];

	float CarDistance = 0.0;

	for (AActor* Current : MyGameState->OutActors) {
		ACar* CurrentCar = Cast<ACar>(Current);
		if (CurrentCar == ActiveCar) {
			CarDistance = CurrentCar->CalculateSplineDistance(CurrentCar->HitBox->GetComponentLocation());
			Temp = Current;
		}
	}



	ATrackCamera* ActiveCamera;

	if (CarDistance >= 1.75 && CarDistance < 3.0) {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[0]);
	}
	else if (CarDistance >= 3.0 && CarDistance < 6.0) {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[1]);
	}
	else if (CarDistance >= 6.0 && CarDistance < 13.7) {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[2]);
	}
	else if (CarDistance >= 13.7 && CarDistance < 16.9){
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[4]);
	}
	else if (CarDistance >= 16.9 && CarDistance < 20.0) {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[5]);
	}
	else if (CarDistance >= 20.0 && CarDistance < 28.6) {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[6]);
	}
	else {
		ActiveCamera = Cast<ATrackCamera>(MyGameState->AllTrackCameras[3]);
	}
	
	ActiveCamera->CarToFollow = Temp;
	ActiveCamera->ActiveCamera = true;
	CurrentActiveCamera = ActiveCamera;

}

void ARacerGameModeBase::RespawnItemBoxes()
{
	for (AActor* Current : MyGameState->AllItemBoxes) {
		AItemBox* CurrentBox = Cast<AItemBox>(Current);
		if (!(CurrentBox->PickUp->IsVisible())) {
			if (CurrentBox->RoundOfUse + 3 <= CurrentRound) {
				CurrentBox->PickUp->ToggleVisibility();
				CurrentBox->SetActorEnableCollision(true);
				CurrentBox->Inactive = false;
				CurrentBox->MessagePlayed = false;
			}
		}
	}
}


//This may potentially cause errors, needs to be tested
void ARacerGameModeBase::DespawnFireTrails() {

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Despawn Fire is reached");

	for (int i = 0; i < MyGameState->AllFireTrails.Num(); i++) {
		/*GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Round Created: " + FString::FromInt(MyGameState->AllFireTrails[i]->RoundCreated));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Current Round: " + FString::FromInt(CurrentRound));
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Black, FString::Printf(TEXT("Bool: %s"), (CurrentRound >= (MyGameState->AllFireTrails[i]->RoundCreated + 3)) ? TEXT("true") : TEXT("false")));*/
		if (CurrentRound >= (MyGameState->AllFireTrails[i]->RoundCreated + 3)) {
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Despawn Fire first loop");
			int Index = MyGameState->AllFireTrails.Find(MyGameState->AllFireTrails[i]);
			MyGameState->AllFireTrails[i]->Destroy();
			MyGameState->AllFireTrails.RemoveAt(Index);
			i = i - 1;
			
		}
	}
}


void ARacerGameModeBase::EndTurn(ACar* Car)
{
	ActiveCar = Car;

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(GetMovementState()));
	

	if (Car != NULL)
	{
		Car->HadTurn = true;
		CurrentTurn++;
		EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear;
		Car->EngineSpeedUp->FadeIn(0.4f, 0.4f, 0.0f, FadeCurve);
		if (Car->FutureMesh->IsVisible()) {
			Car->ToggleFutureVisibility();
		}

		if (Car->GetInitialGear() + 2 == Car->GetGear()) {
			Car->DraftAvailable = false;
			Car->DynamicMaterial->SetScalarParameterValue(TEXT("BlendDraft"), 0.0);
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Draft Used");
		}		

		if (!(Car->Crashed)) {

			if (Car->OrientationReset) {
				Car->OrientationReset = false;
				Car->Moving = false;
				Car->Update_Initial_Gear();
				Car->InitializeRotationVariables();
				SetMovementState(2);
				Car->SetFutureMeshColor(Car->ComputeFuturePosition(Car->GetGear(), Car->FutureMesh->GetRelativeRotation(), Car->GetActorLocation()));
				SetViewTarg();
			}

			else if (Car->Drifting && Car->GetGear() > 2 && Car->DriftPadReference != nullptr) {

				Car->StoredCurrentPosition = Car->DriftPadReference->DriftSpline->FindLocationClosestToWorldLocation(Car->GetActorLocation(), ESplineCoordinateSpace::World);
				
				float DriftDistance;
				if (Car->DriftPadReference->ExtremeTurn) {
					DriftDistance = (float)(Car->GetGear() - 1);
					Car->SetGear(1);
				}
				else
				{
					DriftDistance = (float)(Car->GetGear() - 2);
					Car->SetGear(2);
				}

				if ((Car->DriftPadReference->TurnOfUse + 3) <= CurrentTurn) {
					Car->DriftPadReference->RecentUse = false;
				}

				if (Car->DriftPadReference->DistanceOfUse == DriftDistance && Car->DriftPadReference->RecentUse) {
					DriftDistance += 1;
					if (DriftDistance > 4 && !Car->DriftPadReference->ExtremeTurn) {
						DriftDistance -= 2;
					}
					else if (DriftDistance > 5 && Car->DriftPadReference->ExtremeTurn) {
						DriftDistance -= 2;
					}
				}
				else {
					Car->DriftPadReference->RecentUse = true;
				}
				
				Car->DriftPadReference->DistanceOfUse = DriftDistance;
				Car->DriftPadReference->TurnOfUse = CurrentTurn;
				Car->DestinationPosition = Car->DriftPadReference->DriftSpline->GetLocationAtSplinePoint(DriftDistance, ESplineCoordinateSpace::World);
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "DestinationPosition: " + Car->DestinationPosition.ToString());



				SetMovementState(1);
				Car->ActiveDrifting = true;


			}

			else {

				FVector PossibleVector = Car->GetActorLocation();
				FVector Direction = Car->GetActorRightVector();
				PossibleVector += Direction * Car->GearToDistanceMapping[Car->GetGear()];
				//PossibleVector.Z = Car->HitBox->GetComponentLocation().Z;

				Car->DestinationPosition = PossibleVector;
				Car->StoredCurrentPosition = Car->HitBox->GetComponentLocation();
				float ZValue = Car->DestinationPosition.Z;
				Car->DestinationPosition.Z = Car->StoredCurrentPosition.Z;


				//Update Stored Spline Locations
				Car->PreviousSplineLocation = Car->CurrentSplineLocation;
				Car->CurrentSplineLocation = Car->CalculateSplineDistance(Car->DestinationPosition);


				//Check to see if lap has been completed
				if ((Car->CurrentSplineLocation < 1.5f && Car->PreviousSplineLocation > 28.0f) && Car->Checkpoint1 && Car->Checkpoint2) {
					Car->CurrentLap++;
					if (Car->CurrentLap >= 2) {
						Car->Winner = true;
					}
					Car->Checkpoint1 = false;
					Car->Checkpoint2 = false;
				}


				FRotator ResultRot = Car->GetActorRotation();
				FComponentQueryParams Params = FComponentQueryParams();
				Params.TraceTag = FName("CarMoveTrace");
				Params.bTraceComplex = true;
				Params.AddIgnoredActor(Car);
				Params.AddIgnoredActors(MyGameState->AllPowerUps);
				Params.AddIgnoredActors(MyGameState->AllDriftPoints);
				Params.AddIgnoredActors(MyGameState->AllItemBoxes);
				Params.AddIgnoredActors(MyGameState->AllJumpPads);
				for (AActor* Current : MyGameState->AllCheckPoints) {
					ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
					Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
				}
				if (MyGameState->AllOilSlicks.Num() > 0) {
					for (AOilSlick* Current : MyGameState->AllOilSlicks) {
						if (!Current->Fire->IsVisible()) {
							AActor* ActorSlick = Cast<AActor>(Current);
							Params.AddIgnoredActor(ActorSlick);
						}
					}
				}
				if (MyGameState->AllFireTrails.Num() > 0) {
					for (AFireTrail* Current : MyGameState->AllFireTrails) {
						AActor* ActorTrail = Cast<AActor>(Current);
						Params.AddIgnoredActor(ActorTrail);
					}
				}
				Params.AddIgnoredComponent(Car->FutureMesh);
				Car->CurrentTurnDegree = 0.0;
				GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ Car->OutResults, /*class UPrimitiveComponent* PrimComp*/ Car->HitBox, /*const FVector & Start*/ Car->StoredCurrentPosition,
					/*const FVector & End*/ Car->DestinationPosition, /*const FQuat & Rot*/ ResultRot, /*const FComponentQueryParams & Params*/ Params);

				Car->DestinationPosition.Z = ZValue;
				Car->StoredCurrentPosition.Z = ZValue;



				//Check if collision
				if (Car->OutResults.Num() > 0) {
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, UKismetSystemLibrary::GetDisplayName(Car->OutResults[0].Component.Get()));
					FVector CollisionPoint = Car->OutResults[0].Location;

					//collision
					
					if (Car->Health > 2 && Car->GetGear() != 6 || (Car->Health > 1 && Car->GetGear() <= 2)) {
						SetMovementState(1);
						Car->Bounce(Car->OutResults[0], FVector::ZeroVector, false);
					}
					else {
						//Crashing Animation
						//CollisionPoint.Z = 250.0;
						CollisionPoint.Z = 250.0 + Car->GetActorLocation().Z;
						Car->DestinationPosition = CollisionPoint;
						Car->Crashed = true;

						FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
						DetachmentRules.LocationRule = EDetachmentRule::KeepWorld;
						DetachmentRules.RotationRule = EDetachmentRule::KeepWorld;
						DetachmentRules.ScaleRule = EDetachmentRule::KeepWorld;

						Car->CameraComp->DetachFromComponent(DetachmentRules);
						//Car->CameraComp->SetWorldLocation(CollisionPoint);

						Car->StoredCurrentRotation = Car->GetActorRotation();
						Car->DestinationRotation = Car->StoredCurrentRotation;
						Car->DestinationRotation.Roll = Car->StoredCurrentRotation.Roll + -180;
						//Car->SpringArmRotation = Car->SpringArm->GetComponentRotation();
						if (!(Car->Fire->IsVisible())) {
							Car->Fire->ToggleVisibility();
						}

						FVector FireLocation = Car->Fire->GetComponentLocation();
						//FireLocation.Z = 0.0;
						FireLocation.Z = Car->GetActorLocation().Z;
						Car->Fire->SetWorldLocation(FireLocation);

						Car->Smoke->ToggleVisibility();

						Car->CrashingEffect->Play();

						Car->Moving = true;
						SetMovementState(1);


					}

				}
				else {
					if (Car->GetGear() < 5) {
						Car->CheckDraft();
					}
					Car->Moving = true;
					SetMovementState(1);
				}
			}

		}
		else {

			Car->RespawnCar();
			Car->SetGear(0);
			Car->Update_Initial_Gear();
			SetMovementState(2);
			SetViewTarg();
		}
		Car->EngineIdle->Stop();
	}
	
}


void ARacerGameModeBase::TimerReset() {

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Timer Clear");
	GetWorld()->GetTimerManager().ClearTimer(MovementTimerHandle);
	SetMovementState(0);


	if (IsEndOfRound()) {
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Turn Cycle is called");
		TurnCycle();
	}
	else {
		if (MyGameState->AIControllerActive) {
			AIActiveTurnCycle();
		}
		else {
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "AINotActiveTurnCycle is called");
			AINotActiveTurnCycle();
		}
	}

	SelectTrackCamera();


}


//Switch between cars occurs here if car crashes first during round
void ARacerGameModeBase::SetViewTarg() {
	ARacerPlayerController* RacerPlayer = Cast<ARacerPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));


	//new SetViewTarg
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "CurrentCarIndex Before Change: " + FString::FromInt(CurrentCarIndex));
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "SortedCars.Num(): " + FString::FromInt(SortedCars.Num()));
	if ((CurrentCarIndex + 1) % SortedCars.Num() == 0) {
		CalculateSplineDist();
		CurrentCarIndex = 0;
	}
	else {
		CurrentCarIndex += 1;
	}

	ACar* Skipped = NULL;
	for (int i = 0; i < SortedCars.Num(); i++) {
		if (i >= CurrentCarIndex) continue;
		if (!(SortedCars[i]->HadTurn)) {
			Skipped = SortedCars[i];
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Skipped Set");
		}
	}

	

	if (Skipped != NULL) {
		ActiveCar = Skipped;
		RacerPlayer->SetViewTargetWithBlend(ActiveCar, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "View Target Blend w/ skipped car");
		GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &ARacerGameModeBase::TimerReset, 1.0f, false);
	}
	else if (SortedCars[CurrentCarIndex] == ActiveCar) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "No Blend");
		TimerReset();
	}
	else {
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "SetViewTarg new car");
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "CurrentCarIndex After Change: " + FString::FromInt(CurrentCarIndex));
		ActiveCar = SortedCars[CurrentCarIndex];
		RacerPlayer->SetViewTargetWithBlend(ActiveCar, 1.0f);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "View Target Blend");
		GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &ARacerGameModeBase::TimerReset, 1.0f, false);
	}
	



}

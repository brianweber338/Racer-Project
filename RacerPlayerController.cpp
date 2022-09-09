// Fill out your copyright notice in the Description page of Project Settings.


#include "RacerPlayerController.h"
#include "DriftPoint.h"
#include "RacerGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include <Racer\Car.h>


ARacerPlayerController::ARacerPlayerController() 
{
	GameMode = Cast<ARacerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

}


void ARacerPlayerController::EndTurn()
{
	ACar* Car = Cast<ACar>(GetPawn());

	GameMode->EndTurn(Car);
}


void ARacerPlayerController::Drift() {

	ACar* Car = Cast<ACar>(GetPawn());

	if (Car != NULL && Car->GetGear() > 2) {
		if (Car->Drifting && Car->DriftPadReference != NULL && Car->DriftPadReference != nullptr) {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Time to Drift!");
			
			EndTurn();


		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Drifting Unavailable!");
		}
	}
}

void ARacerPlayerController::SteerLeft() {

	ACar* Car = Cast<ACar>(GetPawn());

	if ((Car != NULL) && (Car->GetInitialGear() != 0))
	{

		Car->TurnLeft = !(Car->TurnLeft);

		if (!(Car->TurnLeft)) {
			Car->FTime = 0;
			Car->CurrentRotation = Car->GetActorRotation().Yaw;
			Car->SetFutureMeshColor(Car->ComputeFuturePosition(Car->GetGear(), FRotator::ZeroRotator, Car->GetActorLocation()));
		}
		
	}

}

void ARacerPlayerController::SteerRight() {

	ACar* Car = Cast<ACar>(GetPawn());

	if ((Car != NULL) && (Car->GetInitialGear() != 0))
	{

		Car->TurnRight = !(Car->TurnRight);

		if (!(Car->TurnRight)) {
			Car->FTime = 0;
			Car->CurrentRotation = Car->GetActorRotation().Yaw;
			Car->SetFutureMeshColor(Car->ComputeFuturePosition(Car->GetGear(), FRotator::ZeroRotator, Car->GetActorLocation()));
		}		
		
	}

}



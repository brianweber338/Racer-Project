// Fill out your copyright notice in the Description page of Project Settings.


#include "RacerGameStateBase.h"
#include "Car.h"
#include "CheckPoint.h"
#include "RepairItem.h"
#include "JumpPad.h"
#include "ItemBox.h"
#include "FireTrail.h"
#include "TrackCamera.h"
#include "DriftPoint.h"
#include "Kismet/GameplayStatics.h"
#include "RacerGameModeBase.h"



ARacerGameStateBase::ARacerGameStateBase() {

	AIControllerActive = true;

	OutActors;
	TSubclassOf<ACar> AllCars = ACar::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllCars, OutActors);

	MovementState = 0;

	AllPowerUps;
	TSubclassOf<ARepairItem> AllRepairItems = ARepairItem::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllRepairItems, AllPowerUps);


	AllDriftPoints;
	TSubclassOf<ADriftPoint> AllDrifts = ADriftPoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllDrifts, AllDriftPoints);


	AllTrackCameras;
	TSubclassOf<ATrackCamera> AllCams = ATrackCamera::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllCams, AllTrackCameras);
	

	AllItemBoxes;
	TSubclassOf<AItemBox> AllBoxes = AItemBox::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllBoxes, AllItemBoxes);


	AllOilSlicks;
	AllFireTrails;


	AllJumpPads;
	TSubclassOf<AJumpPad> AllJumps = AJumpPad::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllJumps, AllJumpPads);

	AllCheckPoints;
	TSubclassOf<ACheckPoint> AllPoints = ACheckPoint::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AllPoints, AllCheckPoints);

}

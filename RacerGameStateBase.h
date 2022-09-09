// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "OilSlick.h"
#include "FireTrail.h"
#include "RacerGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class RACER_API ARacerGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:

	ARacerGameStateBase();


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AIControllerActive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<AActor*> OutActors;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllPowerUps;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllDriftPoints;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllTrackCameras;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllItemBoxes;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllJumpPads;

	UPROPERTY(EditAnywhere)
		TArray<AOilSlick*> AllOilSlicks;

	UPROPERTY(EditAnywhere)
		TArray<AFireTrail*> AllFireTrails;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> AllCheckPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int MovementState;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RacerGameStateBase.h"
#include "TrackCamera.h"
#include "TimerManager.h"
#include "RacerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class RACER_API ARacerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	


protected:

	virtual void BeginPlay() override;
	
public:

	ARacerGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentRound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentCarIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimerHandle MovementTimerHandle;

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void EndTurn(ACar* Car);

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void TurnCycle();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		bool IsEndOfRound();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		bool IsActiveCarNull();

	UFUNCTION(Category = "RacerGameModeBase")
		void TimerReset();


	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void AIActiveTurnCycle();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void AINotActiveTurnCycle();

	UFUNCTION(BlueprintCallable, Category = "RacerGameStateBase")
		int GetMovementState();

	UFUNCTION(BlueprintCallable, Category = "RacerGameStateBase")
		void SetMovementState(int NewState);

	UFUNCTION(BlueprintCallable, Category = "RacerGameStateBase")
		void SetViewTarg();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void SelectTrackCamera();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void RespawnItemBoxes();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void DespawnFireTrails();

	UFUNCTION(BlueprintCallable, Category = "RacerGameModeBase")
		void CalculateSplineDist();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HeadToHead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ACar*> SortedCars;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ACar* ActiveCar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ATrackCamera* CurrentActiveCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AIActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ARacerGameStateBase* MyGameState;


	//Console Commands
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DrawTireDebugLineCommand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DrawCollisionDebugLineCommand;

};

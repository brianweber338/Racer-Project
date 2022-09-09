// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RacerGameModeBase.h"
#include "RacerAIController.generated.h"

/**
 * 
 */
UCLASS()
class RACER_API ARacerAIController : public AAIController
{
	GENERATED_BODY()
	

public:

	ARacerAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ARacerGameModeBase* GameMode;

	UFUNCTION(BlueprintCallable, Category = "RacerAIController")
		void EndTurn();


	UFUNCTION(BlueprintCallable, Category = "RacerAIController")
		void DecideMove();

	UFUNCTION(BlueprintCallable, Category = "RacerAIController")
		void CalculateMove(ACar* Car);

	UFUNCTION(BlueprintCallable, Category = "RacerAIController")
		float CalculateNextMove(ACar* Car, int MoveCount, FVector Position, FRotator Rotation, int CurrentGear, float RelativeSpline);

	UPROPERTY(EditAnywhere)
		int OverallBestGear;

	UPROPERTY(EditAnywhere)
		int OverallBestTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float OverallBestSpline;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	bool CompletedLap;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentRelativeSpline;*/
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RacerGameModeBase.h"
#include "RacerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class RACER_API ARacerPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:

	ARacerPlayerController();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ARacerGameModeBase* GameMode;

	UFUNCTION(BlueprintCallable, Category = "RacerPlayerController")
		void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "RacerPlayerController")
		void Drift();

	UFUNCTION(BlueprintCallable, Category = "RacerPlayerController")
		void SteerLeft();

	UFUNCTION(BlueprintCallable, Category = "RacerPlayerController")
		void SteerRight();

};



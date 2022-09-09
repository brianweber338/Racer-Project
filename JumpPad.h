// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Actor.h"
#include "JumpPad.generated.h"

UCLASS()
class RACER_API AJumpPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJumpPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* JumpPad;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* PadHitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* JumpSpline;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* BounceNoise;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int JumpsCompleted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int RoundOfLastUse;

};

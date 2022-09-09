// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "TrackCamera.generated.h"

UCLASS()
class RACER_API ATrackCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Model;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneCaptureComponent2D* ModelCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		AActor* CarToFollow;

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	AActor* PreviousCarToFollow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool ActiveCamera;

};

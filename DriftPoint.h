// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "DriftPoint.generated.h"

UCLASS()
class RACER_API ADriftPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADriftPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* DriftPad;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* PadHitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USplineComponent* DriftSpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ExtremeTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool RecentUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Loop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int DistanceOfUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TurnOfUse;

	UFUNCTION(Category = "Collision")
		void OnPadHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "Collision")
		void OnPadHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};

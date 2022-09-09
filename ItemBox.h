// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ItemBox.generated.h"

UCLASS()
class RACER_API AItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
		int RoundOfUse;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* PickUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float YAWPERSECOND;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool Inactive;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool MessagePlayed;

	UFUNCTION(Category = "Collision")
		void OnPickUpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION(Category = "Collision")
		void OnPickUpEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/


};

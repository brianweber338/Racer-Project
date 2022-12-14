// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "OilSlick.generated.h"

UCLASS()
class RACER_API AOilSlick : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOilSlick();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Slick;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* SlickHitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire;

	UPROPERTY(VisibleAnywhere)
		int TurnCreated;

	UFUNCTION(Category = "Collision")
		void OnSlickHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION(Category = "Collision")
		void OnSlickBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/

};

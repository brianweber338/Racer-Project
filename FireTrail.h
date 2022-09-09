// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "FireTrail.generated.h"

UCLASS()
class RACER_API AFireTrail : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFireTrail();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* TrailRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Tread1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Tread2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire6;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire7;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire8;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox5;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox6;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox7;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox8;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int TurnCreated;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		int RoundCreated;

	//UFUNCTION(Category = "Collision")
	//	void OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnHitBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

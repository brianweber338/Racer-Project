// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Track_Spline.h"
#include "JumpPad.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "CollisionQueryParams.h"
#include "GameFramework/Pawn.h"
#include "RacerGameModeBase.h"
#include "DriftPoint.h"
#include "Car.generated.h"

class UMaterialInstanceDynamic;


UENUM(BlueprintType)
enum class CarType : uint8 {
	BODY_MuscleCar	UMETA(DisplayName="Muscle Car"),
	BODY_DerbyCar	UMETA(DisplayName = "Derby Car"),
	BODY_GoCart	UMETA(DisplayName = "Go Cart"),
	BODY_Truck	UMETA(DisplayName = "Truck"),
};


UCLASS()
class RACER_API ACar : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Gear;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Initial_Gear;



public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		CarType CarBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* CarBase;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* FutureMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* CowCatcher;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Fire;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* TireFire1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* TireFire2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* TireFire3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* TireFire4;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* Smoke;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* HitBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* LeftFrontWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* RightFrontWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* LeftBackWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* RightBackWheel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* CrashingEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* RepairNoise;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* CollisionNoise;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* EngineIdle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* EngineSpeedUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UAudioComponent* OilSlip;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ADriftPoint* DriftPadReference;


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<float> GearToDistanceMapping;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		TArray<float> GearToTurnMapping;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		ARacerGameModeBase* GameMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TArray<FHitResult> OutResults;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float FTime;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int AllowedTurns;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TurnsInWrongDirection; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AJumpPad* JumpPadReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TurnsSincePickUp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float TotalDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float HitBoxDifference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentTurnDegree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float PreviousSplineLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentSplineLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CurrentLap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int PotentialDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Checkpoint1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Checkpoint2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Moving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Turning;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Crashed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CarToCar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PotentialCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool LightCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Bounced;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool AdditionalBounce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool SpinOut;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Drifting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Jumped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool DraftAvailable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ActiveDrifting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TurnLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool TurnRight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool InWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasPowerUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool PowerUpActive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasCowCatcher;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasOilSlick;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OilSlicked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasNitro;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HasFireTires;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool UsedFireTires;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool HadTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool Winner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool OrientationReset;

	


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InitialRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaximumLeftTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float MaximumRightTurn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString PowerUpName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int RoundsSincePowerUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int WeightClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int CarRank;

	UPROPERTY(BlueprintReadWrite)
		int MAX_HEALTH;

	UPROPERTY(BlueprintReadWrite)
		float TURNSPEED;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AOilSlick> ToSpawn;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AFireTrail> ToSpawnFire;

	UPROPERTY(EditAnywhere)
		ATrack_Spline* MySpline;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DestinationPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector StoredCurrentPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector BouncedMirror;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator DestinationRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator StoredCurrentRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator SpringArmRotation;

	UPROPERTY(EditAnywhere)
		TArray<AActor*> OutActors;

	UPROPERTY(VisibleAnywhere)
		TArray<USceneComponent*> Tires;

//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSubClassOf< AActor > TrackSplineClass;

	UFUNCTION(BlueprintCallable, Category = "Car")
		FVector ComputeFuturePosition(int NewGear, FRotator Rot, FVector InitialLocation);

	UFUNCTION(BlueprintCallable)
		void SpawnOilSlick();

	UFUNCTION(BlueprintCallable)
		void SpawnFireTrail();

	UFUNCTION()
		int GetGear();

	UFUNCTION()
		void AssignCarCharacteristics();

	UFUNCTION()
		void SetGear(int NewGear);

	UFUNCTION(BlueprintCallable, Category = "Car")
		int GetInitialGear();

	UFUNCTION()
		void Update_Initial_Gear();

	

	UFUNCTION()
		FVector GetCarForwardVector();

	UFUNCTION(BlueprintCallable, Category = "Car")
		bool IsHeadToHead(ACar* OtherCar);

	UFUNCTION(BlueprintCallable, Category = "Car")
		void ResetOrientation();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void ToggleFutureVisibility();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void BounceOtherCar(FHitResult Hit, bool HeavyDamage, FVector InverseVector);

	UFUNCTION(BlueprintCallable, Category = "Car")
		void Bounce(FHitResult Hit, FVector BouncedInitialVector, bool UsedBouncedInitialVector);

	UFUNCTION(BlueprintCallable, Category = "Spline")
		float CalculateSplineDistance(FVector ActorPosition);

	UFUNCTION(BlueprintCallable, Category = "Spline")
		float CalculateSplineOrientation(float ActorDist, FVector ActorPosition);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
		void InitializeRotationVariables();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void Upshift();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void Downshift();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void TireRotation();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void SetFutureMeshColor(FVector FuturePosition);

	UFUNCTION(BlueprintCallable, Category = "Car")
		void CheckDraft();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void RespawnCar();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void StorePowerUp();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void UsePowerUp();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void AssessDamage();

	UFUNCTION(BlueprintCallable, Category = "Car")
		void CheckRoundsSincePowerUp();

	UFUNCTION(Category = "Collision")
		void OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(Category = "Collision")
		void OnHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	UMaterialInstanceDynamic* DynamicMaterial;

};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "RacerGameModeBase.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/StaticMeshComponent.h"
#include "OilSlick.h"
#include "ItemBox.h"
#include "CheckPoint.h"
#include "JumpPad.h"
#include "FireTrail.h"
#include "RepairItem.h"
#include "DriftPoint.h"
#include "Materials/MaterialInterface.h"
#include "Engine/EngineTypes.h"
#include "Components/SceneComponent.h"
#include "Components/AudioComponent.h"
#include "DrawDebugHelpers.h"
#include "Track_Spline.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"




// Sets default values
ACar::ACar()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Creates Mesh and sets as Root component
	CarBase = CreateDefaultSubobject<USceneComponent>("CarBase");
	RootComponent = CarBase;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	FutureMesh = CreateDefaultSubobject<UStaticMeshComponent>("FutureMesh");
	FutureMesh->SetupAttachment(Mesh);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	CameraComp = CreateDefaultSubobject<UCameraComponent>("Cam");
	Fire = CreateDefaultSubobject<UParticleSystemComponent>("Fire");
	TireFire1 = CreateDefaultSubobject<UParticleSystemComponent>("TireFire1");
	TireFire2 = CreateDefaultSubobject<UParticleSystemComponent>("TireFire2");
	TireFire3 = CreateDefaultSubobject<UParticleSystemComponent>("TireFire3");
	TireFire4 = CreateDefaultSubobject<UParticleSystemComponent>("TireFire4");
	Smoke = CreateDefaultSubobject<UParticleSystemComponent>("Smoke");
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	HitBox->SetupAttachment(Mesh);
	CrashingEffect = CreateDefaultSubobject<UAudioComponent>("CrashingEffect");
	RepairNoise = CreateDefaultSubobject<UAudioComponent>("RepairNoise");
	CollisionNoise = CreateDefaultSubobject<UAudioComponent>("CollisionNoise");
	OilSlip = CreateDefaultSubobject<UAudioComponent>("OilSlip");
	EngineIdle = CreateDefaultSubobject<UAudioComponent>("EngineIdle");
	EngineSpeedUp = CreateDefaultSubobject<UAudioComponent>("EngineSpeedUp");


	CowCatcher = CreateDefaultSubobject<UStaticMeshComponent>("CowCatcher");
	CowCatcher->SetupAttachment(Mesh);


	LeftFrontWheel = CreateDefaultSubobject<USceneComponent>("LeftFrontWheel");
	RightFrontWheel = CreateDefaultSubobject<USceneComponent>("RightFrontWheel");
	LeftBackWheel = CreateDefaultSubobject<USceneComponent>("LeftBackWheel");
	RightBackWheel = CreateDefaultSubobject<USceneComponent>("RightBackWheel");
	LeftFrontWheel->SetupAttachment(Mesh);
	RightFrontWheel->SetupAttachment(Mesh);
	LeftBackWheel->SetupAttachment(Mesh);
	RightBackWheel->SetupAttachment(Mesh);
	Fire->SetupAttachment(Mesh);
	Smoke->SetupAttachment(Mesh);
	SpringArm->SetupAttachment(HitBox);
	CameraComp->SetupAttachment(SpringArm);

	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag= true;

	
	Tires.Add(LeftFrontWheel);
	Tires.Add(RightFrontWheel);
	Tires.Add(LeftBackWheel);
	Tires.Add(RightBackWheel);

	CarBody;
	WeightClass;
	
	CarRank = 0;


	//Game Variables
	MAX_HEALTH = 5;
	TURNSPEED = 0.4;

	//AllowedTurns = 0;

	//Used by AI to rubberband if stuck
	TurnsInWrongDirection = 0;

	Gear = 0;
	Initial_Gear = 0;
	FTime = 0.0;

	TotalDistance= 0.0;
	CurrentLap = 0;

	// 6 gears plus neutral, for a total of 7.
	//Values dependent on car type
	GearToDistanceMapping.SetNum(7);


	// 6 gears plus neutral, for a total of 7.
	//Values dependent on car type
	GearToTurnMapping.SetNum(7);

	// Netural is 0, car can't turn
	
	
	PotentialDamage = 0;
	CurrentTurnDegree = 0;

	GameMode;
	DriftPadReference;
	JumpPadReference;

	
	Checkpoint1 = true;
	Checkpoint2 = true;


	Moving = false;
	Crashed = false;
	PotentialCollision = false;
	Turning = false;
	Bounced = false;
	AdditionalBounce = false;
	SpinOut = false;
	Drifting = false;
	CarToCar = false;
	Jumped = false;
	ActiveDrifting = false;
	LightCollision = true;

	HasCowCatcher = false;
	HasOilSlick = false;
	OilSlicked = false;
	HasNitro = false;
	HasFireTires = false;
	UsedFireTires = false;
	PowerUpActive = false;
	HasPowerUp = false;
	PowerUpName = "None";
	RoundsSincePowerUp = 0;


	HadTurn = false;
	Winner = false;

	DraftAvailable = false;

	TurnLeft = false;
	TurnRight = false;
	InWall = false;
	InitialRotation = 0.0;
	MaximumLeftTurn;
	MaximumRightTurn;
	CurrentRotation;

	OrientationReset = false;
	

	PreviousSplineLocation;
	CurrentSplineLocation;

	StoredCurrentPosition;
	DestinationPosition;

	StoredCurrentRotation;
	DestinationRotation;

	SpringArmRotation;

	OutResults;

	BouncedMirror;

	OutActors;
	MySpline;

	


	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ACar::OnHitBoxBeginOverlap);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &ACar::OnHitBoxEndOverlap);


}

// Called when the game starts or when spawned
void ACar::BeginPlay()
{

	GameMode = Cast<ARacerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	AssignCarCharacteristics();

	TSubclassOf<ATrack_Spline> Track_Spline = ATrack_Spline::StaticClass();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), Track_Spline, OutActors);
	MySpline = Cast<ATrack_Spline>(OutActors[0]);
	if (MySpline == nullptr) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Nullptr");
	}

	StoredCurrentPosition = HitBox->GetComponentLocation();

	PreviousSplineLocation = CalculateSplineDistance(StoredCurrentPosition);
	CurrentSplineLocation = PreviousSplineLocation;

	HitBoxDifference = HitBox->GetComponentLocation().Z;

	Health = MAX_HEALTH;

	FutureMesh->SetMobility(EComponentMobility::Movable);

	auto Material = FutureMesh->GetMaterial(0);

	DynamicMaterial = UMaterialInstanceDynamic::Create(Material, NULL);
	FutureMesh->SetMaterial(0, DynamicMaterial);

	CrashingEffect->Stop();
	RepairNoise->Stop();
	CollisionNoise->Stop();
	OilSlip->Stop();
	EngineIdle->Stop();
	EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear;
	EngineIdle->FadeIn(0.2f, 0.5f, 0.0f, FadeCurve);
	
	EngineSpeedUp->Stop();

	Super::BeginPlay();


}

// Called every frame
void ACar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (Jumped) {
		FTime += DeltaTime;

		StoredCurrentPosition = JumpPadReference->JumpSpline->FindLocationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);

		FVector NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, 0.5f);
		SetActorLocation(NewLocation);

		TireRotation();

		if (NewLocation == DestinationPosition) {
			Jumped = false;
			Gear = 2;
			Initial_Gear = Gear;
			//AllowedTurns = 0;
			InitializeRotationVariables();
			//Sets Mesh for next turn
			GameMode->SetMovementState(2);
			SetFutureMeshColor(ComputeFuturePosition(GetGear(), FutureMesh->GetRelativeRotation(), GetActorLocation()));
			GameMode->SetViewTarg();
		}


	}


	if (ActiveDrifting) {
		FTime += DeltaTime;

		//Change NewLocation value
		FVector NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, Initial_Gear/2);
		SetActorLocation(NewLocation);
		FRotator DriftRotation = DriftPadReference->DriftSpline->GetRotationAtSplineInputKey(DriftPadReference->DriftSpline->FindInputKeyClosestToWorldLocation(GetActorLocation()), ESplineCoordinateSpace::World);
		DriftRotation.Yaw -= 90.0;
		SetActorRotation(DriftRotation);
		TireRotation();

		if (GetActorLocation() == DestinationPosition) {
			FTime = 0;
			ActiveDrifting = false;
			DriftPadReference = NULL;
			Initial_Gear = Gear;
			InitializeRotationVariables();
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Final Actor Location: " + GetActorLocation().ToString());
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "DestinationPosition: " + DestinationPosition.ToString());
			GameMode->SetMovementState(2);
			//Sets Mesh for next turn
			SetFutureMeshColor(ComputeFuturePosition(GetGear(), FutureMesh->GetRelativeRotation(), GetActorLocation()));
			GameMode->SetViewTarg();
		}

	}


	if (TurnLeft) {
		FTime += DeltaTime;
		if (((CurrentRotation - TURNSPEED * FTime) > MaximumLeftTurn) || (((CurrentRotation - TURNSPEED * FTime) + 360) - MaximumLeftTurn) < GearToTurnMapping[Initial_Gear] * 6) {
			float TempRotation = CurrentRotation;
			if ((CurrentRotation - TURNSPEED * FTime) < 0.0) {
				CurrentRotation += 360;
			}
			CurrentRotation -= TURNSPEED * FTime;
			if (InWall) {
				if ((TempRotation + 3.0) > 360) {
					TempRotation -= 360;
				}
				CurrentRotation = TempRotation + 3.0;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "InWall, Rotation Reset");
			}
			

			FRotator Current = GetActorRotation();
			Current.Yaw = CurrentRotation;
			SetActorRotation(Current);
			TireRotation();
		}
	}

	if (TurnRight) {
		FTime += DeltaTime;
		if (((CurrentRotation + TURNSPEED * FTime) < MaximumRightTurn) || (((CurrentRotation + TURNSPEED * FTime) - 360) - MaximumRightTurn) > -(GearToTurnMapping[Initial_Gear] * 6)) {
			float TempRotation = CurrentRotation;
			if ((CurrentRotation + TURNSPEED * FTime) > 360.0) {
				CurrentRotation -= 360;
			}
			CurrentRotation += TURNSPEED * FTime;
			if (InWall) {
				if ((TempRotation - 3.0) < 0.0) {
					TempRotation += 360;
				}
				CurrentRotation = TempRotation - 3.0;
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "InWall, Rotation Reset");
			}

			FRotator Current = GetActorRotation();
			Current.Yaw = CurrentRotation;
			SetActorRotation(Current);
			TireRotation();
		}
	}

	if (OilSlicked) {
		FRotator SlickRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, Gear * .75f + (0.33 * (FTime + 1)));
		SetActorRotation(SlickRotation);

		if (GetActorRotation() == DestinationRotation || GameMode->GetMovementState() != 1) {
			OilSlicked = false;
			SetActorRotation(DestinationRotation);
			InitializeRotationVariables();
		}

	}


	if (CarToCar) {

		FTime += DeltaTime;
		FVector NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, Gear * 1.25f + (0.33 * (FTime + 1)));
		SetActorLocation(NewLocation);
		

		if (Crashed) {
			FRotator NewRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, Gear* 1.75f + (0.33 * (FTime + 1)));
			SetActorRotation(NewRotation);
			AdditionalBounce = false;

		}
		else {
			TireRotation();
		}

		if (NewLocation == DestinationPosition) {
			FTime = 0;
			CarToCar = false;
			Initial_Gear = Gear;
			//AllowedTurns = 0;
			if (AdditionalBounce) {
				//do some shit
				CollisionNoise->Play();
				StoredCurrentPosition = NewLocation;
				DestinationPosition = BouncedMirror;
				SpinOut = true;
			}
			else {
				InitializeRotationVariables();
				GameMode->SetMovementState(2);
				//Sets Mesh for next turn
				SetFutureMeshColor(ComputeFuturePosition(GetGear(), FutureMesh->GetRelativeRotation(), GetActorLocation()));
			}
			
		}

	}


	if (Bounced) {
		FTime += DeltaTime;

		FVector NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, Initial_Gear * 1.25f + (0.33 * (FTime + 1)));
		SetActorLocation(NewLocation);
		float TargetFOV = (90.0 + (Gear * 5.0));
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, FTime, Initial_Gear * 0.75f + (0.33 * (FTime + 1)));
		CameraComp->SetFieldOfView(NewFOV);
		TireRotation();

		if (NewLocation == DestinationPosition) {
			CollisionNoise->Play();
			StoredCurrentPosition = NewLocation;
			DestinationPosition = BouncedMirror;
			SpinOut = true;
			Bounced = false;
			FTime = 0;
		}
	}

	if (SpinOut) {
		FTime += DeltaTime;
		FVector NewLocation;

		//NewLocation = FMath::VInterpNormalRotationTo(StoredCurrentPosition, DestinationPosition, FTime, Gear * 1.5f + (0.33 * (FTime + 1)));
		//SetActorLocation(NewLocation);
		NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, Gear * 1.25f + (0.33 * (FTime + 1)));
		SetActorLocation(NewLocation);

		if (Crashed) {
			FRotator NewRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, Initial_Gear * 0.75f + ((float(Gear - Initial_Gear)) * (0.33 * (FTime + 1))));
			SetActorRotation(NewRotation);

		}
		else {
			FRotator NewRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, Gear * 2.25f + (0.33 * (FTime + 1)));
			SetActorRotation(NewRotation);
			TireRotation();
		}

		if (NewLocation == DestinationPosition) {
			FTime = 0;
			SpinOut = false;
			if (AdditionalBounce) {
				AdditionalBounce = false;
			}
			Initial_Gear = Gear;
			//AllowedTurns = 0;
			InitializeRotationVariables();
			GameMode->SetMovementState(2);
			//Sets Mesh for next turn
			SetFutureMeshColor(ComputeFuturePosition(GetGear(), FutureMesh->GetRelativeRotation(), GetActorLocation()));
			GameMode->SetViewTarg();
			
		}
	}


	//Now only used by AI
	if (Turning) {
		FTime+= DeltaTime;
		FRotator NewRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, 7.0f);
		SetActorRotation(NewRotation);

		if (NewRotation == DestinationRotation) {
			Turning = false;
			SetFutureMeshColor(ComputeFuturePosition(Gear, FRotator::ZeroRotator, GetActorLocation()));
			FTime = 0;

		}
	}

	if (Moving && GameMode->GetMovementState() == 1) {
		FTime += DeltaTime;

		FVector NewLocation = FMath::VInterpTo(StoredCurrentPosition, DestinationPosition, FTime, Initial_Gear * 0.75f + ((float(Gear - Initial_Gear)) * (0.33 * (FTime + 1))));
		SetActorLocation(NewLocation);
		float TargetFOV = (90.0 + (Gear * 5.0));
		float NewFOV = FMath::FInterpTo(CameraComp->FieldOfView, TargetFOV, FTime, Initial_Gear * 0.75f + ((float(Gear - Initial_Gear)) * (0.33 * (FTime + 1))));
		CameraComp->SetFieldOfView(NewFOV);
		
		

		if (Crashed) {
			FRotator NewRotation = FMath::RInterpTo(StoredCurrentRotation, DestinationRotation, FTime, Initial_Gear * 0.75f + ((float(Gear - Initial_Gear)) * (0.33 * (FTime + 1))));
			SetActorRotation(NewRotation);

		}
		else {
			TireRotation();
		}
		if (NewLocation == DestinationPosition) {
			FTime = 0;
			Moving = false;
			Initial_Gear = Gear;
			//AllowedTurns = 0;
			InitializeRotationVariables();
			//Sets Mesh for next turn
			GameMode->SetMovementState(2);
			SetFutureMeshColor(ComputeFuturePosition(GetGear(), FutureMesh->GetRelativeRotation(), GetActorLocation()));
			GameMode->SetViewTarg();

		}
	}

	if (EngineSpeedUp->IsPlaying() && GameMode->GetMovementState() != 1) {
		EAudioFaderCurve FadeCurve = EAudioFaderCurve::Linear;
		EngineSpeedUp->FadeOut(0.3, 0.1, FadeCurve);
		EngineIdle->FadeIn(0.2f, 0.5f, 0.0f, FadeCurve);
	}

}


void ACar::InitializeRotationVariables() {

	int BaseRotation = ((int)(GetActorRotation().Yaw + 360.0));
	InitialRotation = (float) (BaseRotation % 360);
	CurrentRotation = InitialRotation;
	MaximumLeftTurn = (float) ((int)(BaseRotation - GearToTurnMapping[Initial_Gear]*3) % 360);
	MaximumRightTurn = (float) ((int)(BaseRotation + GearToTurnMapping[Initial_Gear]*3) % 360);

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "MaximumLeftTurn " + FString::SanitizeFloat(MaximumLeftTurn));
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "MaximumRightTurn: " + FString::SanitizeFloat(MaximumRightTurn));


}


void ACar::AssignCarCharacteristics() {

	//Default
	//Normal Top Speed, Normal Acceleration
	//Normal turn radius, Second highest weight
	if (CarBody == CarType::BODY_MuscleCar) {
		GearToDistanceMapping[0] = 0;
		GearToDistanceMapping[1] = 1000;
		GearToDistanceMapping[2] = 2000;
		GearToDistanceMapping[3] = 3000;
		GearToDistanceMapping[4] = 4000;
		GearToDistanceMapping[5] = 5000;
		GearToDistanceMapping[6] = 6000;


		GearToTurnMapping[0] = 0.0;
		GearToTurnMapping[1] = 30.0;
		GearToTurnMapping[2] = 25.0;
		GearToTurnMapping[3] = 20.0;
		GearToTurnMapping[4] = 15.0;
		GearToTurnMapping[5] = 10.0;
		GearToTurnMapping[6] = 5.0;

		WeightClass = 3;
	}

	//Lower Top Speed, High Acceleration
	//+20% better turn radius, Lowest weight
	else if (CarBody == CarType::BODY_GoCart) {
		GearToDistanceMapping[0] = 0;
		GearToDistanceMapping[1] = 1800;
		GearToDistanceMapping[2] = 3000;
		GearToDistanceMapping[3] = 3600;
		GearToDistanceMapping[4] = 4000;
		GearToDistanceMapping[5] = 4400;
		GearToDistanceMapping[6] = 4800;


		GearToTurnMapping[0] = 0.0;
		GearToTurnMapping[1] = 36.0;
		GearToTurnMapping[2] = 30.0;
		GearToTurnMapping[3] = 24.0;
		GearToTurnMapping[4] = 18.0;
		GearToTurnMapping[5] = 12.0;
		GearToTurnMapping[6] = 6.0;

		WeightClass = 1;
	}

	//Lower Top Speed, Medium Acceleration
	//Normal Turn Radius, second lowest weight
	else if (CarBody == CarType::BODY_DerbyCar) {
		GearToDistanceMapping[0] = 0;
		GearToDistanceMapping[1] = 1400;
		GearToDistanceMapping[2] = 2600;
		GearToDistanceMapping[3] = 3600;
		GearToDistanceMapping[4] = 4400;
		GearToDistanceMapping[5] = 5000;
		GearToDistanceMapping[6] = 5400;


		GearToTurnMapping[0] = 0.0;
		GearToTurnMapping[1] = 30.0;
		GearToTurnMapping[2] = 25.0;
		GearToTurnMapping[3] = 20.0;
		GearToTurnMapping[4] = 15.0;
		GearToTurnMapping[5] = 10.0;
		GearToTurnMapping[6] = 5.0;


		WeightClass = 2;
	}

	//Higher Top Speed, Low Acceleration (at start)
	//Smaller turn radious, highest weight
	else if (CarBody == CarType::BODY_Truck) {

		GearToDistanceMapping[0] = 0;
		GearToDistanceMapping[1] = 700;
		GearToDistanceMapping[2] = 1400;
		GearToDistanceMapping[3] = 2400;
		GearToDistanceMapping[4] = 3600;
		GearToDistanceMapping[5] = 5000;
		GearToDistanceMapping[6] = 6600;


		GearToTurnMapping[0] = 0.0;
		GearToTurnMapping[1] = 27.0;
		GearToTurnMapping[2] = 22.5;
		GearToTurnMapping[3] = 18.0;
		GearToTurnMapping[4] = 13.5;
		GearToTurnMapping[5] = 9.0;
		GearToTurnMapping[6] = 4.5;

		WeightClass = 4;

	}


}

void ACar::ResetOrientation() {
	FRotator NewRotation = MySpline->Spline->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World);
	NewRotation.Yaw -= 90.0;
	SetActorRotation(NewRotation);
	OrientationReset = true;
	GameMode->EndTurn(this);
}

void ACar::CheckDraft() {

	FComponentQueryParams Params = FComponentQueryParams();
	Params.TraceTag = FName("FutureMoveTrace");
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);

	Params.AddIgnoredActors(GameMode->MyGameState->AllPowerUps);
	Params.AddIgnoredActors(GameMode->MyGameState->AllDriftPoints);
	Params.AddIgnoredActors(GameMode->MyGameState->AllItemBoxes);
	Params.AddIgnoredActors(GameMode->MyGameState->AllJumpPads);
	for (AActor* Current : GameMode->MyGameState->AllCheckPoints) {
		ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
		Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
	}
	if (GameMode->MyGameState->AllOilSlicks.Num() > 0) {
		for (AOilSlick* Current : GameMode->MyGameState->AllOilSlicks) {
			AActor* ActorSlick = Cast<AActor>(Current);
			Params.AddIgnoredActor(ActorSlick);
		}
	}
	if (GameMode->MyGameState->AllFireTrails.Num() > 0) {
		for (AFireTrail* Current : GameMode->MyGameState->AllFireTrails) {
			AActor* ActorTrail = Cast<AActor>(Current);
			Params.AddIgnoredActor(ActorTrail);
		}
	}

	FVector DraftPosition = ComputeFuturePosition(Gear + 2, Mesh->GetRelativeRotation(), GetActorLocation());
	DraftPosition.Z = HitBox->GetComponentLocation().Z;
	

	GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ OutResults, /*class UPrimitiveComponent* PrimComp*/ HitBox, /*const FVector & Start*/ HitBox->GetComponentLocation(),
		/*const FVector & End*/ DraftPosition, /*const FQuat & Rot*/ GetActorRotation(), /*const FComponentQueryParams & Params*/ Params);

	if (OutResults.Num() > 0) {
		if (OutResults[0].GetActor()->IsA<ACar>()) {
			ACar* OtherCar = Cast<ACar>(OutResults[0].GetActor());
			float CarDirections = FVector::DotProduct(OtherCar->GetCarForwardVector(), GetCarForwardVector());
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "CarDotProduct: " + FString::SanitizeFloat(CarDirections));

			if (1.0f - CarDirections < 0.008f) {

				FVector Dist = GetActorLocation() - OtherCar->GetActorLocation();

				float Yaw = OtherCar->GetActorRotation().Yaw;
				float DistRot = Dist.Rotation().Yaw + 90.0f;

				//Account for 360 degrees of rotation
				if (FMath::Abs(Yaw-DistRot) < 8.0f || 360.0f - FMath::Abs(Yaw-DistRot) < 8.0f) {
					DraftAvailable = true;
				}

			}

			
		}
	}
}


void ACar::AssessDamage() {
	if (Health <= 4) {
		if (!Smoke->IsVisible()) {
			Smoke->ToggleVisibility();
		}
		if (Health <= 2) {
			Smoke->SetWorldScale3D(Smoke->GetComponentScale() * 2);
			if (Health <= 1 && !Fire->IsVisible()) {
				Fire->ToggleVisibility();
			}
		}
	}
}

//OtherCar is car being hit
void ACar::BounceOtherCar(FHitResult Hit, bool HeavyDamage, FVector InverseVector) {
	StoredCurrentPosition = HitBox->GetComponentLocation();
	//StoredCurrentPosition.Z -= HitBox->GetComponentLocation().Z;
	StoredCurrentPosition.Z -= HitBoxDifference;

	DestinationPosition = InverseVector + GetActorLocation();
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "DestinationPosition: " + DestinationPosition.ToString());

	if (Crashed) {
		DestinationPosition.Z = 250.0 + GetActorLocation().Z;

		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
		DetachmentRules.LocationRule = EDetachmentRule::KeepWorld;
		DetachmentRules.RotationRule = EDetachmentRule::KeepWorld;
		DetachmentRules.ScaleRule = EDetachmentRule::KeepWorld;

		CameraComp->DetachFromComponent(DetachmentRules);
		//Car->CameraComp->SetWorldLocation(CollisionPoint);

		StoredCurrentRotation = GetActorRotation();
		DestinationRotation = StoredCurrentRotation;
		//Maybe change value
		DestinationRotation.Roll = StoredCurrentRotation.Roll + -180;

		if (!(Fire->IsVisible())) {
			Fire->ToggleVisibility();
		}

		FVector FireLocation = Fire->GetComponentLocation();
		FireLocation.Z = GetActorLocation().Z;
		Fire->SetWorldLocation(FireLocation);

		if (!(Smoke->IsVisible())) {
			Smoke->ToggleVisibility();
		}

		CrashingEffect->Play();
	}
	else {

		FComponentQueryParams Params = FComponentQueryParams();
		Params.TraceTag = FName("FutureMoveTrace");
		Params.bTraceComplex = true;
		Params.AddIgnoredActor(this);

		Params.AddIgnoredActors(GameMode->MyGameState->AllPowerUps);
		Params.AddIgnoredActors(GameMode->MyGameState->AllDriftPoints);
		Params.AddIgnoredActors(GameMode->MyGameState->AllItemBoxes);
		Params.AddIgnoredActors(GameMode->MyGameState->AllJumpPads);
		for (AActor* Current : GameMode->MyGameState->AllCheckPoints) {
			ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
			Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
		}
		if (GameMode->MyGameState->AllOilSlicks.Num() > 0) {
			for (AOilSlick* Current : GameMode->MyGameState->AllOilSlicks) {
				AActor* ActorSlick = Cast<AActor>(Current);
				Params.AddIgnoredActor(ActorSlick);
			}
		}
		if (GameMode->MyGameState->AllFireTrails.Num() > 0) {
			for (AFireTrail* Current : GameMode->MyGameState->AllFireTrails) {
				AActor* ActorTrail = Cast<AActor>(Current);
				Params.AddIgnoredActor(ActorTrail);
			}
		}

		FVector CorrectHeight = DestinationPosition;
		CorrectHeight.Z = HitBox->GetComponentLocation().Z;

		GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ OutResults, /*class UPrimitiveComponent* PrimComp*/ HitBox, /*const FVector & Start*/ HitBox->GetComponentLocation(),
			/*const FVector & End*/ CorrectHeight, /*const FQuat & Rot*/ GetActorRotation(), /*const FComponentQueryParams & Params*/ Params);

		DrawDebugLine(GetWorld(), StoredCurrentPosition, DestinationPosition, FColor::Orange, false, 100.0f, '\000', 10.0f);

		if (OutResults.Num() > 0) {
			DestinationPosition = OutResults[0].Location;
			DestinationPosition.Z = 0.0;
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "SpinOut is used by othercar");
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "collision: " + UKismetSystemLibrary::GetDisplayName(OutResults[0].Component.Get()));

			//Use Code from Bounce

			FVector CollisionPoint = OutResults[0].Location;
			//CollisionPoint.Z = 0.0;
			CollisionPoint.Z = HitBox->GetComponentLocation().Z - HitBoxDifference;

			FVector InverseVectorNormalized = InverseVector;
			InverseVectorNormalized.Normalize();

			FVector Normal = OutResults[0].ImpactNormal;
			Normal.Z = 0.0;
			//Normal.Z = HitBox->GetComponentLocation().Z - HitBoxDifference;
			Normal.Normalize();

			float LostSpeed = GearToDistanceMapping[Gear] / 5.0;

			FVector Reflected = InverseVectorNormalized - 2 * (FVector::DotProduct(InverseVectorNormalized, Normal) * Normal);
			BouncedMirror = Reflected * LostSpeed + CollisionPoint;
			Reflected.Normalize();

			if (GameMode->DrawCollisionDebugLineCommand) {
				DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + InverseVector * 1000, FColor::Green, false, 100.0f, '\000', 10.0f);
				DrawDebugLine(GetWorld(), CollisionPoint, CollisionPoint + Normal * 1000, FColor::Black, false, 100.0f, '\000', 10.0f);
				DrawDebugLine(GetWorld(), CollisionPoint, CollisionPoint + Reflected * 1000, FColor::Blue, false, 100.0f, '\000', 10.0f);
			}
			

			float PossibleAngle = FMath::RadiansToDegrees(FMath::Atan2(Reflected.Y, Reflected.X) - FMath::Atan2(InverseVectorNormalized.Y, InverseVectorNormalized.X));

			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "PossibleAngle: " + FString::SanitizeFloat(PossibleAngle));


			FRotator SpinRotation = FRotator(0.f, PossibleAngle, 0.f);
			DestinationRotation = SpinRotation;
			StoredCurrentRotation = GetActorRotation();


			AdditionalBounce = true;
		}
	}

	CarToCar = true;
	

	

}


void ACar::Bounce(FHitResult Hit, FVector BouncedInitialVector, bool UsedBouncedInitialVector) {
	LightCollision = true;
	if (Gear >= 3) {
		if (HasCowCatcher) {
			Health -= 1;
		}
		else {
			Health -= 2;
		}
		LightCollision = false;
		
	}
	else {
		Health -= 1;
	}

	AssessDamage();

	//Move to Collision
	FVector CollisionPoint = Hit.Location;
	//CollisionPoint.Z -= HitBox->GetComponentLocation().Z;
	CollisionPoint.Z = HitBox->GetComponentLocation().Z - HitBoxDifference;
	DestinationPosition = CollisionPoint;

	FVector Velocity = GetCarForwardVector();
	if (UsedBouncedInitialVector) {
		Velocity = BouncedInitialVector;
	}
	FVector VelocityNormalized = Velocity;
	VelocityNormalized.Normalize();

	FVector Normal = Hit.ImpactNormal;

	Normal.Z = 0.0;
	Normal.Normalize();


	float LostSpeed = GearToDistanceMapping[Gear]/5.0;
	if (Gear >= 3) {
		Gear = Gear - 2;
	}
	else {
		Gear = 1;
	}
	

	//FVector Reflected = LostSpeed *(-2 * (FVector::DotProduct(Velocity2, Normal) * Normal + Velocity2));
	FVector Reflected = VelocityNormalized - 2*(FVector::DotProduct(VelocityNormalized, Normal) * Normal);
	BouncedMirror = Reflected*LostSpeed + CollisionPoint;
	Reflected.Normalize();

	if (GameMode->DrawCollisionDebugLineCommand) {
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Velocity * 1000, FColor::Green, false, 100.0f, '\000', 10.0f);
		DrawDebugLine(GetWorld(), CollisionPoint, CollisionPoint + Normal * 1000, FColor::Black, false, 100.0f, '\000', 10.0f);
		DrawDebugLine(GetWorld(), CollisionPoint, CollisionPoint + Reflected * 1000, FColor::Blue, false, 100.0f, '\000', 10.0f);
	}
	

	//float AimAtAngle = FMath::RadiansToDegrees(acosf((FVector::DotProduct(VelocityNormalized, Reflected))));

	float PossibleAngle = FMath::RadiansToDegrees( FMath::Atan2(Reflected.Y, Reflected.X) - FMath::Atan2(VelocityNormalized.Y, VelocityNormalized.X));

	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "PossibleAngle: " + FString::SanitizeFloat(PossibleAngle));
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "AimAtAngle: " + FString::SanitizeFloat(AimAtAngle));

	//Additional Spinout Angle
	if (!LightCollision) {
		ACar* OtherCar = Cast<ACar>(Hit.GetActor());
		if (PossibleAngle < 0) {
			if (HasCowCatcher && PowerUpActive || (OtherCar != NULL && OtherCar != nullptr && OtherCar->WeightClass < WeightClass)) {
				PossibleAngle += -3 * Initial_Gear;
			}
			else {
				PossibleAngle += -10 * Initial_Gear;
			}
			
		}
		else
		{
			if (HasCowCatcher && PowerUpActive || (OtherCar != NULL && OtherCar != nullptr && OtherCar->WeightClass < WeightClass)) {
				PossibleAngle += 3 * Initial_Gear;
			}
			else {
				PossibleAngle += 10 * Initial_Gear;
			}
			
		}
	}


	FRotator SpinRotation = FRotator(0.f, PossibleAngle, 0.f);
	DestinationRotation = SpinRotation;
	StoredCurrentRotation = GetActorRotation();
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, SpinRotation.ToString());

	if (Hit.GetActor()->IsA<ACar>()) {
		ACar* OtherCar = Cast<ACar>(Hit.GetActor());
		if (HasCowCatcher && PowerUpActive) {
			OtherCar->Health -= 2;
			//Increased knockback
			LostSpeed = LostSpeed*2.5;
		}
		else {
			OtherCar->Health -= 1;
		}

		if (OtherCar->WeightClass < WeightClass) {
			LostSpeed = LostSpeed * 2.0f;
		}
		else {
			LostSpeed = LostSpeed / 2.0f;
		}
		
		OtherCar->AssessDamage();
		if (OtherCar->Health <= 0) {
			OtherCar->Crashed = true;
		}



		OtherCar->BounceOtherCar(Hit, LightCollision, -Reflected*LostSpeed);
	}


	Bounced = true;


}


void ACar::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor->IsA<AJumpPad>() && !Jumped) {
		JumpPadReference = Cast<AJumpPad>(OtherActor);
		JumpPadReference->BounceNoise->Play();
		if (JumpPadReference->RoundOfLastUse + 2 < GameMode->CurrentRound) {
			JumpPadReference->JumpsCompleted = 1;
		}
		int PointDestination = (JumpPadReference->JumpSpline->GetNumberOfSplinePoints() - JumpPadReference->JumpsCompleted);

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "Point Destination: " + FString::FromInt(PointDestination));
		FVector FutureDestination = JumpPadReference->JumpSpline->GetLocationAtSplinePoint(PointDestination, ESplineCoordinateSpace::World);
		if (JumpPadReference->JumpsCompleted + 1 > GameMode->MyGameState->OutActors.Num()) {
			JumpPadReference->JumpsCompleted = 1;
		}
		else {
			JumpPadReference->JumpsCompleted += 1;
		}

		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, "JumpsCompleted: " + FString::FromInt(JumpPadReference->JumpsCompleted));
		JumpPadReference->RoundOfLastUse = GameMode->CurrentRound;

		if (Bounced) {
			CollisionNoise->Play();
		}

		CarToCar = false;
		Bounced = false;
		SpinOut = false;
		AdditionalBounce = false;
		Moving = false;


		DestinationPosition = FutureDestination;
		FTime = 0.0f;
		Jumped = true;

	}


	else if (!(OtherActor->IsA<ADriftPoint>()) && !(OtherActor->IsA<AOilSlick>()) && !(OtherActor->IsA<AItemBox>()) && !(OtherActor->IsA<ACheckPoint>()) && !(OtherActor->IsA<AFireTrail>())) {
		InWall = true;
	}
}

void ACar::OnHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	InWall = false;
}


//PowerUp value determines which PowerUp is gained
void ACar::StorePowerUp() {
	int PowerUp;
	if (CarRank == 3) {
		PowerUp = FMath::RandRange(0, 1);
	}
	else if (CarRank == 0) {
		PowerUp = FMath::RandRange(2, 3);
	}
	else {
		PowerUp = FMath::RandRange(0, 3);
	}

	if (PowerUp == 0) {
		HasNitro = true;
		PowerUpName = "Nitro";
	}
	else if (PowerUp == 1) {
		HasCowCatcher = true;
		PowerUpName = "Cow Catcher";
	}
	else if (PowerUp == 2) {
		HasOilSlick = true;
		PowerUpName = "Oil Slick";
	}
	else {
		HasFireTires = true;
		PowerUpName = "Fire Tires";
	}
	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "PowerUp: " + PowerUp);
	HasPowerUp = true;
}


void ACar::UsePowerUp() {
	if (HasPowerUp) {
		if (HasCowCatcher) {
			if (!(CowCatcher->IsVisible())) {
				CowCatcher->ToggleVisibility();
			}
			RoundsSincePowerUp = GameMode->CurrentRound;
			PowerUpActive = true;
		}
		else if (HasOilSlick) {
			
			//Called in Blueprint
			SpawnOilSlick();
			HasOilSlick = false;

		}
		else if (HasFireTires) {
			SpawnFireTrail();
			HasFireTires = false;
			UsedFireTires = true;
			if (!TireFire1->IsVisible()) {
				TireFire1->ToggleVisibility();
				TireFire2->ToggleVisibility();
				TireFire3->ToggleVisibility();
				TireFire4->ToggleVisibility();
				TireFire1->SetWorldLocation(Tires[0]->GetComponentLocation());
				TireFire2->SetWorldLocation(Tires[1]->GetComponentLocation());
				TireFire3->SetWorldLocation(Tires[2]->GetComponentLocation());
				TireFire4->SetWorldLocation(Tires[3]->GetComponentLocation());
			}
			
			
		}
		else if (HasNitro) {
			if (Initial_Gear <= 4) {
				Initial_Gear += 2;
			}
			else {
				Initial_Gear = 6;
			}
			Gear = Initial_Gear;
			FVector Update = ComputeFuturePosition(Gear, Mesh->GetRelativeRotation(), GetActorLocation());
			SetFutureMeshColor(Update);
			HasNitro = false;
		}
		HasPowerUp = false;
		PowerUpName = "None";
	}
}


void ACar::SpawnOilSlick()
{
	if (ToSpawn) {

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = this;
		AOilSlick* NewSlick = GetWorld()->SpawnActor<AOilSlick>(ToSpawn, GetActorLocation(), GetActorRotation(), SpawnInfo);
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Oil Slick should spawn");

		GameMode->MyGameState->AllOilSlicks.Add(NewSlick);
		NewSlick->TurnCreated = GameMode->CurrentTurn;

	}
	

}


void ACar::SpawnFireTrail() {


	//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Spawn Fire Trail reached");
	if (ToSpawnFire) {

		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = this;
		AFireTrail* NewTrail = GetWorld()->SpawnActor<AFireTrail>(ToSpawnFire, GetActorLocation(), GetActorRotation(), SpawnInfo);
		//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Fire Trail should spawn");

		GameMode->MyGameState->AllFireTrails.Add(NewTrail);
		NewTrail->TurnCreated = GameMode->CurrentTurn;
		NewTrail->RoundCreated = GameMode->CurrentRound;
		/*NewTrail->Fire1->ToggleVisibility();
		NewTrail->Fire2->ToggleVisibility();
		NewTrail->Fire3->ToggleVisibility();
		NewTrail->Fire4->ToggleVisibility();
		NewTrail->Fire5->ToggleVisibility();
		NewTrail->Fire6->ToggleVisibility();
		if (!(this->CarBody == CarType::BODY_Truck && Gear <= 1)) {
			NewTrail->Fire7->ToggleVisibility();
			NewTrail->Fire8->ToggleVisibility();
		}*/

	}
}


void ACar::CheckRoundsSincePowerUp() {
	if (HasCowCatcher) {
		if (GameMode->CurrentRound - RoundsSincePowerUp >= 3) {
			if (CowCatcher->IsVisible()) {
				CowCatcher->ToggleVisibility();
			}
			PowerUpActive = false;
		}
	}
}


void ACar::TireRotation() 
{
	FComponentQueryParams collisionParams = FComponentQueryParams::DefaultComponentQueryParams;
	TArray<FVector> ImpactPoints;

	//Checks tire location to set rotation
	for (int i = 0; i < 4; i++)
	{
		FHitResult HitDetails;
		GetWorld()->LineTraceSingleByChannel(HitDetails, Tires[i]->GetComponentLocation(), Tires[i]->GetComponentLocation() - FVector(0, 0, 250), ECollisionChannel::ECC_Visibility, collisionParams);
		ImpactPoints.Add(HitDetails.ImpactPoint);

		if (GameMode->DrawTireDebugLineCommand) {
			DrawDebugLine(GetWorld(), Tires[i]->GetComponentLocation(), Tires[i]->GetComponentLocation() - FVector(0, 0, 250), FColor::Orange, true, 20.0f, (uint8)('\000'), 5.0f);
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, HitDetails.ImpactPoint.ToString());
		}
		
	}

	FRotator newRot = FRotator::ZeroRotator;

	//Front Pitch
	float FrontDotProduct = FVector::DotProduct(UKismetMathLibrary::GetDirectionUnitVector(ImpactPoints[0], ImpactPoints[1]), -(GetActorForwardVector()));
	float FrontAngleDegrees = FMath::RadiansToDegrees(acosf(FrontDotProduct));
	if (isnan(FrontAngleDegrees))
	{
		FrontAngleDegrees = 0;
	}
	else {
		FrontAngleDegrees *= FMath::Sign(ImpactPoints[0].Z - ImpactPoints[1].Z);
	}

	//Back Pitch
	float BackDotProduct = FVector::DotProduct(UKismetMathLibrary::GetDirectionUnitVector(ImpactPoints[2], ImpactPoints[3]), -(GetActorForwardVector()));
	float BackAngleDegrees = FMath::RadiansToDegrees(acosf(BackDotProduct));
	if (isnan(BackAngleDegrees))
	{
		BackAngleDegrees = 0;
	}
	else
	{
		BackAngleDegrees *= FMath::Sign(ImpactPoints[2].Z - ImpactPoints[3].Z);
	}

	float PitchAngle = (FrontAngleDegrees / 2) + (BackAngleDegrees / 2);


	//Left Roll
	float LeftDotProduct = FVector::DotProduct(UKismetMathLibrary::GetDirectionUnitVector(ImpactPoints[0], ImpactPoints[2]), GetCarForwardVector() * -1);
	float LeftAngleDegrees = FMath::RadiansToDegrees(acosf(LeftDotProduct));
	if (isnan(LeftAngleDegrees))
	{
		LeftAngleDegrees = 0;
	}
	else {
		LeftAngleDegrees *= FMath::Sign(ImpactPoints[2].Z - ImpactPoints[0].Z);
	}

	//Right Roll
	float RightDotProduct = FVector::DotProduct(UKismetMathLibrary::GetDirectionUnitVector(ImpactPoints[1], ImpactPoints[3]), GetCarForwardVector() * -1);
	float RightAngleDegrees = FMath::RadiansToDegrees(acosf(RightDotProduct));
	if (isnan(RightAngleDegrees))
	{
		RightAngleDegrees = 0;
	}
	else {
		RightAngleDegrees *= FMath::Sign(ImpactPoints[1].Z - ImpactPoints[3].Z);
	}

	float RollAngle = (LeftAngleDegrees / 2) + (RightAngleDegrees / 2);

	newRot.Add(PitchAngle, 0, RollAngle);
	Mesh->SetRelativeRotation(newRot);

}

//Set ParameterValue to 0.0 for Green, 0.5 for Yellow, and 1.0 for Red
void ACar::SetFutureMeshColor(FVector FuturePosition) {


	FutureMesh->SetWorldLocation(FuturePosition);
	float FutureColor = 0.0;
	FRotator ResultRot = /*HitBox->GetComponentRotation()*/ FRotator::ZeroRotator;
	FComponentQueryParams Params = FComponentQueryParams();
	Params.TraceTag = FName("FutureMoveTrace");
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(this);
	FuturePosition.Z = HitBox->GetComponentLocation().Z;

	Params.AddIgnoredActors(GameMode->MyGameState->AllPowerUps);
	Params.AddIgnoredActors(GameMode->MyGameState->AllDriftPoints);
	Params.AddIgnoredActors(GameMode->MyGameState->AllItemBoxes);
	Params.AddIgnoredActors(GameMode->MyGameState->AllJumpPads);
	for (AActor* Current : GameMode->MyGameState->AllCheckPoints) {
		ACheckPoint* CurrentCheck = Cast<ACheckPoint>(Current);
		Params.AddIgnoredComponent(CurrentCheck->CheckPointHitBox);
	}
	if (GameMode->MyGameState->AllOilSlicks.Num() > 0) {
		for (AOilSlick* Current : GameMode->MyGameState->AllOilSlicks) {
			AActor* ActorSlick = Cast<AActor>(Current);
			Params.AddIgnoredActor(ActorSlick);
		}
	}
	if (GameMode->MyGameState->AllFireTrails.Num() > 0) {
		for (AFireTrail* Current : GameMode->MyGameState->AllFireTrails) {
			AActor* ActorTrail = Cast<AActor>(Current);
			Params.AddIgnoredActor(ActorTrail);
		}
	}
	
	


	GetWorld()->ComponentSweepMulti(/*TArray<struct FHitResult> & OutHits*/ OutResults, /*class UPrimitiveComponent* PrimComp*/ HitBox, /*const FVector & Start*/ HitBox->GetComponentLocation(),
		/*const FVector & End*/ FuturePosition, /*const FQuat & Rot*/ ResultRot, /*const FComponentQueryParams & Params*/ Params);


	if (OutResults.Num() > 0) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, UKismetSystemLibrary::GetDisplayName(OutResults[0].Component.Get()));
		if (Health > 2 && Gear != 6 || (Health > 1 && Gear < 2)) {
			FutureColor = 0.5;
		}
		else {
			FutureColor = 1.0;
		}
		PotentialCollision = true;
		if (Gear <= 2 || CowCatcher->IsVisible()) {
			PotentialDamage = 1;
		}
		else {
			PotentialDamage = 2;
		}
	}
	else {
		PotentialCollision = false;
		PotentialDamage = 0;
	}

	if (DraftAvailable && (Initial_Gear + 2 == Gear)) {
		//Add New color to blend with
		DynamicMaterial->SetScalarParameterValue(TEXT("BlendDraft"), .25);
	}
	else {
		DynamicMaterial->SetScalarParameterValue(TEXT("BlendDraft"), 0.0);

	}


	DynamicMaterial->SetScalarParameterValue(TEXT("Blend"), FutureColor);
}



// Called to bind functionality to input
void ACar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ACar::CalculateSplineDistance(FVector ActorPosition) {
	//Need to add param for other vectors

	//MySpline = Cast<ATrack_Spline>(OutActors[0]);

	return MySpline->Spline->FindInputKeyClosestToWorldLocation(/*GetActorLocation()*/ ActorPosition);

}

float ACar::CalculateSplineOrientation(float ActorDist, FVector ActorPosition) {
	//Used by RacerAIController

	FRotator TempRot = ActorPosition.Rotation();
	TempRot.Yaw = MySpline->Spline->FindRotationClosestToWorldLocation(GetActorLocation(), ESplineCoordinateSpace::World).Yaw;
	FVector Temp = TempRot.Vector();
	Temp.Normalize();
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + Temp * 1000, FColor::Yellow, false, 100.0f, '\000', 10.0f);
	//DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + ActorPosition * 1000, FColor::Black, false, 100.0f, '\000', 10.0f);

	return FVector::DotProduct(ActorPosition, Temp);

}

bool ACar::IsHeadToHead(ACar* OtherCar) {
	//See Dot Prodcut Result Reference
	//If dot product is negative, Car1 and Car2 face opposite directions, ie head-on if they collide

	FVector Car1_Direction = GetCarForwardVector();
	FVector Car2_Direction = OtherCar->GetCarForwardVector();

	float DotProductResult = FVector::DotProduct(Car1_Direction, Car2_Direction);
	return (DotProductResult < 0);

}

void ACar::ToggleFutureVisibility() {

	FutureMesh->ToggleVisibility();
}

int ACar::GetGear()
{
	return Gear;

}

int ACar::GetInitialGear()
{
	return Initial_Gear;

}

void ACar::SetGear(int NewGear)
{
	Gear = NewGear;

}


void ACar::Upshift()
{
	if (DraftAvailable) {
		if (Gear <= 5 && (Gear != Initial_Gear + 2)) {
			Gear++;
			FVector FuturePosition = ComputeFuturePosition(Gear, Mesh->GetRelativeRotation(), GetActorLocation());
			//FutureMesh->SetWorldLocation(FuturePosition);
			SetFutureMeshColor(FuturePosition);
		}
	}
	else {
		if (Gear <= 5 && (Gear != Initial_Gear + 1)) {
			Gear++;
			FVector FuturePosition = ComputeFuturePosition(Gear, Mesh->GetRelativeRotation(), GetActorLocation());
			//FutureMesh->SetWorldLocation(FuturePosition);
			SetFutureMeshColor(FuturePosition);
		}
	}
	
}

void ACar::Downshift()
{
	if (Gear != 0 && (Gear != Initial_Gear - 1)) {
		Gear--;
		FVector FuturePosition = ComputeFuturePosition(Gear, Mesh->GetRelativeRotation(), GetActorLocation());
		//FutureMesh->SetWorldLocation(FuturePosition);
		SetFutureMeshColor(FuturePosition);
	}	
}

void ACar::Update_Initial_Gear() {

	Initial_Gear = Gear;
}

FVector ACar::GetCarForwardVector() {

	return GetActorRightVector();
}


FVector ACar::ComputeFuturePosition(int NewGear, FRotator Rot, FVector InitialLocation)
{


	//FVector Result = GetActorLocation();
	FVector Result = InitialLocation;

	//Add Debug Line or Sphere to see if rotation affects this output
	

	// Our car is oriented down the Y Axis,  UE4 treats the Forward vector as the X Axis, therefore our "Forward vector" is actually the Right vector.
	// It would be better to create another function called GetCarFowardVector() so the code was more easily understandable.
	FVector Direction = GetCarForwardVector();	
	//To Rotator, add Rot, convert back to Vector
	FRotator NewRotation = Direction.Rotation() + Rot;
	Direction = NewRotation.Vector();

	Result += Direction * GearToDistanceMapping[NewGear];
	

	return Result;
}




void ACar::RespawnCar() {

	if (Crashed) {

		RepairNoise->Play();

		//Resets Health
		if (Smoke->IsVisible()) {
			Smoke->ToggleVisibility();
			Smoke->SetWorldScale3D(Smoke->GetComponentScale() / 2);
		}
		if (Fire->IsVisible()) {
			Fire->ToggleVisibility();
		}
		Health = MAX_HEALTH;
		

		HasPowerUp = false;
		HasCowCatcher = false;
		if (CowCatcher->IsVisible()) {
			CowCatcher->ToggleVisibility();
		}
		HasNitro = false; 
		PowerUpName = "None";
		

		FVector FireLocation = Fire->GetComponentLocation();

		float NewCarStart = MySpline->Spline->FindInputKeyClosestToWorldLocation(GetActorLocation());
		FVector NewLoc = MySpline->Spline->GetLocationAtSplineInputKey(NewCarStart, ESplineCoordinateSpace::World);
		if (NewCarStart >= 7.0 && NewCarStart < 9.0) {
			NewLoc.Z = 4000.0;
			FireLocation.Z = 4110.0;
		}
		else {
			NewLoc.Z = 0.0;
			FireLocation.Z = 110.0;
		}

		Smoke->SetWorldScale3D(Smoke->GetComponentScale() / 2);
		Fire->SetWorldLocation(FireLocation);
		
		SetActorLocation(NewLoc);
		
		FRotator DriftRotation = MySpline->Spline->GetRotationAtSplineInputKey(NewCarStart, ESplineCoordinateSpace::World);
		DriftRotation.Yaw -= 90.0;

		SetActorRotation(DriftRotation);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, false);
		AttachmentRules.LocationRule = EAttachmentRule::SnapToTarget;
		AttachmentRules.RotationRule = EAttachmentRule::SnapToTarget;
		AttachmentRules.ScaleRule = EAttachmentRule::SnapToTarget;
		CameraComp->AttachToComponent(SpringArm, AttachmentRules);

		
		PotentialCollision = false;
		Crashed = false;

	}
}




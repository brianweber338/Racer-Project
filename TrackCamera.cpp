// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackCamera.h"
#include "Kismet/KismetMathLibrary.h"
#include "Car.h"


// Sets default values
ATrackCamera::ATrackCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Model = CreateDefaultSubobject<UStaticMeshComponent>("Model");
	RootComponent = Model;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(Model);

	ModelCamera = CreateDefaultSubobject<USceneCaptureComponent2D>("ModelCamera");
	ModelCamera->SetupAttachment(SpringArm);

	ActiveCamera = false;

	CarToFollow;
	//PreviousCarToFollow;


}

// Called when the game starts or when spawned
void ATrackCamera::BeginPlay()
{

	Super::BeginPlay();
	
}

// Called every frame
void ATrackCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ActiveCamera) {
		ACar* LocalCarToFollow = Cast<ACar>(CarToFollow);
		FRotator CameraRot = UKismetMathLibrary::FindLookAtRotation(ModelCamera->GetComponentLocation(), LocalCarToFollow->GetActorLocation());
		ModelCamera->SetWorldRotation(CameraRot);
	}


}


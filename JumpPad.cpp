// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpPad.h"

// Sets default values
AJumpPad::AJumpPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	JumpPad = CreateDefaultSubobject<UStaticMeshComponent>("JumpPad");
	RootComponent = JumpPad;

	PadHitBox = CreateDefaultSubobject<UBoxComponent>("PadHitBox");
	PadHitBox->SetupAttachment(JumpPad);

	JumpSpline = CreateDefaultSubobject<USplineComponent>("JumpSpline");
	JumpSpline->SetupAttachment(JumpPad);

	BounceNoise = CreateDefaultSubobject<UAudioComponent>("BounceNoise");

	JumpsCompleted = 1;
	RoundOfLastUse = 0;


}

// Called when the game starts or when spawned
void AJumpPad::BeginPlay()
{
	Super::BeginPlay();

	BounceNoise->Stop();
	
}

// Called every frame
void AJumpPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


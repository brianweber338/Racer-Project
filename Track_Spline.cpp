// Fill out your copyright notice in the Description page of Project Settings.


#include "Car.h"
#include "Track_Spline.h"


// Sets default values
ATrack_Spline::ATrack_Spline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline->SetupAttachment(Root);
	//FinishLine = CreateDefaultSubobject<UBoxComponent>(FName("Finish Line"));
	//FinishLine->OnComponentBeginOverlap.AddDynamic(this, &ATrack_Spline::OnFinishLineBeginOverlap);
	Spline->bDrawDebug = true;

}

// Called when the game starts or when spawned
void ATrack_Spline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrack_Spline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//FinishLine->OnComponentBeginOverlap;

}


//void ATrack_Spline::OnFinishLineBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//
//	ACar* Car = Cast<ACar>(OtherActor);
//	Car->CurrentLap++;
//	//Car->OnFinishLine = true;
//
//
//	//Create variable to prevent cheating at finish line?
//}



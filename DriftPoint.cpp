// Fill out your copyright notice in the Description page of Project Settings.


#include "DriftPoint.h"
#include "Car.h"

// Sets default values
ADriftPoint::ADriftPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	DriftPad = CreateDefaultSubobject<UStaticMeshComponent>("DriftPad");
	RootComponent = DriftPad;

	PadHitBox = CreateDefaultSubobject<UBoxComponent>("PadHitBox");
	PadHitBox->SetupAttachment(DriftPad);

	DriftSpline = CreateDefaultSubobject<USplineComponent>("DriftSpline");
	DriftSpline->SetupAttachment(DriftPad);

	ExtremeTurn = false;
	Loop = false;
	RecentUse = false;
	DistanceOfUse = 0;
	TurnOfUse = 0;

	PadHitBox->OnComponentBeginOverlap.AddDynamic(this, &ADriftPoint::OnPadHitBoxBeginOverlap);
	PadHitBox->OnComponentEndOverlap.AddDynamic(this, &ADriftPoint::OnPadHitBoxEndOverlap);

}

// Called when the game starts or when spawned
void ADriftPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADriftPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADriftPoint::OnPadHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != nullptr) {
		Car->Drifting = true;
		Car->DriftPadReference = this;
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Drifting = true");
	}
	

}

void ADriftPoint::OnPadHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != nullptr) {
		Car->Drifting = false;
		if (!(Car->ActiveDrifting)) {
			Car->DriftPadReference = NULL;
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Drifting = false");
	}
	


}
// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPoint.h"
#include "Car.h"

// Sets default values
ACheckPoint::ACheckPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CheckPointMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckPointMesh");
	RootComponent = CheckPointMesh;

	CheckPointHitBox = CreateDefaultSubobject<UBoxComponent>("CheckPointHitBox");
	CheckPointHitBox->SetupAttachment(CheckPointMesh);

	IsCheckPoint1;
	IsFinishLine;

	//CheckPointHitBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnCheckPointHitBoxBeginOverlap);
	CheckPointHitBox->OnComponentEndOverlap.AddDynamic(this, &ACheckPoint::OnCheckPointHitBoxEndOverlap);

}

// Called when the game starts or when spawned
void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void ACheckPoint::OnCheckPointHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//	
//}

void ACheckPoint::OnCheckPointHitBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != nullptr && Car != NULL) {
		if (IsCheckPoint1) {
			Car->Checkpoint1 = true;
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "CheckPoint1: " + Car->Checkpoint1);
		}
		else {
			Car->Checkpoint2 = true;
			//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "CheckPoint2: " + Car->Checkpoint2);
		}
	}
}
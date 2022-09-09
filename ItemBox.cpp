// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBox.h"
#include "Car.h"

// Sets default values
AItemBox::AItemBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUp = CreateDefaultSubobject<UStaticMeshComponent>("PickUp");
	RootComponent = PickUp;

	PickUp->OnComponentBeginOverlap.AddDynamic(this, &AItemBox::OnPickUpBeginOverlap);
	//PickUp->OnComponentEndOverlap.AddDynamic(this, &AItemBox::OnPickUpEndOverlap);

	YAWPERSECOND = 20.0;

	Inactive = false;
	MessagePlayed = false;

	RoundOfUse;

}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{

	Super::BeginPlay();
	
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{

	FRotator NewRotation = FRotator(0.0, YAWPERSECOND * DeltaTime, 0.0);

	AddActorLocalRotation(NewRotation);

	Super::Tick(DeltaTime);

}

void AItemBox::OnPickUpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != NULL && Car != nullptr){
		if (!(Car->HasPowerUp)) {
			Car->StorePowerUp();
		}
		RoundOfUse = Car->GameMode->CurrentRound;
		Inactive = true;
		PickUp->ToggleVisibility();


		this->SetActorEnableCollision(false);

		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Item Box Overlap");
	}
	
	


}


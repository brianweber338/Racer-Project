// Fill out your copyright notice in the Description page of Project Settings.


#include "RepairItem.h"
#include "Components/AudioComponent.h"
#include "Car.h"

// Sets default values
ARepairItem::ARepairItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickUp = CreateDefaultSubobject<UStaticMeshComponent>("PickUp");
	RootComponent = PickUp;

	PickUp->OnComponentBeginOverlap.AddDynamic(this, &ARepairItem::OnPickUpBeginOverlap);

	YAWPERSECOND = 20.0;

}

// Called when the game starts or when spawned
void ARepairItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARepairItem::Tick(float DeltaTime)
{
	FRotator NewRotation = FRotator(0.0, YAWPERSECOND*DeltaTime, 0.0);

	AddActorLocalRotation(NewRotation);


	Super::Tick(DeltaTime);

}

void ARepairItem::OnPickUpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	ACar* Car = Cast<ACar>(OtherActor);
	
	if (Car->Health < 5) {
		Car->RepairNoise->Play();
		Car->Smoke->ToggleVisibility();
		if (Car->Health <= 2) {
			Car->Smoke->SetWorldScale3D(Car->Smoke->GetComponentScale() / 2);
			if (Car->Fire->IsVisible()) {
				Car->Fire->ToggleVisibility();
			}
		}
		Car->Health = Car->MAX_HEALTH;
		Destroy();
	}
	

	

}
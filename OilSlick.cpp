// Fill out your copyright notice in the Description page of Project Settings.


#include "OilSlick.h"
#include "Components/AudioComponent.h"
#include "FireTrail.h"
#include "Car.h"

// Sets default values
AOilSlick::AOilSlick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	Slick = CreateDefaultSubobject<UStaticMeshComponent>("Slick");
	RootComponent = Slick;

	SlickHitBox = CreateDefaultSubobject<UBoxComponent>("SlickHitBox");
	SlickHitBox->SetupAttachment(Slick);

	Fire = CreateDefaultSubobject<UParticleSystemComponent>("Fire");
	Fire->SetupAttachment(Slick);

	SlickHitBox->OnComponentBeginOverlap.AddDynamic(this, &AOilSlick::OnSlickHitBoxBeginOverlap);
	//SlickHitBox->OnComponentEndOverlap.AddDynamic(this, &AOilSlick::OnPadHitBoxEndOverlap);

	TurnCreated;
}

// Called when the game starts or when spawned
void AOilSlick::BeginPlay()
{
	Super::BeginPlay();


	if (Fire->IsVisible()) {
		Fire->ToggleVisibility();
	}
	
	
}

// Called every frame
void AOilSlick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AOilSlick::OnSlickHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != NULL && Car != nullptr) {
		if (Car->UsedFireTires) {
			if (!Fire->IsVisible()) {
				Fire->ToggleVisibility();
			}
		}
		else if (Car->GameMode->MyGameState->MovementState == 1 && (!(TurnCreated == Car->GameMode->CurrentTurn || TurnCreated == Car->GameMode->CurrentTurn + 1))) {
			Car->OilSlip->Play();

			float RandomRotation = FMath::RandRange((float)30.0, (float)330.0);
			FRotator SlickRotation = FRotator(0.0, RandomRotation, 0.0);
			SlickRotation += Car->GetActorRotation();
			Car->DestinationRotation = SlickRotation;
			Car->StoredCurrentRotation = Car->GetActorRotation();
			


			if (Fire->IsVisible()) {
				Car->Health--;
				Car->AssessDamage();
				if (Car->Health <= 0) {
					Car->DestinationPosition.Z = 250.0 + Car->GetActorLocation().Z;
					Car->Crashed = true;

					FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
					DetachmentRules.LocationRule = EDetachmentRule::KeepWorld;
					DetachmentRules.RotationRule = EDetachmentRule::KeepWorld;
					DetachmentRules.ScaleRule = EDetachmentRule::KeepWorld;

					Car->CameraComp->DetachFromComponent(DetachmentRules);
					//Car->CameraComp->SetWorldLocation(CollisionPoint);

					Car->DestinationRotation.Roll = Car->StoredCurrentRotation.Roll + -180;
					//Car->SpringArmRotation = Car->SpringArm->GetComponentRotation();
					if (!(Car->Fire->IsVisible())) {
						Car->Fire->ToggleVisibility();
					}

					FVector FireLocation = Car->Fire->GetComponentLocation();
					FireLocation.Z = Car->GetActorLocation().Z;
					Car->Fire->SetWorldLocation(FireLocation);

					Car->Smoke->ToggleVisibility();

					Car->CrashingEffect->Play();
				}
			}
			
			Car->OilSlicked = true;
			//Car->SetActorRotation(SlickRotation);


			int Index = Car->GameMode->MyGameState->AllOilSlicks.Find(this);
			Car->GameMode->MyGameState->AllOilSlicks.RemoveAt(Index);
			Destroy();

		}
	}
	else if (OtherActor->IsA<AFireTrail>()) {
		if (!Fire->IsVisible()) {
			Fire->ToggleVisibility();
		}
		
	}

}



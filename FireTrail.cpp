// Fill out your copyright notice in the Description page of Project Settings.


#include "FireTrail.h"
#include "Car.h"

// Sets default values
AFireTrail::AFireTrail()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	TrailRoot = CreateDefaultSubobject<UStaticMeshComponent>("TrailRoot");
	RootComponent = TrailRoot;

	Tread1 = CreateDefaultSubobject<UStaticMeshComponent>("Tread1");
	Tread1->SetupAttachment(RootComponent);

	Tread2 = CreateDefaultSubobject<UStaticMeshComponent>("Tread2");
	Tread2->SetupAttachment(RootComponent);

	


	//Fire Begins not visible
	Fire1 = CreateDefaultSubobject<UParticleSystemComponent>("Fire1");
	Fire2 = CreateDefaultSubobject<UParticleSystemComponent>("Fire2");
	Fire3 = CreateDefaultSubobject<UParticleSystemComponent>("Fire3");
	Fire4 = CreateDefaultSubobject<UParticleSystemComponent>("Fire4");
	Fire5 = CreateDefaultSubobject<UParticleSystemComponent>("Fire5");
	Fire6 = CreateDefaultSubobject<UParticleSystemComponent>("Fire6");
	Fire7 = CreateDefaultSubobject<UParticleSystemComponent>("Fire7");
	Fire8 = CreateDefaultSubobject<UParticleSystemComponent>("Fire8");

	Fire1->SetupAttachment(Tread1);
	Fire2->SetupAttachment(Tread1);
	Fire3->SetupAttachment(Tread1);
	Fire4->SetupAttachment(Tread1);
	Fire5->SetupAttachment(Tread2);
	Fire6->SetupAttachment(Tread2);
	Fire7->SetupAttachment(Tread2);
	Fire8->SetupAttachment(Tread2);


	HitBox1 = CreateDefaultSubobject<UBoxComponent>("HitBox1");
	HitBox2 = CreateDefaultSubobject<UBoxComponent>("HitBox2");
	HitBox3 = CreateDefaultSubobject<UBoxComponent>("HitBox3");
	HitBox4 = CreateDefaultSubobject<UBoxComponent>("HitBox4");
	HitBox5 = CreateDefaultSubobject<UBoxComponent>("HitBox5");
	HitBox6 = CreateDefaultSubobject<UBoxComponent>("HitBox6");
	HitBox7 = CreateDefaultSubobject<UBoxComponent>("HitBox7");
	HitBox8 = CreateDefaultSubobject<UBoxComponent>("HitBox8");

	HitBox1->SetupAttachment(Fire1);
	HitBox2->SetupAttachment(Fire2);
	HitBox3->SetupAttachment(Fire3);
	HitBox4->SetupAttachment(Fire4);
	HitBox5->SetupAttachment(Fire5);
	HitBox6->SetupAttachment(Fire6);
	HitBox7->SetupAttachment(Fire7);
	HitBox8->SetupAttachment(Fire8);

	/*HitBox1->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox2->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox3->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox4->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox5->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox6->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox7->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);
	HitBox8->OnComponentBeginOverlap.AddDynamic(this, &AFireTrail::OnHitBoxBeginOverlap);*/

	HitBox1->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox2->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox3->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox4->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox5->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox6->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox7->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);
	HitBox8->OnComponentEndOverlap.AddDynamic(this, &AFireTrail::OnHitBoxOverlapEnd);



	TurnCreated;
	RoundCreated;

}

// Called when the game starts or when spawned
void AFireTrail::BeginPlay()
{
	Super::BeginPlay();


	
	
}

// Called every frame
void AFireTrail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//void AFireTrail::OnHitBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
//
//	ACar* Car = Cast<ACar>(OtherActor);
//	if (Car != NULL && Car != nullptr) {
//		
//		if (!(Car->GameMode->CurrentTurn == TurnCreated || Car->GameMode->CurrentTurn == TurnCreated + 1)) {
//			UBoxComponent* CurrentHitBox = Cast<UBoxComponent>(OverlappedComponent);
//			UParticleSystemComponent* TempFire = Cast<UParticleSystemComponent>(CurrentHitBox->GetAttachParent());
//			if (TempFire != NULL && TempFire != nullptr) {
//				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Second if statement passed");
//				if (TempFire->IsVisible()) {
//					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "TempFire is toggled");
//					TempFire->ToggleVisibility();
//					Car->Health = Car->Health - 1;
//					Car->AssessDamage();
//					if (Car->Health <= 0) {
//						Car->DestinationPosition.Z = 250.0 + Car->GetActorLocation().Z;
//						Car->Crashed = true;
//
//						FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepRelative, false);
//						DetachmentRules.LocationRule = EDetachmentRule::KeepWorld;
//						DetachmentRules.RotationRule = EDetachmentRule::KeepWorld;
//						DetachmentRules.ScaleRule = EDetachmentRule::KeepWorld;
//
//						Car->CameraComp->DetachFromComponent(DetachmentRules);
//						//Car->CameraComp->SetWorldLocation(CollisionPoint);
//
//						Car->StoredCurrentRotation = Car->GetActorRotation();
//						Car->DestinationRotation = Car->StoredCurrentRotation;
//						Car->DestinationRotation.Roll = Car->StoredCurrentRotation.Roll + -180;
//						//Car->SpringArmRotation = Car->SpringArm->GetComponentRotation();
//						if (!(Car->Fire->IsVisible())) {
//							Car->Fire->ToggleVisibility();
//						}
//
//						FVector FireLocation = Car->Fire->GetComponentLocation();
//						FireLocation.Z = Car->GetActorLocation().Z;
//						Car->Fire->SetWorldLocation(FireLocation);
//
//						Car->Smoke->ToggleVisibility();
//
//						Car->CrashingEffect->Play();
//					}
//
//				}
//			}
//		}
//		else {
//			if (Car->UsedFireTires) {
//				UBoxComponent* CurrentHitBox = Cast<UBoxComponent>(OverlappedComponent);
//				UParticleSystemComponent* TempFire = Cast<UParticleSystemComponent>(CurrentHitBox->GetAttachParent());
//				if (TempFire != NULL && TempFire != nullptr) {
//					if (!TempFire->IsVisible()) {
//						TempFire->ToggleVisibility();
//					}
//				}
//			}
//		}
//		
//	}
//
//
//
//}

void AFireTrail::OnHitBoxOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	ACar* Car = Cast<ACar>(OtherActor);
	if (Car != NULL && Car != nullptr) {

		if (!(Car->GameMode->CurrentTurn == TurnCreated || Car->GameMode->CurrentTurn == TurnCreated + 1)) {
			UBoxComponent* CurrentHitBox = Cast<UBoxComponent>(OverlappedComp);
			UParticleSystemComponent* TempFire = Cast<UParticleSystemComponent>(CurrentHitBox->GetAttachParent());
			if (TempFire != NULL && TempFire != nullptr) {
				//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "Second if statement passed");
				if (TempFire->IsVisible()) {
					//GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, "TempFire is toggled");
					TempFire->ToggleVisibility();
					Car->Health = Car->Health - 1;
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

						Car->StoredCurrentRotation = Car->GetActorRotation();
						Car->DestinationRotation = Car->StoredCurrentRotation;
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
			}
		}
		else {
			if (Car->UsedFireTires) {
				UBoxComponent* CurrentHitBox = Cast<UBoxComponent>(OverlappedComp);
				UParticleSystemComponent* TempFire = Cast<UParticleSystemComponent>(CurrentHitBox->GetAttachParent());
				if (TempFire != NULL && TempFire != nullptr) {
					if (!TempFire->IsVisible()) {
						TempFire->ToggleVisibility();
					}
				}
			}
		}

	}
}

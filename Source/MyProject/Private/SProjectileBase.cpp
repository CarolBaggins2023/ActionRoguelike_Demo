// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectileBase.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASProjectileBase::ASProjectileBase()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentHit.AddDynamic(this, &ASProjectileBase::OnActorHit);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);

	MoveComp = CreateDefaultSubobject<UProjectileMovementComponent>("MoveComp");
	MoveComp->InitialSpeed = 10000.0f;
	MoveComp->ProjectileGravityScale = 0.0f;
	MoveComp->bRotationFollowsVelocity = true;
	MoveComp->bInitialVelocityInLocalSpace = true;
}

// Called when the game starts or when spawned
void ASProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->IgnoreActorWhenMoving(GetInstigator(), true);
}

void ASProjectileBase::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit) {
	// Note that we don't call Explode_Implementation, because that is just how we implement Explode in C++,
	// we may still implement Explode in Blueprint.
	Explode();
}

void ASProjectileBase::Explode_Implementation() {
	if (!ensure(IsValid(this))) {
		return;
	}

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
	Destroy();

	APlayerController *PC = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	PC->ClientStartCameraShake(CameraShake);
}

void ASProjectileBase::PostInitializeComponents() {
	Super::PostInitializeComponents();
}

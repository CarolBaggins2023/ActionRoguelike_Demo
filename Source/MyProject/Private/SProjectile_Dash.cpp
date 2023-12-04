// Fill out your copyright notice in the Description page of Project Settings.


#include "SProjectile_Dash.h"

#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


ASProjectile_Dash::ASProjectile_Dash() {
	TeleportDelay = 0.2f;
	ExplodeDelay = 0.2f;
	
	MoveComp->InitialSpeed = 6000.0f;
}

void ASProjectile_Dash::BeginPlay() {
	Super::BeginPlay();

	// we trigger ASProjectile_Dash::Explode not ASProjectile_Dash::Explode_Implementation,
	// because ASProjectile_Dash::Explode can also be implemented in Blueprint
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedExplode, this, &ASProjectile_Dash::Explode, ExplodeDelay);
}

void ASProjectile_Dash::Explode_Implementation() {
	GetWorldTimerManager().ClearTimer(TimerHandle_DelayedExplode);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MoveComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_DelayedTeleport;
	GetWorldTimerManager().SetTimer(TimerHandle_DelayedTeleport, this, &ASProjectile_Dash::TeleportInstigator,
		TeleportDelay);
}

void ASProjectile_Dash::TeleportInstigator() {
	AActor *ActorToTeleport = GetInstigator();
	if (ensure(ActorToTeleport)) {
		// TODO: we change TeleportTo() to SetActorLocation(), it can be seen as a difficulty
		ActorToTeleport->SetActorLocation(GetActorLocation());
		//ActorToTeleport->TeleportTo(GetActorLocation(), ActorToTeleport->GetActorRotation(), false, false);
	}
}

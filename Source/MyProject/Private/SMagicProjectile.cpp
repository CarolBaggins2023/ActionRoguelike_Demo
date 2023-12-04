// Fill out your copyright notice in the Description page of Project Settings.


#include "SMagicProjectile.h"

#include "SGameplayFunctionLibrary.h"

ASMagicProjectile::ASMagicProjectile() {
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
	
	MoveComp->InitialSpeed = 3000.0f;

	Damage = 50.0f;

	InitialLifeSpan = 10.0f;
}

void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!IsValid(OtherActor) || OtherActor == GetInstigator()) {
		return;
	}

	if (USGameplayFunctionLibrary::ApplyDirectDamage(GetInstigator(), OtherActor, Damage, SweepResult)) {
		Explode();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"

#include "SAttributeComponent.h"
#include "SPlayerState.h"

// Sets default values
ASHealthPotion::ASHealthPotion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	
 	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(RootComponent);

	HealthAmount = 50.0f;
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	if (!ensure(InstigatorPawn)) {
		return;
	}

	USAttributeComponent *AttributeComp = Cast<USAttributeComponent>(
		InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if (ensure(AttributeComp) && AttributeComp->IsInjured()) {
		ASPlayerState *PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if (ensure(PlayerState)) {
			AttributeComp->ApplyHealthChanged(this, HealthAmount);
			Destroy();
		}
	}
	
}

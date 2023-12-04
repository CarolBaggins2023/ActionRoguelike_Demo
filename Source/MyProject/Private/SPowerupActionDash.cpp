// Fill out your copyright notice in the Description page of Project Settings.


#include "SPowerupActionDash.h"

#include "SAction.h"
#include "SAttributeComponent.h"

// Sets default values
ASPowerupActionDash::ASPowerupActionDash()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;
	
	
	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(RootComponent);
}

void ASPowerupActionDash::Interact_Implementation(APawn* InstigatorPawn) {
	if (!ensure(InstigatorPawn && DashActionClass)) {
		return;
	}

	USActionComponent *ActionComp = Cast<USActionComponent>(InstigatorPawn->GetComponentByClass(
		USActionComponent::StaticClass()));
	if (ActionComp) {
		if (!ActionComp->GetAction(DashActionClass)) {
			ActionComp->AddAction(InstigatorPawn, DashActionClass);
		}
		Destroy();
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "SAttributeComponent.h"

#include "SGameModeBase.h"

// Sets default values for this component's properties
USAttributeComponent::USAttributeComponent()
{	
	HealthMax = 100.0f;
	Health = HealthMax;
}

bool USAttributeComponent::ApplyHealthChanged(AActor* InstigatorActor, float Delta) {
	if (Delta < 0.0f && !GetOwner()->CanBeDamaged()) {
		FString Msg = FString::Printf(TEXT("%s can't be damaged."), *GetNameSafe(GetOwner()));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, Msg);

		return false;
	}

	float OldHealth = Health;
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	Health = NewHealth;
	if (ActualDelta != 0) {
		OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, ActualDelta);
	}

	if (ActualDelta < 0.0f && Health <= 0.0f) {
		ASGameModeBase *GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if (GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor) {
	if (FromActor) {
		return Cast<USAttributeComponent>(FromActor->GetComponentByClass(StaticClass()));
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* FromActor) {
	USAttributeComponent *AttributeComp = GetAttributes(FromActor);
	if (AttributeComp) {
		return AttributeComp->IsAlive();
	}
	return false;
}

bool USAttributeComponent::IsInjured() {
	return Health < HealthMax;
}

bool USAttributeComponent::IsAlive() {
	return Health > 0.0f;
}

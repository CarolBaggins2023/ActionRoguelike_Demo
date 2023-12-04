// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameplayFunctionLibrary.h"

#include "SAttributeComponent.h"

bool USGameplayFunctionLibrary::ApplyDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount) {
	if (DamageCauser == TargetActor) {
		return false;
	}

	USAttributeComponent *AttributeComp = USAttributeComponent::GetAttributes(TargetActor);
	if (AttributeComp) {
		return AttributeComp->ApplyHealthChanged(DamageCauser, -DamageAmount);
	}
	return false;
}

bool USGameplayFunctionLibrary::ApplyDirectDamage(AActor* DamageCauser, AActor* TargetActor, float DamageAmount,
	const FHitResult& HitResult) {
	if (ApplyDamage(DamageCauser, TargetActor, DamageAmount)) {
		UPrimitiveComponent *HitComp = HitResult.GetComponent();
		if (HitComp && HitComp->IsSimulatingPhysics(HitResult.BoneName)) {
			HitComp->AddImpulseAtLocation(-HitResult.ImpactNormal * 100000.0f, HitResult.ImpactPoint, HitResult.BoneName);
		}
		return true;
	}
	return false;
}



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAttributeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "STargetDummy.generated.h"

UCLASS()
class MYPROJECT_API ASTargetDummy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASTargetDummy();

protected:

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere)
	USphereComponent *SphereComp;

	UPROPERTY(VisibleAnywhere)
	USAttributeComponent *AttributeComp;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
		float Delta);
};

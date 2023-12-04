// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SHealthPotion.generated.h"

UCLASS()
class MYPROJECT_API ASHealthPotion : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASHealthPotion();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *SphereComp;

	UPROPERTY(VisibleAnywhere,  BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(EditAnywhere)
	float HealthAmount;
};

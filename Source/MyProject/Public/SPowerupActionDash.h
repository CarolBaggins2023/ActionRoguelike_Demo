// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SPowerupActionDash.generated.h"

class USAction;

UCLASS()
class MYPROJECT_API ASPowerupActionDash : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerupActionDash();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent *SphereComp;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USAction> DashActionClass;
};

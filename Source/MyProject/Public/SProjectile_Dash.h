// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SProjectile_Dash.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASProjectile_Dash : public ASProjectileBase
{
	GENERATED_BODY()

public:

	ASProjectile_Dash();

protected:

	virtual void BeginPlay() override;
	
	virtual void Explode_Implementation() override;

	void TeleportInstigator();
	
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float ExplodeDelay;

	FTimerHandle TimerHandle_DelayedExplode;
};

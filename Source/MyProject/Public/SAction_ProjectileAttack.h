// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SAction.h"
#include "SCharacter.h"
#include "SAction_ProjectileAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

public:

	USAction_ProjectileAttack();

	virtual bool CanStart_Implementation(AActor* InstigatorActor) override;
	
	virtual void StartAction_Implementation(AActor* NewInstigator) override;

	virtual void StopAction_Implementation(AActor* NewInstigator) override;

protected:

	UFUNCTION()
	void AttackDelay_TimeElapsed(ASCharacter *InstigatorCharacter);

	UFUNCTION()
	void CoolDown_TimeElapsed();

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay;

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, Category = "Effect")
	FName HandSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	UParticleSystem *CastingEffect;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage *AttackAnim;
	
	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	float CoolDownTime;

	UPROPERTY(BlueprintReadOnly, Category = "Attack")
	bool bIsActive;

	UPROPERTY(EditDefaultsOnly, Category = "Attack")
	TSubclassOf<UCameraShakeBase> CoolDownCameraShake;
};

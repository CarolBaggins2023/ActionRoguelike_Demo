// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SInteractionComponent.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class MYPROJECT_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

	void MoveForward(float Val);

	void MoveRight(float Val);

	void SprintStart();

	void SprintStop();

	void PrimaryAttack();

	void DashAttack();

	void PrimaryInteract();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;

protected:

	UFUNCTION(Exec)
	bool HealSelf(float Amount = 100.0f);
	
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
		float Delta);
	
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent *SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent *CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent *AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actions")
	USActionComponent *ActionComp;

	UPROPERTY(VisibleAnywhere)
	USInteractionComponent *InteractionComp;
};

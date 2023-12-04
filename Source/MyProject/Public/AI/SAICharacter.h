// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SActionComponent.h"
#include "SAttributeComponent.h"
#include "SWorldUserWidget.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "SAICharacter.generated.h"

UCLASS()
class MYPROJECT_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASAICharacter();

	UFUNCTION(BlueprintCallable)
	bool IsAlive();
	
protected:

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp,
		float NewHealth, float Delta);

	UFUNCTION()
	void SetTargetActor(AActor *TargetActor);

	UFUNCTION()
	AActor* GetTargetActor();

	UFUNCTION()
	void OnPawnSeen(APawn *Pawn);
	
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent *PawnSensingComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent *AttributeComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent *ActionComp;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HealthBarWidgetClass;
	
	USWorldUserWidget *ActiveHealthBar;
};

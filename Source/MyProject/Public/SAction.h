// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class MYPROJECT_API USAction : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor *InstigatorActor);

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning();

	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor *NewInstigator);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Action")
	void StopAction(AActor *NewInstigator);

	virtual UWorld* GetWorld() const override;

	void Initialize(USActionComponent *NewActionComp);

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName;
	
protected:

	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags;

	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockTags;

	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor *Instigator;

	UPROPERTY()
	USActionComponent *ActionComp;

	UPROPERTY(BlueprintReadOnly)
	float TimeStarted;
};

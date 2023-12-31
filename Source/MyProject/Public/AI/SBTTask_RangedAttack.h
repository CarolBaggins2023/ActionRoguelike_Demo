// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:

	USBTTask_RangedAttack();

protected:

	UPROPERTY(EditAnywhere, Category = "Attack")
	FName MuzzleName;

	UPROPERTY(EditAnywhere, Category = "Attack")
	UClass *ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxBulletSpread;
};

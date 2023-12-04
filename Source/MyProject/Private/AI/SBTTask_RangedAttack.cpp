// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"


USBTTask_RangedAttack::USBTTask_RangedAttack() {
	MaxBulletSpread = 5.0f;
}

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	AAIController *AIController = OwnerComp.GetAIOwner();
	if (!ensure(AIController)) {
		return EBTNodeResult::Failed;
	}
	
	ACharacter *AIPawn = Cast<ACharacter>(AIController->GetPawn());
	if (!IsValid(AIPawn)) {
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent *BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BlackboardComp)) {
		return EBTNodeResult::Failed;
	}

	AActor *TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
	if (!IsValid(TargetActor) || !USAttributeComponent::IsActorAlive(TargetActor)) {
		return EBTNodeResult::Failed;
	}

	FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation(MuzzleName);
	FVector Direction = TargetActor->GetActorLocation() - AIPawn->GetActorLocation();
	FRotator MuzzleRotation = Direction.Rotation();

	MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
	MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = AIPawn;

	AActor *NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
}

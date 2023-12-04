// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "NavigationSystemTypes.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ASAICharacter::ASAICharacter() {
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	ActionComp = CreateDefaultSubobject<USActionComponent>("ActionComp");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetMesh()->SetGenerateOverlapEvents(true);
}

bool ASAICharacter::IsAlive() {
	return USAttributeComponent::IsActorAlive(this);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
                                    float Delta) {
	if (Delta < 0.0f) {
		if (InstigatorActor != this) {
			SetTargetActor(InstigatorActor);
		}

		if (ActiveHealthBar == nullptr) {
			ActiveHealthBar = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (ActiveHealthBar) {
				ActiveHealthBar->AttachedActor = this;
				ActiveHealthBar->AddToViewport();
			}
		}
	}

	if (!USAttributeComponent::IsActorAlive(this)) {
		ASAIController *AIController = Cast<ASAIController>(GetController());
		if (AIController) {
			AIController->GetBrainComponent()->StopLogic("Killed");
		}

		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		SetLifeSpan(5.0f);
	}
}

void ASAICharacter::SetTargetActor(AActor* TargetActor) {
	AAIController *AIController = Cast<AAIController>(GetController());
	if (AIController) {
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}

AActor* ASAICharacter::GetTargetActor() {
	AAIController *AIController = Cast<AAIController>(GetController());
	if (AIController) {
		UBlackboardComponent *BlackboardComp = AIController->GetBlackboardComponent();
		if (BlackboardComp) {
			return Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		}
	}
	return nullptr;
}

void ASAICharacter::OnPawnSeen(APawn* Pawn) {
	if (GetTargetActor() == Pawn) {
		return;
	}

	SetTargetActor(Pawn);
}

void ASAICharacter::PostInitializeComponents() {
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

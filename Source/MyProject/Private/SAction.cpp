// Fill out your copyright notice in the Description page of Project Settings.


#include "SAction.h"

bool USAction::CanStart_Implementation(AActor* InstigatorActor) {
	if (IsRunning()) {
		return false;
	}

	AActor *Actor = Cast<AActor>(GetOuter());
	USActionComponent *Comp = Cast<USActionComponent>(Actor->GetComponentByClass(USActionComponent::StaticClass()));
	if (Comp->ActiveGameplayTags.HasAny(BlockTags)) {
		return false;
	}

	return true;
}

bool USAction::IsRunning() {
	return bIsRunning;
}

void USAction::StartAction_Implementation(AActor* NewInstigator) {
	UE_LOG(LogTemp, Log, TEXT("Start action %s."), *ActionName.ToString());

	USActionComponent *Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
	Instigator = NewInstigator;

	TimeStarted = GetWorld()->GetTimeSeconds();
	
	GetOwningComponent()->OnActionStarted.Broadcast(GetOwningComponent(), this);
}

void USAction::StopAction_Implementation(AActor* NewInstigator) {
	UE_LOG(LogTemp, Log, TEXT("Stop action %s."), *ActionName.ToString());

	USActionComponent *Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
	Instigator = NewInstigator;
}

UWorld* USAction::GetWorld() const {
	AActor *Actor = Cast<AActor>(GetOuter());
	if (Actor) {
		return Actor->GetWorld();
	}

	return nullptr;
}

void USAction::Initialize(USActionComponent* NewActionComp) {
	ActionComp = NewActionComp;
}

USActionComponent* USAction::GetOwningComponent() const {
	return ActionComp;
}

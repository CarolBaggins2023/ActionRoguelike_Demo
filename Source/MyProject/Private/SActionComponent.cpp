// Fill out your copyright notice in the Description page of Project Settings.


#include "SActionComponent.h"

#include "SAction.h"

// Sets default values for this component's properties
USActionComponent::USActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for (TSubclassOf<USAction> ActionClass : DefaultActions) {
		AddAction(GetOwner(), ActionClass);
	}
}

void USActionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	TArray<USAction*> ActionsCopy = Actions;
	for (USAction *Action : ActionsCopy) {
		if (Action && Action->IsRunning()) {
			Action->StopAction(GetOwner());
		}
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USActionComponent::AddAction(AActor* Instigator, TSubclassOf<USAction> ActionClass) {
	if (!ensure(ActionClass)) {
		return;
	}

	USAction *NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if (ensure(NewAction)) {
	    NewAction->Initialize(this);
	
		Actions.Add(NewAction);
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove) {
	if (!ensure(ActionToRemove && !ActionToRemove->IsRunning())) {
		return;
	}

	Actions.Remove(ActionToRemove);
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName) {
	for (USAction *Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (!Action->CanStart(Instigator)) {
				FString FailedMsg = FString::Printf(TEXT("Failed to run: %s"), *ActionName.ToString());
				// GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}

			Action->StartAction(Instigator);
			return true;
		}
	}

	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName) {
	for (USAction *Action : Actions) {
		if (Action && Action->ActionName == ActionName) {
			if (Action->IsRunning()) {
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	
	return false;
}

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass) {
	for (USAction *Action : Actions) {
		if (Action->IsA(ActionClass)) {
			return Action;
		}
	}
	return nullptr;
}


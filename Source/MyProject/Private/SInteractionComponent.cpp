// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "EntitySystem/MovieSceneComponentDebug.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceDistance = 500.0f;
	TraceRadius = 30.0f;
	CollisionChannel = ECC_WorldDynamic;
}

void USInteractionComponent::PrimaryInteract() {
	if (FocusActor == nullptr) {
		return;
	}

	APawn *MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusActor, MyPawn);
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn *MyPawn = Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled()) {
		FindBestInteractable();
	}
}

void USInteractionComponent::FindBestInteractable() {
	AActor *MyOwner = GetOwner();
	FVector EyesLocation;
	FRotator EyesRotation;
	MyOwner->GetActorEyesViewPoint(EyesLocation, EyesRotation);
	FVector Start = EyesLocation;
	FVector End = EyesLocation + EyesRotation.Vector() * TraceDistance;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(CollisionChannel);

	TArray<FHitResult> Hits;
	float Radius = TraceRadius;
	FCollisionShape Shape;
	Shape.SetSphere(Radius);
	bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, Start, End, FQuat::Identity, ObjectQueryParams, Shape);
	FColor LineColor = bBlockingHit ? FColor::Green : FColor::Red;
	
	FocusActor = nullptr;
	for (FHitResult Hit : Hits) {
		AActor *HitActor = Hit.GetActor();
		if (HitActor && HitActor->Implements<USGameplayInterface>()) {
			// DrawDebugSphere(GetWorld(), Hit.ImpactPoint, Radius, 32, LineColor, false, 0.0f);
			FocusActor = HitActor;
			break;
		}
	}

	// DrawDebugLine(GetWorld(), Start, End, LineColor, false, 2.0f, 0, 0.0f);
}


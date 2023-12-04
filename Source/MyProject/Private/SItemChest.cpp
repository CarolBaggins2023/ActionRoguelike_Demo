// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"

#include "SPlayerState.h"

// Sets default values
ASItemChest::ASItemChest()
{
 	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 100.0f;

	bLidOpened = false;

	CreditsAmount = 100;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	bLidOpened = !bLidOpened;
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator(CurrentPitch, 0.0f, 0.0f));

	ASPlayerState *PlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if (PlayerState) {
		PlayerState->AddCredits(CreditsAmount);
	}

	SetLifeSpan(5.0f);
}

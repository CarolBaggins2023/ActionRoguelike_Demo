// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::TogglePauseMenu() {
	if (PauseMenuInstance && PauseMenuInstance->IsInViewport()) {
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;
		

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());

		UGameplayStatics::SetGamePaused(this, false);
		
		return;
	}

	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if (PauseMenuInstance) {
		UGameplayStatics::SetGamePaused(this, true);
		
		PauseMenuInstance->AddToViewport(100);
		
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}

void ASPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASPlayerController::TogglePauseMenu);
}

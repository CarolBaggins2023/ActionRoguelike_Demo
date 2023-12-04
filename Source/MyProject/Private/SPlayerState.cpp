// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const {
	return Credits;
}

void ASPlayerState::AddCredits(int32 Delta) {
	if (!ensure(Delta >= 0.0f)) {
		return;
	}

	Credits += Delta;

	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

FString ASPlayerState::GetSavedPlayerName() {
	return SavedPlayerName;
}

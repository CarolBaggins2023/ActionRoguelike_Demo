// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable)
	FString GetSavedPlayerName();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Credits")
	int32 Credits;

	// TODO: we tried to save player name across levels but failed, it can be one difficulty
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FString SavedPlayerName;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FRankData {
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	FString PlayerName;

	UPROPERTY(BlueprintReadOnly)
	int32 Credit;

	bool operator<(const FRankData &Rhs) const {
		return Credit < Rhs.Credit;
	}

	bool operator<=(const FRankData &Rhs) const {
		return Credit <= Rhs.Credit;
	}

	bool operator>(const FRankData &Rhs) const {
		return !(*this <= Rhs);
	}

	bool operator>=(const FRankData &Rhs) const {
		return !(*this < Rhs);
	}
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API USSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
	TArray<FRankData> SavedData;
};

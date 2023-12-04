// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SSaveGame.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASGameModeBase();

	virtual void StartPlay() override;
	
	virtual void OnActorKilled(AActor *VictimActor, AActor *Killer);

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

protected:

	UFUNCTION()
	void SpawnBotTimerElapsed();
	
	UFUNCTION()
	void OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper *QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void SpawnPowerupTimerElapsed();
	
	UFUNCTION()
	void OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper *QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(BlueprintNativeEvent)
	void GameEndTimerElapsed();
	
	UPROPERTY(EditAnywhere, Category = "PlayerState")
	int32 CreditsKill;

	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float BotSpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat *DifficultyCurve;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery *SpawnBotQuery;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<AActor> MinionClass;

	FTimerHandle TimerHandle_SpawnPowerups;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float PowerupSpawnTimerInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	TArray<TSubclassOf<AActor>> PowerupClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	UEnvQuery *SpawnPowerupQuery;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	int32 DesiredPowerupCount;

	UPROPERTY(EditDefaultsOnly, Category = "Powerup")
	float RequiredPowerupDistance;

	FTimerHandle TimerHandle_EndGame;

	UPROPERTY(EditDefaultsOnly, Category = "EndGame")
	float GameDuration;

	FTimerHandle TimerHandle_PlayerDeadEndGame;

	UPROPERTY(EditDefaultsOnly, Category = "EndGame")
	float PlayerDeadEndGameDelay;

	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly)
	USSaveGame *CurrentSaveGame;
};

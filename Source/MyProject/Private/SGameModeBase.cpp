// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"

ASGameModeBase::ASGameModeBase() {
	CreditsKill = 10;

	GameDuration = 0.0f;

	PlayerDeadEndGameDelay = 5.0f;
	
	SaveSlotName = "SaveGame01";
}

void ASGameModeBase::StartPlay() {
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed,
		BotSpawnTimerInterval, true);

	if (PowerupClasses.Num() > 0) {
		GetWorldTimerManager().SetTimer(TimerHandle_SpawnPowerups, this, &ASGameModeBase::SpawnPowerupTimerElapsed,
			PowerupSpawnTimerInterval, true);
	}

	if (GameDuration > 0.0f) {
		GetWorldTimerManager().SetTimer(TimerHandle_EndGame, this, &ASGameModeBase::GameEndTimerElapsed,
			GameDuration, false);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
	// if VictimActor does not belong to ASCharacter or its children, Player will be nullptr
	ASCharacter *Player = Cast<ASCharacter>(VictimActor);
	if (Player) {
		GetWorldTimerManager().SetTimer(TimerHandle_PlayerDeadEndGame, this, &ASGameModeBase::GameEndTimerElapsed,
			PlayerDeadEndGameDelay, false);
	}

	APawn *KillerPawn = Cast<APawn>(Killer);
	if (KillerPawn) {
		ASPlayerState *PlayerState = KillerPawn->GetPlayerState<ASPlayerState>();
		if (PlayerState) {
			PlayerState->AddCredits(CreditsKill);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: VictimActor: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) {
	Super::InitGame(MapName, Options, ErrorMessage);

	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	if (SelectedSaveSlot.Len() > 0) {
		SaveSlotName = SelectedSaveSlot;
	}

	// TODO: we need to put it in InitGame, it can be one difficulty
	if (UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) {
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	} else {
		CurrentSaveGame = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	}
}

void ASGameModeBase::SpawnBotTimerElapsed() {
	int32 MaxBotCount = 0;
	if (IsValid(DifficultyCurve)) {
		MaxBotCount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}

	int32 NumOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> Iter(GetWorld()); Iter; ++Iter) {
		ASAICharacter *Bot = *Iter;
		USAttributeComponent *AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp && Bot->IsAlive()) {
			++NumOfAliveBots;
		}
	}
	UE_LOG(LogTemp, Log, TEXT("Find %i alive bots."), NumOfAliveBots);

	if (NumOfAliveBots >= MaxBotCount) {
		UE_LOG(LogTemp, Log, TEXT("At maximum bot capacity."))
		return;
	}

	UEnvQueryInstanceBlueprintWrapper *QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery,
		this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnBotSpawnQueryCompleted);
	}
}

void ASGameModeBase::SpawnPowerupTimerElapsed() {
	UEnvQueryInstanceBlueprintWrapper *QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerupQuery,
		this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnPowerupSpawnQueryCompleted);
	}
}

void ASGameModeBase::OnBotSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
                                              EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (!Locations.IsValidIndex(0)) {
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0], FRotator::ZeroRotator, SpawnParams);
	// DrawDebugSphere(GetWorld(), Locations[0], 30.0f, 32, FColor::Green, false, 5.0f);
}

void ASGameModeBase::OnPowerupSpawnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus) {
	if (QueryStatus != EEnvQueryStatus::Success) {
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	TArray<FVector> UsedLocations;
	int32 SpawnCounter = 0;
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0) {
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num() - 1);
		FVector PickedLocation = Locations[RandomLocationIndex];
		Locations.RemoveAt(RandomLocationIndex);

		bool bIsValidLocation = true;
		for (FVector OtherLocation : UsedLocations) {
			float DistanceTo = (PickedLocation - OtherLocation).Size();
			if (DistanceTo < RequiredPowerupDistance) {
				bIsValidLocation = false;
				break;
			}
		}

		if (!bIsValidLocation) {
			continue;
		}

		int32 RandomClassIndex = FMath::RandRange(0, PowerupClasses.Num() - 1);
		TSubclassOf<AActor> RandomPowerupClass = PowerupClasses[RandomClassIndex];
		GetWorld()->SpawnActor<AActor>(RandomPowerupClass, PickedLocation, FRotator::ZeroRotator);
		// DrawDebugSphere(GetWorld(), PickedLocation, 30.0f, 32, FColor::Blue, false, 5.0f);

		UsedLocations.Add(PickedLocation);
		++SpawnCounter;
	}
}

void ASGameModeBase::GameEndTimerElapsed_Implementation() {
	ASPlayerState *PS = Cast<ASPlayerState>(GameState->PlayerArray[0]);
	
	FRankData Data;
	Data.Credit = PS->GetCredits();
	Data.PlayerName = PS->GetSavedPlayerName();
	
	CurrentSaveGame->SavedData.Add(Data);

	CurrentSaveGame->SavedData.StableSort([](const FRankData &A, const FRankData &B) {
		return A > B;
	});
	
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlotName, 0);
}


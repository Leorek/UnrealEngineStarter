// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "IngameDefaultGameMode.generated.h"


UENUM(BlueprintType)
enum class EGameState : uint8
{
	PreGame 	 UMETA(DisplayName = "Pregame"),
	InGame       UMETA(DisplayName = "InGame"),
	PostGame     UMETA(DisplayName = "PostGame")
};

UENUM(BlueprintType)
enum class EGameLoopState : uint8
{
	ShowPowerUps 	    UMETA(DisplayName = "Show PowerUps"),
	ActivatePowerUps    UMETA(DisplayName = "Activate PowerUps"),
	DeactivatePowerUps  UMETA(DisplayName = "Deactivate PowerUps")
};
/**
 * 
 */
UCLASS()
class GAMEJAM_API AIngameDefaultGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite,EditAnywhere,Category="Players")
	int32 NumberOfPlayers = 2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Players")
	TArray<AActor*> PlayerSpawns = TArray<AActor*>();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Players")
	TArray<APawn*> Players = TArray<APawn*>();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Round Settings")
	int32 TimeOfInit = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Round Settings")
	int32 TimeToShowPowerUps = 6;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Round Settings")
	int32 TimeToActivatePowerUps = 5;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Round Settings")
	int32 TimeToDeactivatePowerUps = 5;
protected:

	UPROPERTY(BlueprintReadWrite, Category = "Timers")
	float TimeGameStart = 0.0f;
	UFUNCTION(BlueprintCallable, Category = "Timers")
	float ActualTimeGame() const;

	UPROPERTY(BlueprintReadWrite, Category = "Game State")
	EGameState ActualGameState = EGameState::PreGame;
	UPROPERTY(BlueprintReadWrite, Category = "Game State")
	EGameLoopState ActualGameLoopState = EGameLoopState::DeactivatePowerUps;

	UPROPERTY(BlueprintReadWrite, Category = "Timers")
	FTimerHandle InitGameTimerHandle;

	// Game Loop
	UPROPERTY(BlueprintReadWrite, Category = "Timers")
	FTimerHandle ShowPowerUpsTimerHandle;
	UPROPERTY(BlueprintReadWrite, Category = "Timers")
	FTimerHandle ActivatePowerUpsTimerHandle;
	UPROPERTY(BlueprintReadWrite, Category = "Timers")
	FTimerHandle DeactivatePowerUpsTimerHandle;

};

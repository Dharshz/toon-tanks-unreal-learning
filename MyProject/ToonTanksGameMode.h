// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ToonTanksGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AToonTanksGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void ActorDied(AActor* DeadActor);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void StartGame();

	UFUNCTION(BlueprintImplementableEvent)
	void EndGame(bool bWonGame);

private:
	class ATank* Tank;

	float StartDelay = 3.0f;

	class AToonTanksPlayerController* ToonTanksPlayerController;

	void HandleGameStart();

	int32 GetTargetTowerCount();

	int32 TargetTowers;
	
};

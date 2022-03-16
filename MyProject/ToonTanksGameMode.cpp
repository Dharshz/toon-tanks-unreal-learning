// Fill out your copyright notice in the Description page of Project Settings.


#include "ToonTanksGameMode.h"
#include "Kismet/GamePlayStatics.h"
#include "TimerManager.h"
#include "Tank.h"
#include "Tower.h"
#include "ToonTanksPlayerController.h"

void AToonTanksGameMode::ActorDied(AActor* DeadActor) 
{
	if(DeadActor == Tank)
	{
		Tank->HandleDestruction();
		if (ToonTanksPlayerController) 
		{
			ToonTanksPlayerController->SetPlayerEnabledState(false);
		}
		EndGame(false);
	} 
	else if(ATower* DestroyedTower = Cast<ATower>(DeadActor))
	{
		DestroyedTower->HandleDestruction();
		--TargetTowers;
		UE_LOG(LogTemp, Warning, TEXT("Towers: %i"), TargetTowers);
		if(TargetTowers == 0)
		{
			EndGame(true);
		}
	}
}


void AToonTanksGameMode::BeginPlay() 
{
	Super::BeginPlay();
	HandleGameStart();
}

void AToonTanksGameMode::HandleGameStart()
{
	TargetTowers = GetTargetTowerCount();
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));
	ToonTanksPlayerController = Cast<AToonTanksPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	StartGame();

	if(ToonTanksPlayerController) {

		ToonTanksPlayerController->SetPlayerEnabledState(false);

		FTimerHandle PlayerStartHandle;
		FTimerDelegate tdelegate = FTimerDelegate::CreateUObject(
			ToonTanksPlayerController,
			&AToonTanksPlayerController::SetPlayerEnabledState,
			true);

		GetWorldTimerManager().SetTimer(PlayerStartHandle, StartDelay, false);

	}

}

int32 AToonTanksGameMode::GetTargetTowerCount()
{
	TArray<AActor*> Towers;
	UGameplayStatics::GetAllActorsOfClass(this, ATower::StaticClass(), Towers);
	return Towers.Num();
}
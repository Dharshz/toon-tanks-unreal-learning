// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "ProjectTile.h"
#include "Particles/ParticleSystem.h"

// Sets default values
ATower::ATower()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TowerCap = CreateDefaultSubobject<UCapsuleComponent>(TEXT("TowerCap"));
	RootComponent = TowerCap;

	TowerBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerBase"));
	TowerBase->SetupAttachment(TowerCap);

	TowerTurret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerTurret"));
	TowerTurret->SetupAttachment(TowerBase);

	TowerSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Tower Spawn"));
	TowerSpawnPoint->SetupAttachment(TowerTurret);
}

// Called when the game starts or when spawned
void ATower::BeginPlay()
{
	Super::BeginPlay();
	
	Tank = Cast<ATank>(UGameplayStatics::GetPlayerPawn(this, 0));

	GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

// Called every frame
void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		if (Distance <= 500.0)
		{
			FRotator LookAtRotation = FRotator(0.f, Tank->GetActorLocation().Rotation().Yaw, 0.f);
			TowerTurret->SetWorldRotation(LookAtRotation);
		}
	}
}

// Called to bind functionality to input
void ATower::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATower::CheckFireCondition()
{
	if (Tank)
	{
		float Distance = FVector::Dist(GetActorLocation(), Tank->GetActorLocation());

		if (Distance <= 700.0)
		{
			Fire();
		}
	}
}

void ATower::Fire()
{
	FVector Location = TowerSpawnPoint->GetComponentLocation();
	FRotator Rotation = TowerSpawnPoint->GetComponentRotation();

	GetWorld()->SpawnActor<AProjectTile>(ProjectTileClass, Location, Rotation);
}

void ATower::HandleDestruction()
{
	if(DeathParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DeathParticles, GetActorLocation(), GetActorRotation());
	}
	Destroy();
}


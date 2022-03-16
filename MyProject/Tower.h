// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tower.generated.h"


UCLASS()
class MYPROJECT_API ATower : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATower();

private:
	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* TowerCap;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TowerBase;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TowerTurret;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* TowerSpawnPoint;

	class ATank* Tank;

	FTimerHandle FireRateTimerHandle;

	float FireRate = 2;

	void CheckFireCondition();

	void Fire();

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectTile> ProjectTileClass;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* DeathParticles;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void HandleDestruction();

};

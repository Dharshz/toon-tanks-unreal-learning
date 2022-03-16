// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

UCLASS()
class MYPROJECT_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATank();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UCapsuleComponent* CapsuleComp;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TankMesh;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TankTurret;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SpawnPoint;

	UPROPERTY(EditAnywhere)
	float Speed = 200;

	UPROPERTY(EditAnywhere)
	float TurnRate = 20;

	APlayerController* PlayerControllerRef;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectTile> ProjectTileClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(float offset);

	void Turn(float radius);

	void Fire();

	void HandleDestruction();

};

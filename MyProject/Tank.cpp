// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GamePlayStatics.h"
#include "DrawDebugHelpers.h"
#include "ProjectTile.h"


// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("capsule"));
	RootComponent = CapsuleComp;

	TankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TankMesh"));
	TankMesh->SetupAttachment(CapsuleComp);

	TankTurret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Turret"));
	TankTurret->SetupAttachment(TankMesh);

	SpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Tank Spawn"));
	SpawnPoint->SetupAttachment(TankTurret);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(CapsuleComp);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	
}

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		PlayerControllerRef->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility,
			false,
			HitResult
		);

		/*DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			25.f,
			12,
			FColor::Red,
			true,
			10.f
		); */

		FVector ToTarget = HitResult.ImpactPoint - TankTurret->GetComponentLocation();
		FRotator LookAtRotation = FRotator(0.f, ToTarget.Rotation().Yaw, 0.f);
		TankTurret->SetWorldRotation(LookAtRotation);
	}

}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Move(float offset)
{
	FVector offsetVector = FVector::ZeroVector;
	offsetVector.X = offset * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalOffset(offsetVector, true);
}


void ATank::Turn(float radius) 
{
	FRotator RotateLocation = FRotator::ZeroRotator;
	RotateLocation.Yaw = radius * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);
	AddActorLocalRotation(RotateLocation, true);
}

void ATank::Fire()
{
	 /* DrawDebugSphere(
		GetWorld(),
		SpawnPoint -> GetComponentLocation(),
		25.f,
		12,
		FColor::Red,
		true,
		10.f
	); */

	FVector Location = SpawnPoint->GetComponentLocation();
	FRotator Rotation = SpawnPoint->GetComponentRotation();

	auto ProjectTile = GetWorld()->SpawnActor<AProjectTile>(ProjectTileClass, Location, Rotation);
	ProjectTile->SetOwner(this);
}

void ATank::HandleDestruction()
{
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

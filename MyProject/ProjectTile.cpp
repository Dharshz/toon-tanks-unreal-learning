// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectTile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AProjectTile::AProjectTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	ProjectTileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	RootComponent = ProjectTileMesh;

	ProjectTileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Bullet Movement"));
	ProjectTileMovementComp->MaxSpeed = 1900.f;
	ProjectTileMovementComp->InitialSpeed = 1700.f;
	InitialLifeSpan = 3;

	TrailParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trails"));
	TrailParticle->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AProjectTile::BeginPlay()
{
	Super::BeginPlay();
	
	ProjectTileMesh->OnComponentHit.AddDynamic(this, &AProjectTile::OnHit);
}

// Called every frame
void AProjectTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectTile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr) 
	{
		Destroy();
		return;
	}

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		auto DamageType = UDamageType::StaticClass();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwner->GetInstigatorController(), this, DamageType);
		if(HitParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, GetActorLocation(), GetActorRotation());
		}
		
		Destroy();
	}
}


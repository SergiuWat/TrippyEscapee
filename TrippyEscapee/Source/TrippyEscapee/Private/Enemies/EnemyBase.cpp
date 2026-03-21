// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));

	PawnSensing->SightRadius = 600.0f;
	PawnSensing->SetPeripheralVisionAngle(80.0f);
	PawnSensing->SensingInterval = 0.1f;
	PawnSensing->bOnlySensePlayers = true;

	bHasSeenPlayer = false;
	MinDistance = 200.0f;
	MaxDistance = 400.0f;
	MoveSpeed = 300.0f;
	BulletSpeed = 600.f;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(GetRootComponent());

	FireRate = 1.0f;


	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (PawnSensing)
	{
		PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBase::OnSeePawn);
	}
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bHasSeenPlayer || !PlayerCharacter)
	{
		GetCharacterMovement()->StopMovementImmediately();
		StopFiring();
		return;
	}

	FVector MyLocation = GetActorLocation();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();

	float Distance = FVector::Dist(PlayerLocation, MyLocation);

	FVector Direction = (PlayerLocation - MyLocation).GetSafeNormal();

	Direction.Y = 0.0f;
	Direction.Z = 0.0f;
	Direction.Normalize();

	if (Distance > MinDistance)
	{
		AddMovementInput(Direction, 1.0f);
	}
	else
	{
		GetCharacterMovement()->StopMovementImmediately();
	}

	if (Distance <= MaxDistance)
	{
		StartFiring();
	}
	else
	{
		StopFiring();
	}

	float DeltaX = PlayerLocation.X - MyLocation.X;

	if (DeltaX > 0)
	{
		SetActorScale3D(FVector(-1, 1, 1));   // right
	}
	else
	{
		SetActorScale3D(FVector(1, 1, 1));  // left
	}
}

void AEnemyBase::StartFiring()
{
	if (!GetWorldTimerManager().IsTimerActive(FireTimer))
	{
		GetWorldTimerManager().SetTimer(FireTimer, this, &AEnemyBase::Fire, FireRate, true);
	}
}

void AEnemyBase::StopFiring()
{
	GetWorldTimerManager().ClearTimer(FireTimer);
}

void AEnemyBase::Fire()
{
	if (!PlayerCharacter || !BulletClass) return;

	FVector SpawnLocation = MuzzlePoint->GetComponentLocation();


	FVector Direction = (PlayerCharacter->GetActorLocation() - SpawnLocation).GetSafeNormal();

	FRotator Rotation = Direction.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;

	if (BulletClass)
	{
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, Rotation, SpawnParams);
		if (Bullet)
		{
			Bullet->GetProjectileMovement()->Velocity = Direction * 2000.f;
			Bullet->GetProjectileMovement()->InitialSpeed = BulletSpeed;
			Bullet->GetProjectileMovement()->MaxSpeed = BulletSpeed;
			Bullet->GetBoxCollision()->IgnoreActorWhenMoving(this, true);
		}
	}
}

void AEnemyBase::OnSeePawn(APawn* Pawn)
{
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSeePawn triggered but Pawn is NULL"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy saw pawn: %s"), *Pawn->GetName());

	bHasSeenPlayer = true;
}



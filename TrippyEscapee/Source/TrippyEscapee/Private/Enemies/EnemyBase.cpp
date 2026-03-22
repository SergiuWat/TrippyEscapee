// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyBase.h"
#include "Perception/PawnSensingComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Pickups/BaseStamp.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"

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
	MaxDistance = 600.0f;
	MoveSpeed = 300.0f;
	BulletSpeed = 600.f;

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Y);

	FireRate = 1.0f;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


	HandPivot = CreateDefaultSubobject<USceneComponent>(TEXT("HandPivot"));
	HandPivot->SetupAttachment(RootComponent);


	HandSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("HandSprite"));
	HandSprite->SetupAttachment(HandPivot);


	MuzzlePoint = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzlePoint"));
	MuzzlePoint->SetupAttachment(HandSprite);

}




void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (PlayerCharacter)
	{
		PlayerCharacter->OnPlayerUpsideDown.AddDynamic(this, &AEnemyBase::OnUpsideDownActivate);
		PlayerCharacter->OnPlayerUpsideDownStampFinished.AddDynamic(this, &AEnemyBase::OnUpsideDownDezactivate);
	}
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBase::ReceiveDamage);
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
		//GetCharacterMovement()->StopMovementImmediately();
		StopFiring();
		return;
	}
	if (bIsStunned) return;
	UpdateHandRotation();
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

	//if (Distance <= MaxDistance)
	//{
	//	StartFiring();
	//}
	//else
	//{
	//	StopFiring();
	//}
	StartFiring();

	float DeltaX = PlayerLocation.X - MyLocation.X;

	if (DeltaX > 0)
	{
		SetActorScale3D(FVector(-1, 1, 1));   // right
	/*	FVector PivotLocation = HandPivot->GetComponentLocation();
		PivotLocation.Y = -PivotLocation.Y;*/

	}
	else
	{
		SetActorScale3D(FVector(1, 1, 1));  // left
		/*FVector PivotLocation = HandPivot->GetComponentLocation();
		PivotLocation.Y = PivotLocation.Y;*/

	}

	if (bIsUpsideDown)
	{

		if (GetCharacterMovement()->IsMovingOnGround())
		{
			GetCharacterMovement()->SetMovementMode(MOVE_Falling);
		}
	}
	//UpdatePivot(DeltaX);
	UpdateHandRotation();
}

void AEnemyBase::UpdatePivot(float DirectionX)
{
	float X = (DirectionX > 0) ? -38.f : 38.f;
	float Y = (DirectionX > 0) ? -0.01f : 0.01f;

	if (bIsUpsideDown)
	{
		X *= -1.f;
		Y *= -1.f;
	}

	HandPivot->SetRelativeLocation(FVector(X, Y, 0.f));
}

void AEnemyBase::OnUpsideDownActivate()
{
	bIsUpsideDown = true;


	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 180.f));


	GetCharacterMovement()->GravityScale = -1.f;


	GetCharacterMovement()->Velocity.Z = 0.f;
}

void AEnemyBase::OnUpsideDownDezactivate()
{
	bIsUpsideDown = false;

	SetActorRotation(FRotator(GetActorRotation().Pitch, GetActorRotation().Yaw, 0.f));

	GetCharacterMovement()->GravityScale = 1.f;

	GetCharacterMovement()->Velocity.Z = 0.f;
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
			UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
			Bullet->GetProjectileMovement()->Velocity = Direction * 2000.f;
			Bullet->GetProjectileMovement()->InitialSpeed = BulletSpeed;
			Bullet->GetProjectileMovement()->MaxSpeed = BulletSpeed;
			Bullet->GetBoxCollision()->IgnoreActorWhenMoving(this, true);
		}
	}
}

void AEnemyBase::OnSeePawn(APawn* Pawn)
{
	if (bHasSeenPlayer) return;
	if (!Pawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSeePawn triggered but Pawn is NULL"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Enemy saw pawn: %s"), *Pawn->GetName());

	bHasSeenPlayer = true;
	HandSprite->SetRelativeScale3D(FVector(-0.5f, 1.f, 0.5f));
}

void AEnemyBase::ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCause)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	UE_LOG(LogTemp, Warning, TEXT("Takes Damage"));
	if (HitFlipbook)
	{
		UE_LOG(LogTemp, Warning, TEXT("InFlipBook"));
		bIsStunned = true;
		GetCharacterMovement()->StopMovementImmediately();

		bIsHitAnimationFinished = false;
		float Duration = HitFlipbook->GetTotalDuration();
		GetWorld()->GetTimerManager().SetTimer(HitAnimationTimerHandle, this, &AEnemyBase::OnHitAnimationFinished, Duration, false);
	}
	// Update HUD Here
	UE_LOG(LogTemp, Warning, TEXT("Enemy took damage: %f, Current Health: %f"), Damage, Health);
	if (Health <= 0.f)
	{
		TSubclassOf<ABaseStamp> StampToSpawn = GetRandomStamp();
		if (StampToSpawn)
		{
			GetWorld()->SpawnActor<ABaseStamp>(
				StampToSpawn,
				GetActorLocation(),
				FRotator::ZeroRotator
			);
		}
		Destroy();
	}
}

TSubclassOf<ABaseStamp> AEnemyBase::GetRandomStamp()
{
	float TotalWeight = 0.f;

	for (const FStampDrop& Drop : StampDrops)
	{
		TotalWeight += Drop.DropChance;
	}

	if (TotalWeight <= 0.f) return nullptr;

	float RandomValue = FMath::FRandRange(0.f, TotalWeight);

	float RunningSum = 0.f;

	for (const FStampDrop& Drop : StampDrops)
	{
		RunningSum += Drop.DropChance;

		if (RandomValue <= RunningSum)
		{
			return Drop.StampClass;
		}
	}

	return nullptr;
}


void AEnemyBase::UpdateHandRotation()
{
	if (!PlayerCharacter || !HandPivot || !MuzzlePoint) return;

	FVector Start = HandPivot->GetComponentLocation();
	FVector Target = PlayerCharacter->GetActorLocation();

	FVector Direction = (Target - Start).GetSafeNormal();

	Direction.Y = 0.0f;
	Direction.Normalize();

	FRotator NewRotation = Direction.Rotation();
	NewRotation *= -1.f;
	NewRotation.Yaw = 0.f;
	NewRotation.Roll = 0.f;

	float DeltaX = Target.X - Start.X;

	if (DeltaX > 0)
	{
		//HandSprite->SetRelativeScale3D(FVector(0.5f, 1.f, 0.5f)); // Right

		NewRotation *= -1.f;
	}
	else
	{
		//HandSprite->SetRelativeScale3D(FVector(-0.5f, 1.f, 0.5f)); // Left


	}

	if (bIsUpsideDown)
	{
		//NewRotation.Pitch *= -1.f;
	}
	HandPivot->SetWorldRotation(NewRotation);
}

void AEnemyBase::OnHitAnimationFinished()
{
	UE_LOG(LogTemp, Warning, TEXT("HitAnimationFinished"));
	bIsHitAnimationFinished = true;
	bIsStunned = false;
}


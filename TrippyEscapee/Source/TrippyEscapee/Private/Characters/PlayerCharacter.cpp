// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Weapon/Bullet.h"
#include "Controllers/PlayerCharacterController.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "PaperFlipbook.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#include "DrawDebugHelpers.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bUsePawnControlRotation = false;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	HandPivot = CreateDefaultSubobject<USceneComponent>(TEXT("HandPivot"));
	HandPivot->SetupAttachment(RootComponent);


	HandSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("HandSprite"));
	HandSprite->SetupAttachment(HandPivot);


	SpawnBulletPosition = CreateDefaultSubobject<USceneComponent>(TEXT("SpawnBulletPosition"));
	SpawnBulletPosition->SetupAttachment(HandSprite);
	
}


void APlayerCharacter::SetCheckpoint(FVector NewLocation)
{
	LastCheckpointLocation = NewLocation;

	// save stats
	SavedHealth = Health;

	UE_LOG(LogTemp, Warning, TEXT("Checkpoint saved at: %s"), *NewLocation.ToString());
}

void APlayerCharacter::Respawn()
{
	if (LastCheckpointLocation.IsZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("No checkpoint set!"));
		return;
	}

	SetActorLocation(LastCheckpointLocation);
	Health = SavedHealth;

	UE_LOG(LogTemp, Warning, TEXT("Respawned at checkpoint"));
}


void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(GetController()) : PlayerCharacterController;

	OnTakeAnyDamage.AddDynamic(this, &APlayerCharacter::ReceiveDamage);
	if (PlayerCharacterController)
	{
		PlayerCharacterController->SetShowMouseCursor(true);
	}
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHandRotation();
	FVector Velocity = GetVelocity();
	Velocity.Y = 0.f;
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	if (Speed == 0)
	{
		FVector PivotLocation = HandPivot->GetRelativeLocation();
		PivotLocation.Y = -0.01f;
		PivotLocation.X = -38.0f;
		HandPivot->SetRelativeLocation(PivotLocation);
	}
	if (bTookDamage)
	{
		FlickerTimer -= DeltaTime;
		if (FlickerTimer <= 0.f)
		{
			FlickerTimer = FlickerDuration;
			GetSprite()->SetVisibility(true);
			HandSprite->SetVisibility(true);
		}
		else
		{
			GetSprite()->SetVisibility(false);
			HandSprite->SetVisibility(false);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerCharacter::Shoot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		if (MovementVector.X < 0)
		{
			FVector PivotLocation = HandPivot->GetRelativeLocation();
			PivotLocation.X = 38.f;
			PivotLocation.Y = 0.01f;
			HandPivot->SetRelativeLocation(PivotLocation);
		}
		else {
			FVector PivotLocation = HandPivot->GetRelativeLocation();
			PivotLocation.X = -38.f;
			PivotLocation.Y = -0.01f;
			HandPivot->SetRelativeLocation(PivotLocation);
		}
		AddMovementInput(ForwardDirection, MovementVector.X);
	}
}

void APlayerCharacter::Shoot(const FInputActionValue& Value)
{
	if (!SpawnBulletPosition) return;

	FVector StartPos = SpawnBulletPosition->GetComponentLocation();	
	FVector ForwardVector = SpawnBulletPosition->GetForwardVector();

	FVector MouseWorldPos;
	FVector MouseWorldDir;

	if (PlayerCharacterController->DeprojectMousePositionToWorld(MouseWorldPos, MouseWorldDir))
	{
		float T = (StartPos.Y - MouseWorldPos.Y) / MouseWorldDir.Y;
		FVector Target = MouseWorldPos + MouseWorldDir * T;

		FVector Direction = (Target - StartPos).GetSafeNormal();
		FRotator SpawnRotation = Direction.Rotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = this;

		if (BulletClass)
		{
			ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, StartPos, SpawnRotation, SpawnParams);
			if (Bullet)
			{
				UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
				Bullet->GetProjectileMovement()->Velocity = Direction * 2000.f;
				Bullet->GetBoxCollision()->IgnoreActorWhenMoving(this, true);
			}
		}
	}
}

void APlayerCharacter::SetConfusedControls(bool bIsConfused)
{
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			if (bIsConfused)
			{
				GetWorld()->GetTimerManager().SetTimer(ConfusedTimerHandle, this, &APlayerCharacter::ConfusedTimerFinished, ConfusedDuration, false);
				Subsystem->RemoveMappingContext(DefaultMappingContext);
				Subsystem->AddMappingContext(ConfusedMappingContext, 0);
			}
			else
			{
				Subsystem->RemoveMappingContext(ConfusedMappingContext);
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}
}

void APlayerCharacter::ConfusedTimerFinished()
{
	SetConfusedControls(false);
	bIsStampActive = false;
}

void APlayerCharacter::SetReverseDamage(bool bIsReverseDamage)
{
	bIsReverseDamageActive = bIsReverseDamage;
	bIsStampActive = true;
	GetWorld()->GetTimerManager().SetTimer(ReverseDamageTimerHandle, this, &APlayerCharacter::ReverseDamageTimerFinished, ReverseDamageDuration, false);
}

void APlayerCharacter::ReverseDamageTimerFinished()
{
	bIsReverseDamageActive = false;
	bIsStampActive = false;
}

void APlayerCharacter::OnHitAnimationFinished()
{
	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(GetController()) : PlayerCharacterController;
	if (PlayerCharacterController)
	{
		EnableInput(PlayerCharacterController);
	}
	bIsHitAnimationFinished = true;
}

void APlayerCharacter::TookDamageTimerFinished()
{
	bTookDamage = false;
	GetSprite()->SetVisibility(true);
	HandSprite->SetVisibility(true);
}

void APlayerCharacter::ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, AController* InstigatorController, AActor* DamageCause)
{
	if (bTookDamage) return;

	PlayerCharacterController = PlayerCharacterController == nullptr ? Cast<APlayerCharacterController>(GetController()) : PlayerCharacterController;
	if (PlayerCharacterController)
	{
		DisableInput(PlayerCharacterController);
	}

	if (HitFlipbook)
	{
		bIsHitAnimationFinished = false;
		float Duration = HitFlipbook->GetTotalDuration();
		GetWorld()->GetTimerManager().SetTimer(HitAnimationTimerHandle, this, &APlayerCharacter::OnHitAnimationFinished, Duration, false);
	}

	bTookDamage = true;
	GetWorld()->GetTimerManager().SetTimer(DamageTimerHandle, this, &APlayerCharacter::TookDamageTimerFinished, DamageCooldown, false);
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
	// Update HUD Here
	UE_LOG(LogTemp, Warning, TEXT("Player took damage: %f, Current Health: %f"), Damage, Health);
	if (Health <= 0.f)
	{
		Respawn();
	}
}


void APlayerCharacter::UpdateHandRotation()
{
	FVector MouseWorldPos;
	FVector MouseWorldDir;

	if (PlayerCharacterController->DeprojectMousePositionToWorld(MouseWorldPos, MouseWorldDir))
	{
		FVector StartPos = HandPivot->GetComponentLocation();

		float T = (StartPos.Y - MouseWorldPos.Y) / MouseWorldDir.Y;
		FVector Target = MouseWorldPos + MouseWorldDir * T;

		FVector Direction = (Target - StartPos).GetSafeNormal();

		FRotator NewRotation = Direction.Rotation();

		NewRotation.Yaw = 0.f;
		NewRotation.Roll = 0.f;

		if (Target.X < GetActorLocation().X)
		{

			HandSprite->SetRelativeScale3D(FVector(-0.5f, 1.f, 0.5f));
			NewRotation *= -1.f;
		}
		else
		{
			HandSprite->SetRelativeScale3D(FVector(0.5f, 1.f, 0.5f));
		}
		HandPivot->SetWorldRotation(NewRotation);
	}
}
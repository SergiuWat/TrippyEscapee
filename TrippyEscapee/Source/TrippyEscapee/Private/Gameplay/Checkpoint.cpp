// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"

ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = false;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	RootComponent = Collision;

	Collision->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	Collision->SetCollisionProfileName(TEXT("Trigger"));

	bIsActivated = false;

	GetRenderComponent()->SetupAttachment(RootComponent);
	GetRenderComponent()->SetRelativeLocation(FVector::ZeroVector);
}

void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);

	if (RiseFlipbook)
	{
		GetRenderComponent()->SetFlipbook(RiseFlipbook);
		GetRenderComponent()->Stop();
	}
}

void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsActivated) return;
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

	if (Player)
	{
		bIsActivated = true;

		if (RiseFlipbook)
		{
			GetRenderComponent()->SetFlipbook(RiseFlipbook);
			GetRenderComponent()->PlayFromStart();

			float Duration = RiseFlipbook->GetTotalDuration();

			GetWorldTimerManager().SetTimer(FlipbookTimer, this, &ACheckpoint::OnRiseFinished, Duration, false);
		}
		FVector CheckpointLocation = GetActorLocation();
		CheckpointLocation.Z += 150.f;
		Player->SetCheckpoint(CheckpointLocation);

		UE_LOG(LogTemp, Warning, TEXT("Checkpoint activated!"));
	}
}


void ACheckpoint::OnRiseFinished()
{
	if (IdleFlipbook)
	{
		GetRenderComponent()->SetFlipbook(IdleFlipbook);
		GetRenderComponent()->Play();
	}
}
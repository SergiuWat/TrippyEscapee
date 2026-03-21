// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/BaseStamp.h"
#include "Components/SphereComponent.h"
#include "PaperSpriteComponent.h"
#include "Characters/PlayerCharacter.h"

ABaseStamp::ABaseStamp()
{
	PrimaryActorTick.bCanEverTick = false;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetSphereRadius(50.f);
	SphereCollision->SetCollisionProfileName(TEXT("Trigger"));

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->SetupAttachment(RootComponent);
	Sprite->SetRelativeLocation(FVector(0, 0, 0));

	GetRenderComponent()->SetRelativeLocation(FVector::ZeroVector);
}

void ABaseStamp::BeginPlay()
{
	Super::BeginPlay();

	if (SphereCollision)
	{
		SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseStamp::OnOverlapBegin);
	}
}

void ABaseStamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
	if (Player)
	{

	}

}

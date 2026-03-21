// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Bullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "PaperSpriteComponent.h"
#include "../TrippyEscapee.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/EnemyBase.h"

ABullet::ABullet()
{
    PrimaryActorTick.bCanEverTick = false;

    BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = BoxCollision;
    BoxCollision->SetMobility(EComponentMobility::Movable);
    BoxCollision->SetCollisionResponseToChannel(ECC_PlayerCharacter, ECollisionResponse::ECR_Overlap);
    BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	GetRenderComponent()->SetupAttachment(RootComponent);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->InitialSpeed = 0;
    ProjectileMovement->MaxSpeed = 0;
    ProjectileMovement->ProjectileGravityScale = 0.0f;
    ProjectileMovement->bConstrainToPlane = true;
    ProjectileMovement->SetPlaneConstraintNormal(FVector(0, 1, 0)); 
}
void ABullet::BeginPlay()
{
	Super::BeginPlay();
    if (GetOwner()) 
    {
		BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);
    }
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	//GetRenderComponent()->SetMobility(EComponentMobility::Movable);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Take damage or what ever
	APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    if (Player)
    {
		//Deal damage here
		UGameplayStatics::ApplyDamage(Player, 20.f, GetInstigatorController(), this, UDamageType::StaticClass());
        Destroy();
    }
	AEnemyBase* Enemy = Cast<AEnemyBase>(OtherActor);
    if (Enemy)
    {
		UGameplayStatics::ApplyDamage(Enemy, 20.f, GetInstigatorController(), this, UDamageType::StaticClass());
		Destroy();
    }

}

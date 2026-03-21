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
    BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxCollision->SetCollisionObjectType(ECC_WorldDynamic);
    BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
    BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
        BoxCollision->MoveIgnoreActors.Add(GetOwner());
		BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);
    }
    PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlapBegin);
	//GetRenderComponent()->SetMobility(EComponentMobility::Movable);
}

void ABullet::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Take damage or what ever
    UE_LOG(LogTemp, Warning, TEXT("Overlap"));
    if (!OtherActor || OtherActor == GetOwner()) return;

    bool bReverse = PlayerCharacter && PlayerCharacter->GetIsReverseDamageActive();

    AActor* DamageTarget = bReverse ? GetOwner() : OtherActor;

    if (DamageTarget)
    {
        UGameplayStatics::ApplyDamage(
            DamageTarget,
            20.f,
            GetInstigatorController(),
            this,
            UDamageType::StaticClass()
            );
    }
    UE_LOG(LogTemp, Warning, TEXT("DESTROYED"));
    Destroy();

}

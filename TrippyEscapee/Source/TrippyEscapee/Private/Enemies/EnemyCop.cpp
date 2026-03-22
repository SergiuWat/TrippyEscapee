// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EnemyCop.h"
#include "Enemies/EnemyCop.h"
#include "Weapon/Bullet.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacter.h"
#include "Sound/SoundCue.h"

void AEnemyCop::Fire()
{
    if (!PlayerCharacter || !BulletClass) return;

    FVector SpawnLocation = MuzzlePoint->GetComponentLocation();
    FVector BaseDirection = (PlayerCharacter->GetActorLocation() - SpawnLocation).GetSafeNormal();

    // 3 directions: center, up, down
    TArray<FVector> Directions;

    Directions.Add(BaseDirection); // center

    // Rotate 45 degrees around Z axis (since you're in 2D X/Z plane)
    Directions.Add(BaseDirection.RotateAngleAxis(45.f, FVector(0, 1, 0)));  // up
    Directions.Add(BaseDirection.RotateAngleAxis(-45.f, FVector(0, 1, 0))); // down

    for (FVector Dir : Directions)
    {
        FRotator Rotation = Dir.Rotation();

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = this;

        ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(
            BulletClass,
            SpawnLocation,
            Rotation,
            SpawnParams
        );

        if (Bullet)
        {
            Bullet->GetProjectileMovement()->Velocity = Dir * 2000.f;
            Bullet->GetProjectileMovement()->InitialSpeed = BulletSpeed;
            Bullet->GetProjectileMovement()->MaxSpeed = BulletSpeed;

            Bullet->GetBoxCollision()->IgnoreActorWhenMoving(this, true);
        }
    }

    // play sound once (not 3 times)
    UGameplayStatics::PlaySoundAtLocation(this, ShootSound, GetActorLocation());
}

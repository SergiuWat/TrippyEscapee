// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/SpawnManager.h"
#include "Enemies/EnemyBase.h"
#include "Characters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawnManager::SpawnEnemy()
{
    if (!GetWorld()) return;

    TSubclassOf<AEnemyBase> ClassToSpawn = nullptr;

    switch (EnemyType)
    {
    case EEnemyTypes::EET_COP_1:
        ClassToSpawn = Cop1Class;
        break;

    case EEnemyTypes::EET_COP_2:
        ClassToSpawn = Cop2Class;
        break;
    }
	FVector LocationToSpawn = GetActorLocation();
    LocationToSpawn.Z += 150.f;

	FRotator RotationToSpawn = GetActorRotation();
	RotationToSpawn.Yaw += 180.f;
    if (ClassToSpawn)
    {
        SpawnedEnemy = GetWorld()->SpawnActor<AEnemyBase>(ClassToSpawn, LocationToSpawn, RotationToSpawn);
    }
}

void ASpawnManager::DespawnEnemy()
{
    if (SpawnedEnemy)
    {
        SpawnedEnemy->Destroy();
        SpawnedEnemy = nullptr;
    }
}

void ASpawnManager::HandlePlayerDeath()
{
    DespawnEnemy();
}

void ASpawnManager::HandlePlayerRespawn()
{
	SpawnEnemy();
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if(PlayerCharacter)
	{
		PlayerCharacter->OnPlayerDeath.AddDynamic(this, &ASpawnManager::HandlePlayerDeath);
		PlayerCharacter->OnPlayerRespawn.AddDynamic(this, &ASpawnManager::HandlePlayerRespawn);
	}
    SpawnEnemy();
}

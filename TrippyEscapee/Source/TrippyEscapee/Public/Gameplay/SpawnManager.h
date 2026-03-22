// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class AEnemyBase;
class APlayerCharacter;
UENUM(BlueprintType)
enum class EEnemyTypes : uint8
{
	EET_COP_1 UMETA(DisplayName = "Cop 1"),
	EET_COP_2 UMETA(DisplayName = "Cop 2"),


	EET_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class TRIPPYESCAPEE_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy")
	EEnemyTypes EnemyType = EEnemyTypes::EET_COP_1;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemyBase> Cop1Class;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	TSubclassOf<AEnemyBase> Cop2Class;

	UPROPERTY()
	AEnemyBase* SpawnedEnemy;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	void SpawnEnemy();
	void DespawnEnemy();

	UFUNCTION()
	void HandlePlayerDeath();

	UFUNCTION()
	void HandlePlayerRespawn();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	


};

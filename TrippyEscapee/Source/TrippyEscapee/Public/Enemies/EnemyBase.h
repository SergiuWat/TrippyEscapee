// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "EnemyBase.generated.h"

class UPawnSensingComponent;
class APlayerCharacter;
class ABullet;
class ABaseStamp;
class UPaperSpriteComponent;
class USoundCue;
class UPaperFlipbook;

USTRUCT(BlueprintType)
struct FStampDrop
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<ABaseStamp> StampClass;

	UPROPERTY(EditAnywhere)
	float DropChance; // e.g. 10 = 10%, 50 = 50%
};
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API AEnemyBase : public APaperZDCharacter
{
	GENERATED_BODY()

public:

	AEnemyBase();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

	UPROPERTY()
	APlayerCharacter* PlayerCharacter;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	bool bHasSeenPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MinDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float FireRate;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float BulletSpeed;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<ABullet> BulletClass;

	UPROPERTY(VisibleAnywhere, Category = "Combat")
	USceneComponent* MuzzlePoint;

	FTimerHandle FireTimer;

	UPROPERTY(EditAnywhere, Category = "Stamps")
	TArray<FStampDrop> StampDrops;

	TSubclassOf<ABaseStamp>GetRandomStamp();

	void StartFiring();
	void StopFiring();
	virtual void Fire();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	FTimerHandle EnemyDeadTimerHandle;

	UFUNCTION()
	void OnEnemyDeadTimerFinished();

	float Health = 100.f;
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	bool BIsEnemyDead = false;


	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	UPaperFlipbook* DeathFlipbook;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsEnemyType2 = false;


	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCause);

	/*
	* Hand logic
	*/
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HandPivot;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* HandSprite;

	void UpdateHandRotation();

	UPROPERTY(BlueprintReadOnly)
	bool bIsHitAnimationFinished = true;

	FTimerHandle HitAnimationTimerHandle;

	UFUNCTION()
	void OnHitAnimationFinished();

	bool bIsStunned = false;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	UPaperFlipbook* HitFlipbook;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundCue* ShootSound;

	/*
	* Upside down
	*/

	void UpdatePivot(float DirectionX);
	UFUNCTION()
	void OnUpsideDownActivate();

	UFUNCTION()
	void OnUpsideDownDezactivate();

	bool bIsUpsideDown = false;


protected:

	virtual void BeginPlay() override;
private:

	
};

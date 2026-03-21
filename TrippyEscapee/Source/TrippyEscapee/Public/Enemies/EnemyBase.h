// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "EnemyBase.generated.h"

class UPawnSensingComponent;
class APlayerCharacter;
class ABullet;
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

	void StartFiring();
	void StopFiring();
	void Fire();

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	float Health = 100.f;
	float MaxHealth = 100.f;

	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCause);
protected:

	virtual void BeginPlay() override;
private:

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDCharacter.h"
#include "PlayerCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class ABullet;
class APlayerCharacterController;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API APlayerCharacter : public APaperZDCharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* FollowCamera;

	/*
	* Input actions
	*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	void Move(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SpawnBulletPosition;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;

	float Health = 100.f;
	float MaxHealth = 100.f;

	UFUNCTION()
	void ReceiveDamage(AActor* DamageActor, float Damage, const UDamageType* DamageType, class AController* InstigatorController, AActor* DamageCause);

	/*
	* Checkpoint
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	FVector LastCheckpointLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Checkpoint")
	float SavedHealth;

	void SetCheckpoint(FVector NewLocation);

	void Respawn();

protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	APlayerCharacterController* PlayerCharacterController;
public:
	FORCEINLINE class USpringArmComponent* GetSprintArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
};

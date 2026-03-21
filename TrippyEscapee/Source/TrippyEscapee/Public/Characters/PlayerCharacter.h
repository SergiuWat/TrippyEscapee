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
class UPaperSpriteComponent;
class UPaperFlipbook;
class USoundCue;
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* ConfusedMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* ShootAction;

	void Move(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundCue* ShootSound;

	void Shoot(const FInputActionValue& Value);
	

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SpawnBulletPosition;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<ABullet> BulletClass;

	/*
	* Damage logic
	*/


	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	UPaperFlipbook* HitFlipbook;

	float Health = 100.f;
	float MaxHealth = 100.f;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHitAnimationFinished = true;

	FTimerHandle HitAnimationTimerHandle;

	void OnHitAnimationFinished();

	FTimerHandle DamageTimerHandle;
	bool bTookDamage = false;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float DamageCooldown = 2.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float FlickerDuration = 0.1f;

	float FlickerTimer = FlickerDuration;

	UFUNCTION()
	void TookDamageTimerFinished();

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


	/*
	* Confused Controls
	*/
	FTimerHandle ConfusedTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Confused Controls")
	float ConfusedDuration = 5.f;

	void SetConfusedControls(bool bIsConfused);
	void ConfusedTimerFinished();


	/*
	* Reverse Damage
	*/
	FTimerHandle  ReverseDamageTimerHandle;

	bool bIsReverseDamageActive = false;

	UPROPERTY(EditAnywhere, Category = "Reverse Damage")
	float ReverseDamageDuration = 5.f;

	void SetReverseDamage(bool bIsReverseDamage);
	void ReverseDamageTimerFinished();

	/*
	* Stamps
	*/

	bool bIsStampActive = false;


	/*
	* Hand logic
	*/
	UPROPERTY(VisibleAnywhere)
	USceneComponent* HandPivot;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* HandSprite;

	void UpdateHandRotation();


protected:
	virtual void BeginPlay() override;
private:
	UPROPERTY()
	APlayerCharacterController* PlayerCharacterController;
public:
	FORCEINLINE class USpringArmComponent* GetSprintArm() const { return SpringArm; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE void SetStampActivation(bool active) { bIsStampActive = active; }
	FORCEINLINE bool GetStampActivation() { return bIsStampActive ; }
	FORCEINLINE bool GetIsReverseDamageActive() { return bIsReverseDamageActive; }
	
};

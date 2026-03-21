// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "Bullet.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API ABullet : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();
	
	UPROPERTY(VisibleAnywhere);
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere);
	UProjectileMovementComponent* ProjectileMovement;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

public:
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
	UBoxComponent* GetBoxCollision() const { return BoxCollision; }
};

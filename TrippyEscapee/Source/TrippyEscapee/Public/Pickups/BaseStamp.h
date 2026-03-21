// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "BaseStamp.generated.h"

class USphereComponent;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API ABaseStamp : public APaperSpriteActor
{
	GENERATED_BODY()
public:

	ABaseStamp();

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* Sprite;

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};

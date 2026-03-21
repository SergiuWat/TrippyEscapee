// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperFlipbookActor.h"
#include "Checkpoint.generated.h"

class UBoxComponent;
class UPaperFlipbook;
class UPaperSpriteComponent;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API ACheckpoint : public APaperFlipbookActor
{
	GENERATED_BODY()

public:
	ACheckpoint();

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	UPaperFlipbook* RiseFlipbook;

	UPROPERTY(EditAnywhere, Category = "Checkpoint")
	UPaperFlipbook* IdleFlipbook;

	bool bIsActivated;
	FTimerHandle FlipbookTimer;
	void OnRiseFinished();
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* Collision;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);
};

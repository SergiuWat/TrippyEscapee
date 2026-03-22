// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BaseStamp.h"
#include "UpsideDownStamp.generated.h"

/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API AUpsideDownStamp : public ABaseStamp
{
	GENERATED_BODY()

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};

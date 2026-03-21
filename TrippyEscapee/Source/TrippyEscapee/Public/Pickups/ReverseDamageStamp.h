// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickups/BaseStamp.h"
#include "ReverseDamageStamp.generated.h"

/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API AReverseDamageStamp : public ABaseStamp
{
	GENERATED_BODY()
	
public:

protected:
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};

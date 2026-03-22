// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EnemyBase.h"
#include "EnemyCop.generated.h"

/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API AEnemyCop : public AEnemyBase
{
	GENERATED_BODY()

public:
	

	virtual void Fire() override;
};

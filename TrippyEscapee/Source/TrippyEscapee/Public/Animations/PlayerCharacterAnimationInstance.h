// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperZDAnimInstance.h"
#include "PlayerCharacterAnimationInstance.generated.h"

/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API UPlayerCharacterAnimationInstance : public UPaperZDAnimInstance
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;

	void Init(TScriptInterface<IPaperZDAnimInstanceManager> InManager);
};

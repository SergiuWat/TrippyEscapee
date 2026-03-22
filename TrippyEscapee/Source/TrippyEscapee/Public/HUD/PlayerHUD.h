// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

class UUserWidget;
class UCharacterOverlay;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API APlayerHUD : public AHUD
{
	GENERATED_BODY()

public:

	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCharacterOverlay> CharacterOverlayClass;

	UPROPERTY()
	UCharacterOverlay* CharacterOverlay;


	void AddCharacterOverlay();

	UCharacterOverlay* GetOverlay() const { return CharacterOverlay; }

protected:
	virtual void BeginPlay() override;
	
};

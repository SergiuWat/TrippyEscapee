// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerHUD.h"
#include "HUD/CharacterOverlay.h"

void APlayerHUD::BeginPlay()
{
	Super::BeginPlay();
	AddCharacterOverlay();
}

void APlayerHUD::DrawHUD()
{
	Super::DrawHUD();
}

void APlayerHUD::AddCharacterOverlay()
{
	CharacterOverlay = CreateWidget<UCharacterOverlay>(GetWorld(), CharacterOverlayClass);

	if (CharacterOverlay)
	{
		CharacterOverlay->AddToViewport();
	}
}

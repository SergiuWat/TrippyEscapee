// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/CharacterOverlay.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"

void UCharacterOverlay::UpdateHearts(int32 NewHealth)
{
    Heart1->SetBrushFromTexture(NewHealth >= 1 ? FullHeart : EmptyHeart);
    Heart2->SetBrushFromTexture(NewHealth >= 2 ? FullHeart : EmptyHeart);
    Heart3->SetBrushFromTexture(NewHealth >= 3 ? FullHeart : EmptyHeart);
    Heart4->SetBrushFromTexture(NewHealth >= 4 ? FullHeart : EmptyHeart);
    Heart5->SetBrushFromTexture(NewHealth >= 5 ? FullHeart : EmptyHeart);
}

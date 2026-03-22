// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterOverlay.generated.h"

class UImage;
class UTexture2D;
/**
 * 
 */
UCLASS()
class TRIPPYESCAPEE_API UCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHearts(int32 NewHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hearts")
	UTexture2D* FullHeart;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hearts")
	UTexture2D* EmptyHeart;

	UPROPERTY(meta = (BindWidget))
	UImage* Heart1;

	UPROPERTY(meta = (BindWidget))
	UImage* Heart2;

	UPROPERTY(meta = (BindWidget))
	UImage* Heart3;

	UPROPERTY(meta = (BindWidget))
	UImage* Heart4;

	UPROPERTY(meta = (BindWidget))
	UImage* Heart5;
	
};

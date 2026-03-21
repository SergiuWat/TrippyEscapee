// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/MixedControlsStamp.h"
#include "Characters/PlayerCharacter.h"

void AMixedControlsStamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (Player->GetStampActivation()) return;
		Player->SetConfusedControls(true);
		Destroy();
	}
}

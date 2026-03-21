// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/ReverseDamageStamp.h"
#include "Characters/PlayerCharacter.h"

void AReverseDamageStamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("GOOD CAST"));
		if (Player->GetStampActivation()) return;

		Player->SetStampActivation(true);
		Player->SetReverseDamage(true);
		UE_LOG(LogTemp, Warning, TEXT("DESTROYED"));
		Destroy();
	}
}

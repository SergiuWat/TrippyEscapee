// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickups/UpsideDownStamp.h"
#include "Characters/PlayerCharacter.h"

void AUpsideDownStamp::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);

    if (Player)
    {
        Player->SetUpsideDown(true);
        Destroy();
    }
}

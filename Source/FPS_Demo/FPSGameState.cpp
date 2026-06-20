// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "Net/UnrealNetwork.h"

AFPSGameState::AFPSGameState()
{
}

void AFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AFPSGameState, CurrentScore);
    DOREPLIFETIME(AFPSGameState, TargetScore);
    DOREPLIFETIME(AFPSGameState, GameResultState);
    DOREPLIFETIME(AFPSGameState, DeadPlayerCount);
}
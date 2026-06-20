// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "FPSGameState.h"
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode()
{
    GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::AddScore(int32 ScoreAmount)
{
    AFPSGameState* FPSGameState = GetGameState<AFPSGameState>();
    if (!FPSGameState)
    {
        return;
    }

    if (FPSGameState->GameResultState != EGameResultState::Playing)
    {
        return;
    }

    FPSGameState->CurrentScore += ScoreAmount;

    CheckVictory();
}

void AFPSGameMode::CheckVictory()
{

    AFPSGameState* FPSGameState = GetGameState<AFPSGameState>();
    if (!FPSGameState)
    {
        return;
    }

    if (FPSGameState->GameResultState != EGameResultState::Playing)
    {
        return;
    }

    if (FPSGameState->CurrentScore >= FPSGameState->TargetScore)
    {
        FPSGameState->GameResultState = EGameResultState::Victory;
    }

}

void AFPSGameMode::GameOver()
{
    AFPSGameState* FPSGameState = GetGameState<AFPSGameState>();
    if (!FPSGameState)
    {
        return;
    }

    if (FPSGameState->GameResultState != EGameResultState::Playing)
    {
        return;
    }

    FPSGameState->GameResultState = EGameResultState::GameOver;
}

void AFPSGameMode::NotifyPlayerDied()
{
    AFPSGameState* FPSGameState = GetGameState<AFPSGameState>();
    if (!FPSGameState)
    {
        return;
    }

    if (!FPSGameState->IsGamePlaying())
    {
        return;
    }

    FPSGameState->DeadPlayerCount++;

    const int32 PlayerCount = GameState ? GameState->PlayerArray.Num() : 0;

    if (PlayerCount > 0 && FPSGameState->DeadPlayerCount >= PlayerCount)
    {
        GameOver();
    }
}
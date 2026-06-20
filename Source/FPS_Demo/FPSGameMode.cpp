// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode()
{
}

void AFPSGameMode::AddScore(int32 ScoreAmount)
{
    if (GameResultState != EGameResultState::Playing)
    {
        return;
    }

    CurrentScore += ScoreAmount;

    UE_LOG(LogTemp, Warning, TEXT("Score: %d / %d"), CurrentScore, TargetScore);

    CheckVictory();
}

void AFPSGameMode::CheckVictory()
{
    if (GameResultState != EGameResultState::Playing)
    {
        return;
    }

    if (CurrentScore >= TargetScore)
    {
        GameResultState = EGameResultState::Victory;

        UE_LOG(LogTemp, Warning, TEXT("Victory!"));
    }
}

void AFPSGameMode::GameOver()
{
    if (GameResultState != EGameResultState::Playing)
    {
        return;
    }

    GameResultState = EGameResultState::GameOver;

    UE_LOG(LogTemp, Warning, TEXT("Game Over"));
}
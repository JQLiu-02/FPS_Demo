// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameMode.h"
#include "Engine/Engine.h"

AFPSGameMode::AFPSGameMode()
{
}

void AFPSGameMode::AddScore(int32 ScoreAmount)
{
    if (bGameWon)
    {
        return;
    }

    CurrentScore += ScoreAmount;

    UE_LOG(LogTemp, Warning, TEXT("Score: %d / %d"), CurrentScore, TargetScore);

    if (GEngine)
    {
        const FString ScoreMessage = FString::Printf(
            TEXT("Score: %d / %d"),
            CurrentScore,
            TargetScore
        );

        GEngine->AddOnScreenDebugMessage(
            -1,
            2.0f,
            FColor::Yellow,
            ScoreMessage
        );
    }

    CheckVictory();
}

void AFPSGameMode::CheckVictory()
{
    if (CurrentScore >= TargetScore)
    {
        bGameWon = true;

        UE_LOG(LogTemp, Warning, TEXT("Victory!"));

        if (GEngine)
        {
            GEngine->AddOnScreenDebugMessage(
                -1,
                5.0f,
                FColor::Green,
                TEXT("Victory!")
            );
        }
    }
}
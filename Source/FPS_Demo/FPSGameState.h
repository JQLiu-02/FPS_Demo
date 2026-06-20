// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPSGameTypes.h"
#include "FPSGameState.generated.h"

UCLASS()
class FPS_DEMO_API AFPSGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    AFPSGameState();

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
    int32 CurrentScore = 0;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
    int32 TargetScore = 40;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    EGameResultState GameResultState = EGameResultState::Playing;

    UPROPERTY(Replicated, BlueprintReadOnly, Category = "Game State")
    int32 DeadPlayerCount = 0;

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetCurrentScore() const { return CurrentScore; }

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetTargetScore() const { return TargetScore; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsGameWon() const { return GameResultState == EGameResultState::Victory; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsGameOver() const { return GameResultState == EGameResultState::GameOver; }

    UFUNCTION(BlueprintPure, Category = "Game State")
    bool IsGamePlaying() const { return GameResultState == EGameResultState::Playing; }

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
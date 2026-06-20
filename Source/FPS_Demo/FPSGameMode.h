// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameResultState : uint8
{
    Playing UMETA(DisplayName = "Playing"),
    Victory UMETA(DisplayName = "Victory"),
    GameOver UMETA(DisplayName = "Game Over")
};

UCLASS()
class FPS_DEMO_API AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
    AFPSGameMode();


protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Score")
    int32 TargetScore = 30;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    int32 CurrentScore = 0;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
    EGameResultState GameResultState = EGameResultState::Playing;

    void CheckVictory();

public:

    void AddScore(int32 ScoreAmount);

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

    void GameOver();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Score")
    bool bGameWon = false;

    void CheckVictory();

public:

    void AddScore(int32 ScoreAmount);

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetCurrentScore() const { return CurrentScore; }

    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetTargetScore() const { return TargetScore; }

    UFUNCTION(BlueprintPure, Category = "Score")
    bool IsGameWon() const { return bGameWon; }
};

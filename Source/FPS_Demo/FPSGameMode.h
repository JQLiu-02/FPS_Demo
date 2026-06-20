// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameTypes.h"
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

    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 ScoreAmount);

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void GameOver();

    UFUNCTION(BlueprintCallable, Category = "Game State")
    void NotifyPlayerDied();

protected:
    void CheckVictory();
};

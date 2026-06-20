#pragma once

#include "CoreMinimal.h"
#include "FPSGameTypes.generated.h"

UENUM(BlueprintType)
enum class EGameResultState : uint8
{
    Playing UMETA(DisplayName = "Playing"),
    Victory UMETA(DisplayName = "Victory"),
    GameOver UMETA(DisplayName = "Game Over")
};
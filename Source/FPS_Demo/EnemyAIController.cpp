// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "FPSGameState.h"
#include "Kismet/GameplayStatics.h"

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    TargetPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

    if (!TargetPlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy AI: PlayerPawn not found"));
        return;
    }

    UE_LOG(LogTemp, Warning, TEXT("Enemy AI: Start chasing player"));

    UpdateChase();

    GetWorldTimerManager().SetTimer(
        ChaseTimerHandle,
        this,
        &AEnemyAIController::UpdateChase,
        ChaseUpdateInterval,
        true
    );
}

void AEnemyAIController::UpdateChase()
{
    if (!HasAuthority())
    {
        return;
    }

    const AFPSGameState* FPSGameState = GetWorld()->GetGameState<AFPSGameState>();
    if (FPSGameState && !FPSGameState->IsGamePlaying())
    {
        StopMovement();
        return;
    }

    TargetPlayerPawn = FindNearestAlivePlayer();

    if (!TargetPlayerPawn)
    {
        StopMovement();
        return;
    }

    const APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return;
    }

    const float Distance = FVector::Dist(
        ControlledPawn->GetActorLocation(),
        TargetPlayerPawn->GetActorLocation()
    );



    if (Distance <= AttackRange)
    {
        StopMovement();
        SetFocus(TargetPlayerPawn);
        TryAttack();
        return;
    }

    SetFocus(TargetPlayerPawn);

    MoveToActor(
        TargetPlayerPawn,
        AcceptanceRadius,
        false
    );
}

void AEnemyAIController::TryAttack()
{
    if (!bCanAttack)
    {
        return;
    }

    AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(GetPawn());
    if (!Enemy)
    {
        return;
    }

    Enemy->PlayAttackMontage();

    if (AFPSCharacter* PlayerCharacter = Cast<AFPSCharacter>(TargetPlayerPawn))
    {
        PlayerCharacter->TakeDamageFromEnemy(20.0f);
    }

    bCanAttack = false;

    GetWorldTimerManager().SetTimer(
        AttackCooldownTimerHandle,
        this,
        &AEnemyAIController::ResetAttackCooldown,
        AttackInterval,
        false
    );
}

void AEnemyAIController::ResetAttackCooldown()
{
    bCanAttack = true;
}

APawn* AEnemyAIController::FindNearestAlivePlayer() const
{
    const APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return nullptr;
    }

    TArray<AActor*> PlayerActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFPSCharacter::StaticClass(), PlayerActors);

    APawn* BestTarget = nullptr;
    float BestDistanceSq = TNumericLimits<float>::Max();

    for (AActor* Actor : PlayerActors)
    {
        AFPSCharacter* FPSCharacter = Cast<AFPSCharacter>(Actor);
        if (!FPSCharacter)
        {
            continue;
        }

        if (FPSCharacter->IsDead())
        {
            continue;
        }

        const float DistanceSq = FVector::DistSquared(
            ControlledPawn->GetActorLocation(),
            FPSCharacter->GetActorLocation()
        );

        if (DistanceSq < BestDistanceSq)
        {
            BestDistanceSq = DistanceSq;
            BestTarget = FPSCharacter;
        }
    }

    return BestTarget;
}
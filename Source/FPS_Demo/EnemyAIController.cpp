// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSCharacter.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "TimerManager.h"
#include "FPSGameMode.h"
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
    AFPSGameMode* FPSGameMode = Cast<AFPSGameMode>(UGameplayStatics::GetGameMode(this));
    if (FPSGameMode && !FPSGameMode->IsGamePlaying())
    {
        StopMovement();
        return;
    }

    if (!TargetPlayerPawn)
    {
        return;
    }

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn)
    {
        return;
    }

    const float Distance = FVector::Dist(
        ControlledPawn->GetActorLocation(),
        TargetPlayerPawn->GetActorLocation()
    );

    SetFocus(TargetPlayerPawn);

    if (Distance <= AttackRange)
    {
        StopMovement();
        TryAttack();
        return;
    }

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
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class FPS_DEMO_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float AcceptanceRadius = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float ChaseUpdateInterval = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackInterval = 1.2f;

	UPROPERTY()
	TObjectPtr<APawn> TargetPlayerPawn;

	FTimerHandle ChaseTimerHandle;

	FTimerHandle AttackCooldownTimerHandle;

	bool bCanAttack = true;

	void UpdateChase();

	void TryAttack();

	void ResetAttackCooldown();

};

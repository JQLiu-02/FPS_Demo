// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UAnimMontage;

UCLASS()
class FPS_DEMO_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth = 100.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	bool bIsDead = false;



	void Die();

public:

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeHit(float DamageAmount);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayAttackMontage();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};

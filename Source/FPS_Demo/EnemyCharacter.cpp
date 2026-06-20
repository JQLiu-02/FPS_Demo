// Fill out your copyright notice in the Description page of Project Settings.

#include "Animation/AnimInstance.h"
#include "EnemyAIController.h"
#include "EnemyCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    SetReplicateMovement(true);

    AIControllerClass = AEnemyAIController::StaticClass();
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurrentHealth = MaxHealth;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::TakeHit(float DamageAmount)
{
    if (!HasAuthority())
    {
        return;
    }

    if (bIsDead)
    {
        return;
    }

    CurrentHealth -= DamageAmount;

    if (CurrentHealth <= 0.0f)
    {
        CurrentHealth = 0.0f;
        bIsDead = true;

        Die();
    }
}

void AEnemyCharacter::Die()
{
    if (!HasAuthority())
    {
        return;
    }

    AFPSGameMode* FPSGameMode = GetWorld()->GetAuthGameMode<AFPSGameMode>();
    if (FPSGameMode)
    {
        FPSGameMode->AddScore(10);
    }

    Destroy();
}
void AEnemyCharacter::PlayAttackMontage()
{
    if (!AttackMontage)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy AttackMontage is null"));
        return;
    }

    UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
    if (!AnimInstance)
    {
        UE_LOG(LogTemp, Warning, TEXT("Enemy AnimInstance is null"));
        return;
    }

    AnimInstance->Montage_Play(AttackMontage);

    UE_LOG(LogTemp, Warning, TEXT("Enemy Play Attack Montage"));
}

void AEnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AEnemyCharacter, CurrentHealth);
    DOREPLIFETIME(AEnemyCharacter, bIsDead);
}
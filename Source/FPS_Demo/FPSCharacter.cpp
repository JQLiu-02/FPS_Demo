// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // FPS 常用设置：鼠标控制角色朝向
    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = false;
    bUseControllerRotationRoll = false;

    // 不让移动方向自动改变角色朝向，FPS 里朝向由鼠标决定
    GetCharacterMovement()->bOrientRotationToMovement = false;

    // 调整第三人称 Mesh，让它和角色箭头方向对齐
    GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
    GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

    // 创建第一人称 Mesh
    FirstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
    check(FirstPersonMeshComponent != nullptr);

    // 按教程：挂到 Character 默认 Mesh 上
    FirstPersonMeshComponent->SetupAttachment(GetMesh());

    // 创建第一人称 Camera
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    check(FirstPersonCameraComponent != nullptr);

    // 按教程：挂到 FirstPersonMesh 的 head 插槽上
    FirstPersonCameraComponent->SetupAttachment(FirstPersonMeshComponent, FName(TEXT("head")));

    // 调整摄像机到眼睛附近
    FirstPersonCameraComponent->SetRelativeLocationAndRotation(
        FirstPersonCameraOffset,
        FRotator(0.0f, 90.0f, -90.0f)
    );

    // 摄像机使用 Pawn 控制旋转
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // 第一人称渲染设置
    FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
    FirstPersonCameraComponent->bEnableFirstPersonScale = true;
    FirstPersonCameraComponent->FirstPersonFieldOfView = FirstPersonFieldOfView;
    FirstPersonCameraComponent->FirstPersonScale = FirstPersonScale;

    // 第一人称 Mesh 使用第一人称渲染通道
    FirstPersonMeshComponent->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;

    // 第三人称 Mesh 用作世界空间表现
    GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
    check(GEngine != nullptr);

    // 自己只看第一人称 Mesh
    if (FirstPersonMeshComponent)
    {
        FirstPersonMeshComponent->SetOnlyOwnerSee(true);
        FirstPersonMeshComponent->SetCastShadow(false);
    }
    // 自己不看第三人称 Mesh
    GetMesh()->SetOwnerNoSee(true);

    // 添加 Enhanced Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                if (FirstPersonContext)
                {
                    Subsystem->AddMappingContext(FirstPersonContext, 0);
                }
            }
        }
    }

    // Get the player controller for this character
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        // Get the enhanced input local player subsystem and add a new input mapping context to it
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(FirstPersonContext, 0);
        }
    }
    // Display a debug message for five seconds. 
    // The -1 "Key" value argument prevents the message from being updated or refreshed.
    GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("We are using AdventureCharacter."));
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    UEnhancedInputComponent* EnhancedInputComponent =
        Cast<UEnhancedInputComponent>(PlayerInputComponent);

    if (!EnhancedInputComponent)
    {
        return;
    }

    if (MoveAction)
    {
        EnhancedInputComponent->BindAction(
            MoveAction,
            ETriggerEvent::Triggered,
            this,
            &AFPSCharacter::Move
        );
    }

    if (LookAction)
    {
        EnhancedInputComponent->BindAction(
            LookAction,
            ETriggerEvent::Triggered,
            this,
            &AFPSCharacter::Look
        );
    }

    if (JumpAction)
    {
        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Started,
            this,
            &AFPSCharacter::StartJump
        );

        EnhancedInputComponent->BindAction(
            JumpAction,
            ETriggerEvent::Completed,
            this,
            &AFPSCharacter::StopJump
        );
    }

    if (FireAction)
    {
        EnhancedInputComponent->BindAction(
            FireAction,
            ETriggerEvent::Started,
            this,
            &AFPSCharacter::Fire
        );
    }
}

void AFPSCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller)
    {
        AddMovementInput(GetActorForwardVector(), MovementVector.Y);
        AddMovementInput(GetActorRightVector(), MovementVector.X);
    }
}

void AFPSCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller)
    {
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void AFPSCharacter::StartJump()
{
    Jump();
}

void AFPSCharacter::StopJump()
{
    StopJumping();
}

void AFPSCharacter::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire"));
}
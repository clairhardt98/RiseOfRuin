// Fill out your copyright notice in the Description page of Project Settings.


#include "CPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Data/InputDataAsset.h"

ACPlayerController::ACPlayerController(const FObjectInitializer& ObjectInitializer)
{
}


void ACPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ACPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

////////////////////////////////////////////////////////////////////////////
///input
////////////////////////////////////////////////////////////////////////////
void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// local player subsystem을 가져옴
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer());

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMappingContext, 0);

	// EnhancedInputComponent를 가져옴
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);

	// 액션 바인딩

	EnhancedInputComponent->BindAction(InputActions->InputMove, ETriggerEvent::Triggered, this, &ACPlayerController::Move);
	EnhancedInputComponent->BindAction(InputActions->InputLook, ETriggerEvent::Triggered, this, &ACPlayerController::Look);
	EnhancedInputComponent->BindAction(InputActions->InputSpaceAction, ETriggerEvent::Started, this, &ACPlayerController::SpaceActionStart);
	EnhancedInputComponent->BindAction(InputActions->InputSpaceAction, ETriggerEvent::Completed, this, &ACPlayerController::SpaceActionComplete);

	// !TODO : 약공격 및 강공격에 대한 함수를 추가해야 합니다
}

void ACPlayerController::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ACPlayerController::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2d>();

	GetCharacter()->AddControllerYawInput(LookAxisVector.X);
	GetCharacter()->AddControllerPitchInput(LookAxisVector.Y);
}

void ACPlayerController::SpaceActionStart(const FInputActionValue& Value)
{
	// 여기서 스페이스바를 누른 시점에 시간 측정 시작
	

	// 일정 시간 이후에 SpaceActionComplete함수가 호출되지 않았다면 달리기를 시작함
}

void ACPlayerController::SpaceActionComplete(const FInputActionValue& Value)
{
	// 시간 측정 이후 흐른 시간이 일정 시간보다 짧다면 구르기

	// 아니라면 달리기를 멈추기
}

void ACPlayerController::Sprint()
{
}

void ACPlayerController::Dodge()
{
}


void ACPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

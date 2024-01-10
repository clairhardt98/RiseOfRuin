// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RiseOfRuinCharacter.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"

class UInputMappingContext;
class UInputDataAsset;

UCLASS()
class RISEOFRUIN_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void SpaceActionStart(const FInputActionValue& Value);
	void SpaceActionComplete(const FInputActionValue& Value);

protected:
	void Sprint();
	void Dodge();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputDataAsset* InputActions;
};

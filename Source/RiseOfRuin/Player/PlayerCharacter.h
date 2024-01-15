// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UInputDataAsset;
struct FInputActionValue;

UCLASS()
class RISEOFRUIN_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	/** 캐릭터를 따라다니는 카메라에 부착되는 카메라 붐 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** 팔로우 카메라 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** 매핑 컨텍스트 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** 인풋 데이터 에셋 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputDataAsset* InputDataAsset;
	

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** 스페이스 바 입력 시 로직 처리 */
	void OnSpacePressed(const FInputActionValue& Value);

	/** 스페이스 바 입력 해제 시 로직 처리 */
	void OnSpaceReleased(const FInputActionValue& Value);

public:	

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};

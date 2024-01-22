#include "PlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "RiseOfRuin/RiseOfRuin.h"
#include "RiseOfRuin/RiseOfRuinCharacter.h"
#include "RiseOfRuin/Data/InputDataAsset.h"
#include "InputMappingContext.h"

APlayerCharacter::APlayerCharacter()
{
	// 충돌 캡슐 콜라이더 사이즈 지정
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// 컨트롤러의 회전을 캐릭터가 아닌 카메라에 한함
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터 움직임 세팅
	GetCharacterMovement()->bOrientRotationToMovement = true; // 캐릭터가 인풋에 딱 맞게 움직임
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 회전강도

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// 카메라가 달려있는 SprintArmComponent
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // 카메라가 플레이어를 따라다니는 거리
	CameraBoom->bUsePawnControlRotation = true; // 컨트롤러 회전 시 암도 같이 돎

	// 플레이어를 따라다니는 카메라
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // 카메라 붐 끝에 카메라 부착
	FollowCamera->bUsePawnControlRotation = false; // 암이 회전한다고 해서 카메라가 회전하지 않음

	//스켈레탈 메시
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_KWANG
	(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Meshes/KwangRosewood.KwangRosewood"));
	if(SK_KWANG.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Skeletal Mesh Success"));
		GetMesh()->SetSkeletalMesh(SK_KWANG.Object);
	}
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FRotator(0.0f, -90.0f, 0.0f));
	//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//애니메이션 블루프린트
	static ConstructorHelpers::FClassFinder<UAnimInstance> KWANG_ANIM
	(TEXT("/Game/ParagonKwang/Characters/Heroes/Kwang/Animations/Kwang_AnimBlueprint.Kwang_AnimBlueprint_C"));
	if(KWANG_ANIM.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("Anim Blueprint Success"));
		GetMesh()->SetAnimInstanceClass(KWANG_ANIM.Class);
	}
	//인풋 매핑 컨텍스트 가져오기
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> INPUT_MAPPING_CONTEXT
	(TEXT("Game/Input/IMC_DefaultInputMappingContext.IMC_DefaultInputMappingContext"));
	if(INPUT_MAPPING_CONTEXT.Succeeded())
	{
		PRINT_LOG(TEXT("PlayerCharacter"), TEXT("InputMappingContext Load Succeeded"));
		DefaultMappingContext = INPUT_MAPPING_CONTEXT.Object;
	}
	//인풋 데이터 에셋 가져오기
	static ConstructorHelpers::FObjectFinder<UInputDataAsset> INPUT_DATA_ASSET
	(TEXT("/Game/Input/BP_InputAssetData.BP_InputAssetData"));
	if(INPUT_DATA_ASSET.Succeeded())
	{
		PRINT_LOG(TEXT("PlayerCharacter"), TEXT("InputDataAsset Load Succeeded"));
		InputDataAsset = INPUT_DATA_ASSET.Object;
	}
	
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(!InputDataAsset)
		{
			UE_LOG(LogTemplateCharacter, Error, TEXT("InputDataAsset is Null"));
		}
		else
		{
			// Move함수 바인딩
			EnhancedInputComponent->BindAction(InputDataAsset->InputMove, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
			// Look함수 바인딩
			EnhancedInputComponent->BindAction(InputDataAsset->InputLook, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
			// 스페이스 입력 시 함수 바인딩
			EnhancedInputComponent->BindAction(InputDataAsset->InputSpaceAction, ETriggerEvent::Started, this, &APlayerCharacter::OnSpacePressed);
			// 스페이스 입력 해제 시 함수 바인딩
			EnhancedInputComponent->BindAction(InputDataAsset->InputSpaceAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnSpaceReleased);
		}
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	// wasd 키보드 인풋값을 받아옴
	FVector2D MovementVector = Value.Get<FVector2D>();
	PRINT_LOG(TEXT("PlayerCharacter"), TEXT("Move"));

	if (Controller != nullptr)
	{
		// 앞쪽 방향 구하기
		const FRotator Rotation = Controller->GetControlRotation();
		// Yaw : 앞쪽 방향의 회전(고개를 좌우로 돌리는 회전)
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 전방 단위 벡터
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 우측방 단위 벡터
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 전방으로 움직임 벡터의 Y값(w,s)를 받아서 움직임
		AddMovementInput(ForwardDirection, MovementVector.Y);
		// 우측방으로 움직임 벡터의 X값(a, d)를 받아서 움직임
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// 마우스 인풋을 받아옴
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// 마우스 인풋의 X는 Controller의 Yaw에, Y는 Pitch에 입력해줌

		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::OnSpacePressed(const FInputActionValue& Value)
{
	PRINT_LOG(TEXT("PlayerCharacter : "), TEXT("OnSpacePressed"));
}

void APlayerCharacter::OnSpaceReleased(const FInputActionValue& Value)
{
	PRINT_LOG(TEXT("PlayerCharacter : "), TEXT("OnSpaceReleased"));
}

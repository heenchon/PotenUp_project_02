#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/SurvivalComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>("Survival Component");
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArm);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				GetLocalViewingPlayerController()->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<
		UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered
																			, this, &ThisClass::MoveTo);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered
																			, this, &ThisClass::Jump);
	}
}

void APlayerCharacter::MoveTo(const FInputActionValue& Value)
{
	const FVector VectorValue = Value.Get<FVector>();
	// 바라보는 방향에 상관없이 회전이 Yaw 쪽으로만 이동해야 하기 때문에 Yaw를 가져옴
	// 수영처럼 바라보는 방향이 Z축으로도 필요하다면 로직이 달라질 수 있음
	const FRotator Rotator = FRotator(0, GetController()->GetControlRotation().Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X)
	// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	* VectorValue.X
	* (VectorValue.Y != 0 ? UKismetMathLibrary::Sin(UKismetMathLibrary::DegreesToRadians(45)) : 1);
	
	const FVector RightDirection = FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y)
	// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	* VectorValue.Y
	* (VectorValue.X != 0 ? UKismetMathLibrary::Cos(UKismetMathLibrary::DegreesToRadians(45)) : 1);
	
	AddMovementInput(ForwardDirection);
	AddMovementInput(RightDirection);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector VectorValue = Value.Get<FVector>();

	// Yaw가 가로 회전이기 때문에 X값을 넣어줌
	AddControllerYawInput(VectorValue.X);
	// Pitch는 앞 뒤가 아닌 위 아래 회전이기 때문에 Y값을 넣어줌
	AddControllerPitchInput(VectorValue.Y);
}





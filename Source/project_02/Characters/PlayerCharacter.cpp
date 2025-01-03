#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Component/SurvivalComponent.h"
#include "Kismet/KismetMathLibrary.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>("Survival Component");
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

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	}
}

void APlayerCharacter::MoveTo(const FInputActionValue& Value)
{
	const FVector VectorValue = Value.Get<FVector>();
	
	const FVector ForwardDirection = FRotationMatrix(GetController()->GetControlRotation()).GetUnitAxis(EAxis::X)
	// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	* VectorValue.X
	* (VectorValue.Y != 0 ? UKismetMathLibrary::Sin(UKismetMathLibrary::DegreesToRadians(45)) : 1);
	
	const FVector RightDirection = FRotationMatrix(GetController()->GetControlRotation()).GetUnitAxis(EAxis::Y)
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



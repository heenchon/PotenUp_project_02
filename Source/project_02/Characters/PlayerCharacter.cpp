#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Component/SurvivalComponent.h"


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

	UE_LOG(LogTemp, Display, TEXT("%f, %f, %f"), VectorValue.X, VectorValue.Y, VectorValue.Z);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	
}



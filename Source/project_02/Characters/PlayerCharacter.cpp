#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/SurvivalComponent.h"
#include "Component/InventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/HY/Trash/Trash.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Tool/HookRope.h"
#include "project_02/HY/Raft/Raft.h"

APlayerCharacter::APlayerCharacter()
{
	SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>("Survival Component");
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
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
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::OnInteractiveHolding);
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Completed
		, this, &ThisClass::OnInteractiveEnd);
		EnhancedInputComponent->BindAction(UseInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::UseItem);
	}
}

void APlayerCharacter::UseItem()
{
	ABasePlayerState* PS = static_cast<ABasePlayerState*>(GetPlayerState());
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ATrash::StaticClass()))
	{
		ATrash* Trash = static_cast<ATrash*>(FindDroppedActor);
		
		const uint32 RemainValue = PS->AddItem(Trash->GetItemMetaInfo());
		Trash->UpdateItemInfo(RemainValue);
	}
}


void APlayerCharacter::SetTestInteractiveItem(const TSubclassOf<AActor>& NewActorClass)
{
	if (TestInteractiveItem)
	{
		TestInteractiveItem->Destroy();
	}
	
	if (NewActorClass)
	{
		TestInteractiveItem = GetWorld()->SpawnActor<AActor>(NewActorClass);
		
		if (TestInteractiveItem)
		{
			TestInteractiveItem->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::KeepRelativeTransform, "InteractiveSocket");
		}
	}
}

void APlayerCharacter::OnInteractiveHolding()
{
	if (IsBlockAction()) return;
	if (TestInteractiveItem && TestInteractiveItem.IsA(AHookRope::StaticClass()))
	{
		static_cast<AHookRope*>(TestInteractiveItem)->OnHoldInteractive();
	}	
}

void APlayerCharacter::OnInteractiveEnd()
{
	if (IsBlockAction()) return;
	if (TestInteractiveItem && TestInteractiveItem.IsA(AHookRope::StaticClass()))
	{
		static_cast<AHookRope*>(TestInteractiveItem)->OnEndInteractive();
	}
}

void APlayerCharacter::FindToUse()
{
	if (IsBlockAction()) return;
	
	const FVector StartPosition = SpringArm->GetComponentLocation();
	
	const FVector EndPosition = StartPosition + CameraComponent->GetForwardVector() * UseInteractiveRange;

	FHitResult HitResult;
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesArray;
	ObjectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel1));
	
	TArray<AActor*> ActorsToNotTargeting;
	ActorsToNotTargeting.Add(this);

	if (UKismetSystemLibrary::SphereTraceSingleForObjects(
		GetWorld(), StartPosition, EndPosition, 20.f, ObjectTypesArray, false
		, ActorsToNotTargeting, EDrawDebugTrace::None, HitResult, true))
	{
		FindDroppedActor = HitResult.GetActor();
	}
}

void APlayerCharacter::MoveTo(const FInputActionValue& Value)
{
	if (SurvivalComponent->GetIsDied()) return;
	
	const FVector VectorValue = Value.Get<FVector>();
	// 바라보는 방향에 상관없이 회전이 Yaw 쪽으로만 이동해야 하기 때문에 Yaw를 가져옴
	// 수영처럼 바라보는 방향이 Z축으로도 필요하다면 로직이 달라질 수 있음
	const FRotator Rotator = FRotator(0, GetController()->GetControlRotation().Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(Rotator).GetUnitAxis(EAxis::X)
	// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	* VectorValue.X;
	
	const FVector RightDirection = FRotationMatrix(Rotator).GetUnitAxis(EAxis::Y)
	// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	* VectorValue.Y;

	FVector FinalValue = FVector(ForwardDirection + RightDirection);
	FinalValue = FinalValue.GetSafeNormal(1);
	
	AddMovementInput(FinalValue);

	FindToUse();
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	// TODO: 이 부분들에 대해서 UI 관련 회전 Lock을 거는 공통 변수를
	// 하나 만드는 것도 좋아보인다. 다만 그렇게 하면 공통 관리가 심해질 수 있어 보임
	if (IsBlockAction()) return;
	
	const FVector VectorValue = Value.Get<FVector>();

	// Yaw가 가로 회전이기 때문에 X값을 넣어줌
	AddControllerYawInput(VectorValue.X);
	// Pitch는 앞 뒤가 아닌 위 아래 회전이기 때문에 Y값을 넣어줌
	AddControllerPitchInput(VectorValue.Y * - 0.5);
	
	FindToUse();
}

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Display, TEXT("Player Damaged Value: %f"), DamageAmount);
	SurvivalComponent->AddDamage(static_cast<uint8>(DamageAmount));
	return DamageAmount;
}

// 특정 상황에서만 사용해야 함.
bool APlayerCharacter::IsBlockAction() const
{
	return SurvivalComponent->GetIsDied() || InventoryComponent->GetIsOpenInventory();
}

//희연
ARaft* APlayerCharacter::GetRaft()
{
	FVector start = GetActorLocation();
	FVector end = start - FVector(0, 0, 50.0f);
	
	
	return Raft;
}

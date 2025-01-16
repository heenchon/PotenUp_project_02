#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/SurvivalComponent.h"
#include "Component/InventoryComponent.h"
#include "Component/SwimmingComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "project_02/HY/Trash/Trash.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Tool/HookRope.h"
#include "project_02/HY/Paddle/PaddleTest.h"
#include "project_02/HY/Raft/Sail.h"

APlayerCharacter::APlayerCharacter()
{
	SurvivalComponent = CreateDefaultSubobject<USurvivalComponent>("Survival Component");
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
	SwimmingComponent = CreateDefaultSubobject<USwimmingComponent>("Swimming Component");
	
	ChestBox = CreateDefaultSubobject<UBoxComponent>("Chest Box");
	ChestBox->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "ChestSocket");
	
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
		, this, &ThisClass::GoToUp);
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::OnInteractiveHolding);
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Completed
		, this, &ThisClass::OnInteractiveEnd);
		EnhancedInputComponent->BindAction(UseInputAction, ETriggerEvent::Triggered
		, this, &ThisClass::UseItem);
		//희연
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Started
		, this, &ThisClass::OnInteractivePressed);
	}
}

void APlayerCharacter::UseItem()
{
	// if (FindDroppedActor)
	// {
	// 	UE_LOG(LogTemp, Display, TEXT("Using Item: %s"), *FindDroppedActor->GetName());
	// }
	
	ABasePlayerState* PS = static_cast<ABasePlayerState*>(GetPlayerState());
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ATrash::StaticClass()))
	{
		ATrash* Trash = static_cast<ATrash*>(FindDroppedActor);
		
		const uint32 RemainValue = PS->AddItem(Trash->GetItemMetaInfo());
		Trash->UpdateItemInfo(RemainValue);
	}
	//희연
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ASail::StaticClass()))
	{
		ASail* Sail = static_cast<ASail*>(FindDroppedActor);
		
		Sail->SailToggle();
	}
}

//희연: 마우스 눌렀을 때 감지
void APlayerCharacter::OnInteractivePressed()
{
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ASail::StaticClass()))
	{
		ASail* Sail = static_cast<ASail*>(FindDroppedActor);
		Sail->RotateInit(GetControlRotation().Yaw);
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
	//희연
	if (TestInteractiveItem && TestInteractiveItem.IsA(APaddleTest::StaticClass()))
	{
		static_cast<APaddleTest*>(TestInteractiveItem)->PaddlingStart();
	}
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ASail::StaticClass()))
	{
		ASail* Sail = static_cast<ASail*>(FindDroppedActor);
		Sail->RotateSail();
	}
}

void APlayerCharacter::OnInteractiveEnd()
{
	if (IsBlockAction()) return;
	if (TestInteractiveItem && TestInteractiveItem.IsA(AHookRope::StaticClass()))
	{
		static_cast<AHookRope*>(TestInteractiveItem)->OnEndInteractive();
	}
	//희연
	if (TestInteractiveItem && TestInteractiveItem.IsA(APaddleTest::StaticClass()))
	{
		static_cast<APaddleTest*>(TestInteractiveItem)->PaddlingEnd();
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
	FRotator MoveToRotator;
	if (SwimmingComponent->GetIsSwimMode())
	{
		MoveToRotator = GetController()->GetControlRotation();
		
		const FVector ForwardDirection = FRotationMatrix(MoveToRotator).GetUnitAxis(EAxis::X) * VectorValue.X;

		// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
		FVector FinalValue = ForwardDirection.GetSafeNormal(1);
		
		// 이동하는 방향에 대한 처리로 "올라가려는" 높이 방향이 수위 높이 좌표에 내 몸 높이의 일정 Percent 만큼
		// (몸의 지름을 Capsule을 기준으로 한다)에 따라서 더 올라갈지 유지할지를 결정해준다.
		FinalValue.Z = SwimmingComponent->IsOwnerNearWaterLevel(FinalValue) ? 0 : FinalValue.Z;
		AddMovementInput(FinalValue);
	} else
	{
		// 바라보는 방향에 상관없이 회전이 Yaw 쪽으로만 이동해야 하기 때문에 Yaw를 가져옴
		// 수영처럼 바라보는 방향이 Z축으로도 필요하다면 로직이 달라질 수 있음
		MoveToRotator = FRotator(0, GetController()->GetControlRotation().Yaw, 0);
		
		const FVector ForwardDirection = FRotationMatrix(MoveToRotator).GetUnitAxis(EAxis::X) * VectorValue.X;
	    const FVector RightDirection = FRotationMatrix(MoveToRotator).GetUnitAxis(EAxis::Y) * VectorValue.Y;

	    FVector FinalValue = FVector(ForwardDirection + RightDirection);
		// 방향과 대각선으로 이동 시 더 빠르게 이동하는 것을 방지하기 위해 추가 수식으로 이동 속도를 줄임
	    FinalValue = FinalValue.GetSafeNormal(1);

	    AddMovementInput(FinalValue);
	}
	
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

void APlayerCharacter::GoToUp(const FInputActionValue& Value)
{
	if (SwimmingComponent->GetIsSwimMode())
	{
		// 보통 Normal Vector를 Params로 넣는데 지금은 올라가는 경우기에 하드코딩으로 값을 넣는다.
		if (SwimmingComponent->IsOwnerNearWaterLevel(GetActorUpVector()))
		{
			SwimmingComponent->PlayDiving();
		} else
		{
			AddMovementInput(GetActorUpVector());
		}
	} else
	{
		Jump();
	}
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

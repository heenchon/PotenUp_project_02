#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Component/SurvivalComponent.h"
#include "Component/InventoryComponent.h"
#include "Component/SwimmingComponent.h"
#include "Components/BoxComponent.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "project_02/HY/Trash/Trash.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Tool/HookRope.h"
#include "project_02/HY/Paddle/PaddleTest.h"
#include "project_02/HY/Raft/Raft.h"
#include "project_02/HY/Raft/Sail.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/HY/Objects/PlaceObjects.h"
//TODO: 상민띠가 아이템 클래스 만들면 교체 
#include "project_02/HY/Items/Usable_Item.h"

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
		
		EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Started
		, this, &ThisClass::OnInteractivePressed);
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
	
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ASail::StaticClass()))
	{
		ASail* Sail = static_cast<ASail*>(FindDroppedActor);
		
		Sail->SailToggle();
	}

	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(APlaceObjects::StaticClass()))
	{
		APlaceObjects* PlaceObject = static_cast<APlaceObjects*>(FindDroppedActor);
		//TODO: 상민띠가 아이템 클래스 만들면 교체
		if (TestInteractiveItem && TestInteractiveItem.IsA(AUsable_Item::StaticClass()))
		{
			AUsable_Item* Item = static_cast<AUsable_Item*>(TestInteractiveItem);
			PlaceObject->Interact(Item);
		}
	}

	// UI 후처리
	ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
	check(PC)
	PC->GetPlayerUI()->SetInteractiveUIStatus(FindDroppedActor);
}

// 특정 아이템을 손에 들거나 내려놓게 하는 함수
void APlayerCharacter::SetViewItemOnHand(const TSubclassOf<AActor>& NewActorClass)
{
	if (MainHandTool)
	{
		MainHandTool->Destroy();
	}
	
	if (NewActorClass)
	{
		MainHandTool = GetWorld()->SpawnActor<AActor>(NewActorClass);
		
		if (MainHandTool)
		{
			MainHandTool->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::KeepRelativeTransform, "InteractiveSocket");
			MainHandTool->SetOwner(this);
		}
	}
}

void APlayerCharacter::OnInteractivePressed()
{
	// 이 방식으로 통일
	if (IsValid(MainHandTool) && MainHandTool.IsA(AInteractiveItem::StaticClass()))
	{
		UE_LOG(LogTemp, Display, TEXT("Test Result: %s"), *MainHandTool->GetName());
		AInteractiveItem* InteractiveItem = static_cast<AInteractiveItem*>(MainHandTool);
		InteractiveItem->StartInteractive();
	}

	// TODO: 레거시 코드
	if (IsValid(FindDroppedActor) && FindDroppedActor.IsA(ASail::StaticClass()))
	{
		ASail* Sail = static_cast<ASail*>(FindDroppedActor);
		Sail->RotateInit(GetControlRotation().Yaw);
		IsInteracting = true;
	}
	if (TestInteractiveItem && TestInteractiveItem.IsA(AUsable_Item::StaticClass()))
	{
		static_cast<AUsable_Item*>(TestInteractiveItem)->Use();
	}
}

// TODO: 레거시 코드로 제거 필요
void APlayerCharacter::OnInteractiveHolding()
{
	if (IsBlockAction()) return;
	
	// TODO: 우선순위에 대한 로직 추가 필요
	// 여기서부터 아래까지는 보통 상호작용에 대한 처리이기 때문에 우선순위가 매우 높다.
	// 상호작용에 대해서는 HoldEnd에 대해서도 처리하지 않는 것이 원칙. 추후 컴포넌트화 필요
	if (IsInteracting)
	{
		if (ASail* Sail = Cast<ASail>(FindDroppedActor))
		{
			Sail->RotateSail();
		}
		return;
	}
	
	// 손에든 아이템을 실행시키는 방식임
	if (MainHandTool && MainHandTool.IsA(APaddleTest::StaticClass()))
	{
		static_cast<APaddleTest*>(MainHandTool)->PaddlingStart();
	}
	
	if (MainHandTool && MainHandTool.IsA(AHookRope::StaticClass()))
	{
		static_cast<AHookRope*>(MainHandTool)->OnHoldInteractive();
	}
	
}

void APlayerCharacter::OnInteractiveEnd()
{
	if (IsBlockAction()) return;
	if (IsInteracting)
	{
		IsInteracting = false;
		return;
	}
	
	IsInteracting = false;
	
	if (MainHandTool && MainHandTool.IsA(AHookRope::StaticClass()))
	{
		static_cast<AHookRope*>(MainHandTool)->OnEndInteractive();
	}

	if (MainHandTool && MainHandTool.IsA(APaddleTest::StaticClass()))
	{
		static_cast<APaddleTest*>(MainHandTool)->PaddlingEnd();
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
		// Raft가 하위 액터들에 인해 Trace Channel이 자동으로 자식을 따라가는 것으로 추정됨
		// 하드코딩으로 현재는 무시 처리한다.
		// TODO: 원인 분석 후 해결하는 것을 권장함.
		FindDroppedActor = HitResult.GetActor()->IsA(
			ARaft::StaticClass()) ? nullptr : HitResult.GetActor();
	} else
	{
		FindDroppedActor = nullptr;
	}
	
	ABasePlayerController* PC = Cast<ABasePlayerController>(GetController());
	check(PC)
	PC->GetPlayerUI()->SetInteractiveUIStatus(FindDroppedActor);
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

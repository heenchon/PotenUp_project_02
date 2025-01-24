#include "BuildingComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/InventoryComponent.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Building/BuildingWall.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/DataTable/BuildData.h"
#include "project_02/HY/Objects/PlaceObjects.h"
#include "project_02/Player/BasePlayerState.h"

UBuildingComponent::UBuildingComponent()
{
	
}

void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(Player->InputComponent))
		{
			EnhancedInputComponent->BindAction(BuildChangeAction, ETriggerEvent::Triggered
			, this, &ThisClass::ChangeNextBuildAction);
			EnhancedInputComponent->BindAction(InteractiveInputAction, ETriggerEvent::Triggered
			, this, &ThisClass::BuildCustomObject);
		}
	}
}

void UBuildingComponent::TraceGroundToBuild(const FVector& TraceTo)
{
	if (!CanBuild)
	{
		return;
	}
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());

	FHitResult HitResult;
	
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorLocation() + TraceTo * TraceRange,
		GetCheckTraceChannel(),
		false, IgnoreActors,
		EDrawDebugTrace::Type::ForOneFrame,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1
	))
	{
		CurrentHitActor = HitResult.GetActor();
		if (HitResult.GetActor()->IsA(ABuildingActor::StaticClass()))
		{
			// 기존 컴포넌트와 동일한 경우는 그냥 처리하지 않음
			if (HitResult.GetComponent() == CurrentWireframeBox)
			{
				return;
			}

			// 바닥을 바라보고 있고, 그리고 그 중 Static Mesh 즉 바닥 Mesh를 바라보고 있는 경우에
			// 오브젝트 설치에 대한 최소한의 조건을 만족시킨다. 그리고 이 조건은 반드시 오브젝트 설치인 경우다.
			if (HitResult.GetComponent()->IsA(UStaticMeshComponent::StaticClass())
				&& HitResult.GetActor()->IsA(ABuildingFloor::StaticClass())
				&& FrameType == EBuildType::Object)
			{
				CreateWireframeForObject(HitResult);
				return;
			}
			
			if (HitResult.GetComponent()->IsA(UBoxComponent::StaticClass()))
			{
				CreateWireframeForGrid(HitResult);
				return;
			}
			
			ClearWireframe();
		} else
		{
			// 다른 액터를 바라보는 경우
			ClearWireframe();
		}
	} else
	{
		// 여기는 Trace시 결과가 없는 경우 즉, 바라보는 곳이 허공이기 때문에 Wireframe을 날려야 하는 경우
		ClearWireframe();
	}
}

void UBuildingComponent::CreateWireframeForGrid(const FHitResult& HitResult)
{
	CurrentWireframeBox = HitResult.GetComponent();

	// 임시용 주석
	if (!WireframeToFloorClass || !WireframeToWallClass)
	{
		return;
	}

	if (CurrentWireframeActor && CurrentWireframeActor.IsA(ABuildingActor::StaticClass()))
	{
		switch (FrameType)
		{
		case EBuildType::Floor:
			{
				ReattachFloor(HitResult);
				break;
			}
		case EBuildType::Wall:
			{
				ReattachWall(HitResult);
				break;
			}
		default:
			{
				break;
			}
		}
	} else
	{
		// 없으면 새로 만든다. 바닥의 케이스
		switch (FrameType)
		{
			case EBuildType::Floor:
			{
				SpawnFrameFloor(HitResult);
				break;
			}
			case EBuildType::Wall:
			{
				SpawnFrameWall(HitResult);
				break;
			}
			default:
			{
				break;
			}
		}
	}
}

void UBuildingComponent::CreateWireframeForObject(const FHitResult& HitResult)
{
	if (!CustomBuildItemClass)
	{
		return;
	}

	// 조건은 총 2가지다.
	// 1. 현재 Wireframe인 액터가 존재하지 않는다.
	// 2. 현재 선택중인 커스텀 오브젝트의 타입이 아니다.
	// 이 중 하나라도 만족하면 기존의 Actor를 없애고 새로 만든다.
	
	if (
		!CurrentWireframeActor
		|| !CurrentWireframeActor.IsA(CustomBuildItemClass)
	)
	{
		if (CurrentWireframeActor)
		{
			CurrentWireframeActor->Destroy();
		}
		
		CurrentWireframeActor = GetWorld()->SpawnActor<AActor>(CustomBuildItemClass, HitResult.ImpactPoint, FRotator::ZeroRotator);
		CurrentWireframeActor->AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
		
		if (APlaceObjects* PlaceObject = Cast<APlaceObjects>(CurrentWireframeActor))
		{
			PlaceObject->IsEnabled = false;
		}
		return;
	}
	
	CurrentWireframeActor->SetHidden(false);
	CurrentWireframeActor->SetActorLocation(HitResult.ImpactPoint);
	if (APlaceObjects* PlaceObject = Cast<APlaceObjects>(CurrentWireframeActor))
	{
		PlaceObject->SetWireframeMaterial(PlaceObject->CanBuild ? WireframeMaterial : WireframeBlockMaterial);
	}
}

void UBuildingComponent::ReattachFloor(const FHitResult& HitResult)
{
	if (ABuildingFloor* FloorWireframe = Cast<ABuildingFloor>(CurrentWireframeActor))
	{
		FloorWireframe->SetActorHiddenInGame(false);
		FloorWireframe->AttachToComponent(
				HitResult.GetComponent(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		FloorWireframe->SetWireframeMaterial(CanBuildBlockBuild(FloorWireframe) ? WireframeMaterial : WireframeBlockMaterial);
	}
}

void UBuildingComponent::ReattachWall(const FHitResult& HitResult)
{
	if (ABuildingWall* WallWireframe = Cast<ABuildingWall>(CurrentWireframeActor))
	{
		WallWireframe->SetActorHiddenInGame(false);
		WallWireframe->AttachToComponent(
				HitResult.GetComponent(),
				FAttachmentTransformRules::KeepRelativeTransform);
		WallWireframe->SetWireframeMaterial(CanBuildBlockBuild(WallWireframe) ? WireframeMaterial : WireframeBlockMaterial);
	}
}

void UBuildingComponent::SpawnFrameFloor(const FHitResult& HitResult)
{
	if (ABuildingFloor* NewWireframe = GetWorld()->SpawnActor<ABuildingFloor>(WireframeToFloorClass,
			HitResult.GetComponent()->GetComponentLocation(),
			HitResult.GetComponent()->GetComponentRotation()))
	{
		// 우선 값은 바로 할당하기
		NewWireframe->SetWireframe(true);
		NewWireframe->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		NewWireframe->SetWireframeMaterial(
			CanBuildBlockBuild(NewWireframe) ?
			WireframeMaterial : WireframeBlockMaterial);
		// 상위 함수에서 이미 검증하지만 혹시 모르니 재검증
		if (const ABuildingActor* ParentBuild = Cast<ABuildingActor>(HitResult.GetActor()))
		{
			NewWireframe->SetMainBuild(ParentBuild->GetMainBuild());
		}
		
		CurrentWireframeActor = NewWireframe;
	}
}

void UBuildingComponent::SpawnFrameWall(const FHitResult& HitResult)
{
	FVector NewLocation = HitResult.GetComponent()->GetComponentLocation();
	// TODO: 테스트용 하드코딩으로 추후 제거 필요 -> 설치 컴포넌트 위치를 Scene으로 이전
	NewLocation.Z += 70;
	FRotator NewRotation = HitResult.GetComponent()->GetComponentRotation();
	NewRotation.Yaw += 90;
	
	if (ABuildingWall* NewWireframe = GetWorld()->SpawnActor<ABuildingWall>(WireframeToWallClass, NewLocation, NewRotation))
	{
		// 우선 값은 바로 할당하기
		NewWireframe->SetWireframe(true);
		NewWireframe->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::KeepWorldTransform);
		NewWireframe->SetWireframeMaterial(
			CanBuildBlockBuild(NewWireframe) ?
			WireframeMaterial : WireframeBlockMaterial);
		// 상위 함수에서 이미 검증하지만 혹시 모르니 재검증
		if (const ABuildingActor* ParentBuild = Cast<ABuildingActor>(HitResult.GetActor()))
		{
			NewWireframe->SetMainBuild(ParentBuild->GetMainBuild());
		}
		
		CurrentWireframeActor = NewWireframe;
	}
}

void UBuildingComponent::ClearWireframe()
{
	if (CurrentWireframeActor)
	{
		CurrentWireframeActor->SetActorHiddenInGame(true);
	}
	CurrentWireframeBox = nullptr;
}

void UBuildingComponent::DeleteWireframe()
{
	if (CurrentWireframeActor)
	{
		CurrentWireframeActor->Destroy();
		CurrentWireframeActor = nullptr;
	}
	CurrentWireframeBox = nullptr;
	CurrentHitActor = nullptr;
}

void UBuildingComponent::BuildWireframe()
{
	if (!CurrentWireframeActor)
	{
		return;	
	}

	// 단순 설치류 아이템이면 여기서 설치한다.
	if (APlaceObjects* PlaceObject = Cast<APlaceObjects>(CurrentWireframeActor))
	{
		if (!PlaceObject->CanBuild)
		{
			return;
		}

		// 내가 손에 들고 있는 것을 대상으로 설치되기 때문에 하나를 버린다.
		// 이 Component가 플레이어 대상으로 진행되기에 플레이어를 넣어둔다.
		if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
		{
			if (ABasePlayerState* PS = Cast<ABasePlayerState>(PlayerCharacter->GetPlayerState()))
			{
				PS->DropItem(PlayerCharacter->GetInventoryComponent()->GetSelectedHotSlotIndex(), 1);
				SetBuildMode(false);
			}
		}

		PlaceObject->SetDefaultMaterial();
		PlaceObject->IsEnabled = true;
		
		CurrentWireframeActor = nullptr;
		CurrentWireframeBox = nullptr;
		CurrentHitActor = nullptr;
		
		return;
	}

	// 설치류가 아닌 건축 블럭이라면 여기를 수행한다.
	if (const ABuildingActor* NewBuildActor = Cast<ABuildingActor>(CurrentWireframeActor))
	{
		if (!CanBuildBlockBuild(NewBuildActor))
		{
			return;
		}
		
		// 여기서는 Meta Data를 업데이트 처리한다.
		if (ABuildingActor* ParentBuild = Cast<ABuildingActor>(CurrentHitActor))
		{
			if (!CurrentWireframeBox)
			{
				return;
			}

			if (const APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
			{
				ABasePlayerState* PS = Cast<ABasePlayerState>(PlayerCharacter->GetPlayerState());
				
				if (!PS)
				{
					return;
				}
				
				if (PS->RemoveItem(GetBuildBlockType(NewBuildActor), NewBuildActor->GetConsumeCount()))
				{
					ParentBuild->UpdateBuildData(CurrentWireframeBox, Cast<ABuildingActor>(CurrentWireframeActor));

					CurrentWireframeActor = nullptr;
					CurrentWireframeBox = nullptr;
					CurrentHitActor = nullptr;
				}
			}
		}
	}
}

ETraceTypeQuery UBuildingComponent::GetCheckTraceChannel() const
{
	switch (FrameType)
	{
		case EBuildType::Wall:
			return TraceTypeQuery5;
		case EBuildType::Floor:
		case EBuildType::Object:
		default:
			return TraceTypeQuery4;
	}
}

void UBuildingComponent::ChangeNextBuildAction()
{
	if (!CanBuild)
	{
		return;
	}

	// 커스텀 오브젝트인 경우는 상태가 변경되지 않는다.
	if (FrameType == EBuildType::Object)
	{
		return;
	}

	// 변경 값에 관계 없이 초기화 과정은 거쳐준다.
	DeleteWireframe();
	
	if (FrameType == EBuildType::Floor)
	{
		FrameType = EBuildType::Wall;
	} else
	{
		FrameType = EBuildType::Floor;
	}
}

void UBuildingComponent::SetBuildType(const EBuildType NewType)
{
	DeleteWireframe();
	FrameType = NewType;
}

void UBuildingComponent::BuildCustomObject()
{
	BuildWireframe();
}

uint32 UBuildingComponent::GetBuildBlockType(const ABuildingActor* BuildingActor)
{
	if (BuildingActor->GetBlockType() == EBlockType::Wood)
	{
		// 나뭇조각 id
		// TODO: 하드코딩을 방지할 수 있는 방법을 탐색해야 한다.
		return 5;
	}
	return 0;
}

bool UBuildingComponent::CanBuildBlockBuild(const ABuildingActor* BuildingActor)
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		const ABasePlayerState* PS = Player->GetPlayerState<ABasePlayerState>();
		if (!PS)
		{
			return false;	
		}

		// 전체 아이템 정보 에서 내가 원하는 아이템 타입 정보를 가져온다.
		if (const uint32* Result = PS->GetCurrentRemainItemValue().Find(GetBuildBlockType(BuildingActor)))
		{
			UE_LOG(LogTemp, Display, TEXT("Result: %d"), *Result)
			if (*Result >= BuildingActor->GetConsumeCount())
			{
				return true;
			}
		}
	}
	
	return false;
}

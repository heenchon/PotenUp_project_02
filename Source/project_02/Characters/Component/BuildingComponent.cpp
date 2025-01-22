#include "BuildingComponent.h"

#include "EnhancedInputComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Building/BuildingWall.h"
#include "project_02/Characters/PlayerCharacter.h"


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
		// Custom Channel인 4번으로 체크
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
			
			CreateWireframeForGrid(HitResult);
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
	if (!HitResult.GetComponent()->IsA(UBoxComponent::StaticClass()))
	{
		ClearWireframe();
		return;
	}
	
	CurrentWireframeBox = HitResult.GetComponent();

	if (!WireframeToFloorClass)
	{
		return;
	}

	if (CurrentWireframeActor)
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

void UBuildingComponent::ReattachFloor(const FHitResult& HitResult)
{
	CurrentWireframeActor->SetActorHiddenInGame(false);
	CurrentWireframeActor->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);
}

void UBuildingComponent::ReattachWall(const FHitResult& HitResult)
{
	CurrentWireframeActor->SetActorHiddenInGame(false);
	CurrentWireframeActor->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::KeepRelativeTransform);
	CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);
}


void UBuildingComponent::SpawnFrameFloor(const FHitResult& HitResult)
{
	if (ABuildingFloor* NewWireframe = GetWorld()->SpawnActor<ABuildingFloor>(WireframeToFloorClass,
			HitResult.GetComponent()->GetComponentLocation(),
			HitResult.GetComponent()->GetComponentRotation()))
	{
		// 우선 값은 바로 할당하기
		CurrentWireframeActor = NewWireframe;
		CurrentWireframeActor->SetWireframe(true);
		CurrentWireframeActor->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);

		// 상위 함수에서 이미 검증하지만 혹시 모르니 재검증
		if (const ABuildingActor* ParentBuild = Cast<ABuildingActor>(HitResult.GetActor()))
		{
			CurrentWireframeActor->SetMainBuild(ParentBuild->GetMainBuild());
		}
	}
}

void UBuildingComponent::SpawnFrameWall(const FHitResult& HitResult)
{
	FVector NewLocation = HitResult.GetComponent()->GetComponentLocation();
	// TODO: 테스트용 하드코딩으로 추후 제거 필요
	NewLocation.Z += 50;
	FRotator NewRotation = HitResult.GetComponent()->GetComponentRotation();
	NewRotation.Yaw += 90;
	
	if (ABuildingWall* NewWireframe = GetWorld()->SpawnActor<ABuildingWall>(WireframeToWallClass, NewLocation, NewRotation))
	{
		// 우선 값은 바로 할당하기
		CurrentWireframeActor = NewWireframe;
		CurrentWireframeActor->SetWireframe(true);
		CurrentWireframeActor->AttachToComponent(
			HitResult.GetComponent(),
			FAttachmentTransformRules::KeepWorldTransform);
		CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);

		// 상위 함수에서 이미 검증하지만 혹시 모르니 재검증
		if (const ABuildingActor* ParentBuild = Cast<ABuildingActor>(HitResult.GetActor()))
		{
			CurrentWireframeActor->SetMainBuild(ParentBuild->GetMainBuild());
		}
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
	
	if (!CurrentWireframeBox)
	{
		return;
	}

	// 여기서는 Meta Data를 업데이트 처리한다.
	if (ABuildingActor* ParentBuild = Cast<ABuildingActor>(CurrentHitActor))
	{
		ParentBuild->UpdateBuildData(CurrentWireframeBox, CurrentWireframeActor);
	}

	CurrentWireframeActor = nullptr;
	CurrentWireframeBox = nullptr;
	CurrentHitActor = nullptr;
}

ETraceTypeQuery UBuildingComponent::GetCheckTraceChannel() const
{
	switch (FrameType)
	{
		case EBuildType::Wall:
			return TraceTypeQuery5;
		case EBuildType::Floor:
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


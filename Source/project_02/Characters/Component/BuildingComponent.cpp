#include "BuildingComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/DataTable/BuildData.h"


UBuildingComponent::UBuildingComponent()
{
}

void UBuildingComponent::BeginPlay()
{
	Super::BeginPlay();
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
		TraceTypeQuery4,
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
			
			if (WireframeToBuildClass->IsChildOf(ABuildingFloor::StaticClass()))
			{
				CreateWireframeForGrid(HitResult);
			}
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

	if (!WireframeToBuildClass)
	{
		return;
	}

	if (CurrentWireframeActor)
	{
		// 이미 부착하려는 액터가 존재하면 다시 나타나게 처리함
		CurrentWireframeActor->SetActorHiddenInGame(false);
		CurrentWireframeActor->AttachToComponent(
				HitResult.GetComponent(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);
	} else
	{
		// 없으면 새로 만든다.
		if (ABuildingActor* NewWireframe = GetWorld()->SpawnActor<ABuildingActor>(WireframeToBuildClass,
			HitResult.GetComponent()->GetComponentLocation(),
			HitResult.GetComponent()->GetComponentRotation()))
		{
			NewWireframe->SetWireframe(true);
			CurrentWireframeActor = NewWireframe;
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
}

void UBuildingComponent::ClearWireframe()
{
	if (CurrentWireframeActor)
	{
		CurrentWireframeActor->SetActorHiddenInGame(true);
	}
	CurrentWireframeBox = nullptr;
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
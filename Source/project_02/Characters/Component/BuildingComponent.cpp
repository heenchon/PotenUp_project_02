#include "BuildingComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Characters/PlayerCharacter.h"


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
		CurrentWireframeActor->SetActorHiddenInGame(false);
		CurrentWireframeActor->AttachToComponent(
				HitResult.GetComponent(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);
	} else
	{
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
	
	CurrentWireframeActor->SetWireframe(false);
	CurrentWireframeActor->SetDefaultMaterial();
	CurrentWireframeActor = nullptr;
	CurrentWireframeBox = nullptr;
}
#include "BuildingComponent.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"


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
		// Visibility로 체크
		TraceTypeQuery1,
		false, IgnoreActors,
		EDrawDebugTrace::Type::None,
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
			
			// Wireframe을 위한 Box Collision일 경우
			if (HitResult.GetComponent()->IsA(UBoxComponent::StaticClass())) {
				CurrentWireframeBox = HitResult.GetComponent();

				if (!BuildWireframe)
				{
					return;
				}
				
				if (ABuildingActor* NewWireframe = GetWorld()->SpawnActor<ABuildingActor>(BuildWireframe,
					HitResult.GetComponent()->GetComponentLocation(),
					HitResult.GetComponent()->GetComponentRotation()))
				{
					// TODO: 이런게 많이 생길 것 같으면 Object Pooling을 해라...
					NewWireframe->SetWireframe(true);
					if (CurrentWireframeActor)
					{
						CurrentWireframeActor->Destroy();
					}
					CurrentWireframeActor = NewWireframe;
					CurrentWireframeActor->AttachToComponent(
						HitResult.GetComponent(),
						FAttachmentTransformRules::SnapToTargetNotIncludingScale);
					CurrentWireframeActor->SetWireframeMaterial(WireframeMaterial);
				}
			}
		} else
		{
			// 다른 액터를 바라보는 경우
			ClearBuildWireframe();
		}
	} else
	{
		// 여기는 Trace시 결과가 없는 경우 즉, 바라보는 곳이 허공이기 때문에 Wireframe을 날려야 하는 경우
		ClearBuildWireframe();
	}
}

void UBuildingComponent::ClearBuildWireframe()
{
	if (CurrentWireframeActor)
	{
		CurrentWireframeActor->Destroy();
	}
	if (CurrentWireframeBox)
	{
		CurrentWireframeBox = nullptr;
	}
}



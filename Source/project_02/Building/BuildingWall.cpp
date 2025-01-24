#include "BuildingWall.h"

#include "BuildingFloor.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/HY/RaftGameState.h"


ABuildingWall::ABuildingWall()
{
	ForwardFloorBodyBox = CreateDefaultSubobject<UBoxComponent>("Forward Floor Body Box");
	ForwardFloorBodyBox->SetupAttachment(GetRootComponent());
	ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ForwardFloorBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	ForwardFloorBodyBox->SetRelativeLocation({0, 88.408905, 87.5});
	ForwardFloorBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	BackwardFloorBodyBox = CreateDefaultSubobject<UBoxComponent>("Backward Floor Body Box");
	BackwardFloorBodyBox->SetupAttachment(GetRootComponent());
	BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	BackwardFloorBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	BackwardFloorBodyBox->SetRelativeLocation({0, -90.929366, 87.5});
	BackwardFloorBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
}

void ABuildingWall::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildingWall::OnWireframeActive()
{
	Super::OnWireframeActive();
	ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingWall::OnWireframeInactive()
{
	Super::OnWireframeInactive();
	
	ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void ABuildingWall::UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild)
{
	Super::UpdateBuildData(TargetComp, ChildBuild);

	// 여기서는 설치하면 무조건 바닥만 설치되기 때문에 바닥에 대한 대응만 해야한다.
	if (ARaftGameState* RaftGameState = GetWorld()->GetGameState<ARaftGameState>())
	{
		FVector NewPos = GetBuildPos();
		// 벽은 그냥 한칸 위로 올라갔다고 가정하자
		NewPos.Z += 1;
		// 현재 기준으로 좌, 우를 바라보는지, 상, 하를 바라보는지는 어떻게 계산할 것인가?
		// TODO: 정보 저장에 필요한 데이터 여기에 잘 넣어두기
		if (TargetComp == ForwardFloorBodyBox)
		{
			ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			if (UKismetMathLibrary::Fraction(NewPos.X) != 0)
			{
				NewPos.X += 0.5;
			}
			if (UKismetMathLibrary::Fraction(NewPos.Y) != 0)
			{
				NewPos.Y += 0.5;
			}
		}
		if (TargetComp == BackwardFloorBodyBox)
		{
			BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			if (UKismetMathLibrary::Fraction(NewPos.X) != 0)
			{
				NewPos.X -= 0.5;
			}
			if (UKismetMathLibrary::Fraction(NewPos.Y) != 0)
			{
				NewPos.Y -= 0.5;
			}
		}
		UE_LOG(LogTemp, Display, TEXT("벽 기준으로 바닥 설치 완료: 좌표값: %s"), *NewPos.ToString())
		ChildBuild->SetBuildPos(NewPos);
		RaftGameState->UpdateBuildMetaData(NewPos, ChildBuild);

		// 어차피 벽이지만 그래도 재검증은 하는게 정신건강에 좋다.
		if (ABuildingFloor* ChildFloor = Cast<ABuildingFloor>(ChildBuild))
		{
			ChildFloor->UpdateWireframeBoxInfo();
		}
	}
}
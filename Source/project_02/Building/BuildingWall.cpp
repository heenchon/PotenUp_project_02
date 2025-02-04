#include "BuildingWall.h"

#include "BuildingFloor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/Helper/EnumHelper.h"
#include "project_02/HY/Raft/Raft.h"
#include "project_02/Player/BasePlayerController.h"


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
	if (const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PC->GetPlayerRaft())
		{
			return;
		}
		
		FVector NewPos = GetBuildPos();
		// 벽은 그냥 한칸 위로 올라갔다고 가정하자
		NewPos.Z += 1;
		// 현재 기준으로 좌, 우를 바라보는지, 상, 하를 바라보는지는 어떻게 계산할 것인가?
		if (TargetComp == ForwardFloorBodyBox)
		{
			ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			// NewPos의 각 X, Y의 소수 자리만 Fraction으로 추출함
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
		PC->GetPlayerRaft()->UpdateBuildMetaData(NewPos, ChildBuild);

		// 어차피 벽이지만 그래도 재검증은 하는게 정신건강에 좋다.
		if (ABuildingFloor* ChildFloor = Cast<ABuildingFloor>(ChildBuild))
		{
			ChildFloor->UpdateWireframeBoxInfo();
		}
	}
}

void ABuildingWall::UpdateWireframeBoxInfo()
{
	if (const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PC->GetPlayerRaft())
		{
			return;
		}

		constexpr float MoveTo[2] = { 0.5, -0.5 };
		constexpr EBlockPos MovePos[2] = { EBlockPos::North, EBlockPos::South };
		// 벽은 그냥 한칸 위로 올라갔다고 가정하자
		// 현재 X가 소수점이라는 의미는 해당 벽이 앞이나 뒤에 있다라는 것 이다.
		if (UKismetMathLibrary::Fraction(GetBuildPos().X) != 0)
		{
			for (int i = 0; i < 2; i++)
			{
				FVector NewPos = GetBuildPos();
				NewPos.Z += 1;
				NewPos.X += MoveTo[i];
				// 앞 뒤로 각각 객체가 있는지 검증하고 있으면 collision 비활성화 처리
				if (PC->GetPlayerRaft()->GetRaftBuildPointerData().FindRef(NewPos))
				{
					if (MovePos[i] == EBlockPos::North)
					{
						ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					} else
					{
						BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					}
				}
			}
		}
		// 현재 Y가 소수점이라는 의미는 해당 벽이 왼쪽 혹은 오른쪽에 있다라는 것 이다.
		if (UKismetMathLibrary::Fraction(GetBuildPos().Y) != 0)
		{
			for (int i = 0; i < 2; i++)
			{
				FVector NewPos = GetBuildPos();
				NewPos.Z += 1;
				NewPos.Y += MoveTo[i];
				// 앞 뒤로 각각 객체가 있는지 검증하고 있으면 collision 비활성화 처리
				if (PC->GetPlayerRaft()->GetRaftBuildPointerData().FindRef(NewPos))
				{
					UE_LOG(LogTemp, Display, TEXT("발견 %s: %s"), *GetBuildPos().ToString(), *FEnumHelper::GetClassEnumKeyAsString(MovePos[i]));
					if (MovePos[i] == EBlockPos::North)
					{
						ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					} else
					{
						BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
					}
				}
			}
		}
	}
}

TObjectPtr<UBoxComponent> ABuildingWall::GetBoxByDirection(
	const EBlockPos Direction, const bool IsReverse) const
{
	if (Direction == EBlockPos::North)
	{
		return IsReverse ? BackwardFloorBodyBox : ForwardFloorBodyBox;
	}
	return IsReverse ? ForwardFloorBodyBox : BackwardFloorBodyBox;
}

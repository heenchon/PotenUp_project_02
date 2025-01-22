#include "BuildingFloor.h"

#include "Components/BoxComponent.h"
#include "project_02/DataTable/BuildData.h"
#include "project_02/Helper/EnumHelper.h"
#include "project_02/HY/RaftGameState.h"

// Sets default values
ABuildingFloor::ABuildingFloor()
{
	RightBodyBox = CreateDefaultSubobject<UBoxComponent>("Right Body Box");
	RightBodyBox->SetupAttachment(GetRootComponent());
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	RightBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	RightBodyBox->SetRelativeLocation({0, 177, 0});
	RightBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	LeftBodyBox = CreateDefaultSubobject<UBoxComponent>("Left Body Box");
	LeftBodyBox->SetupAttachment(GetRootComponent());
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	LeftBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	LeftBodyBox->SetRelativeLocation({0, -177, 0});
	LeftBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	NorthBodyBox = CreateDefaultSubobject<UBoxComponent>("North Body Box");
	NorthBodyBox->SetupAttachment(GetRootComponent());
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	NorthBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	NorthBodyBox->SetRelativeLocation({177, 0, 0});
	NorthBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	SouthBodyBox = CreateDefaultSubobject<UBoxComponent>("South Body Box");
	SouthBodyBox->SetupAttachment(GetRootComponent());
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SouthBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	SouthBodyBox->SetRelativeLocation({-177, 0, 0});
	SouthBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});

	NorthWallBodyBox = CreateDefaultSubobject<UBoxComponent>("North Wall Body Box");
	NorthWallBodyBox->SetupAttachment(GetRootComponent());
	NorthWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	NorthWallBodyBox->SetRelativeLocation({-177, 0, 0});
	NorthWallBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	SouthWallBodyBox = CreateDefaultSubobject<UBoxComponent>("South Wall Body Box");
	SouthWallBodyBox->SetupAttachment(GetRootComponent());
	SouthWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	SouthWallBodyBox->SetRelativeLocation({-177, 0, 0});
	SouthWallBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	EastWallBodyBox = CreateDefaultSubobject<UBoxComponent>("East Wall Body Box");
	EastWallBodyBox->SetupAttachment(GetRootComponent());
	EastWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	EastWallBodyBox->SetRelativeLocation({-177, 0, 0});
	EastWallBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	WestWallBodyBox = CreateDefaultSubobject<UBoxComponent>("West Wall Body Box");
	WestWallBodyBox->SetupAttachment(GetRootComponent());
	WestWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	WestWallBodyBox->SetRelativeLocation({-177, 0, 0});
	WestWallBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
}

void ABuildingFloor::BeginPlay()
{
	Super::BeginPlay();
}

void ABuildingFloor::OnWireframeActive()
{
	Super::OnWireframeActive();
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingFloor::OnWireframeInactive()
{
	Super::OnWireframeInactive();
	
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
}

void ABuildingFloor::UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* Child)
{
	Super::UpdateBuildData(TargetComp, Child);

	if (ARaftGameState* RaftGameState = GetWorld()->GetGameState<ARaftGameState>())
	{
		FVector NewPos = GetBuildPos();
		
		if (TargetComp == RightBodyBox)
		{
			NewPos.Y += 1;
		}
		
		if (TargetComp == LeftBodyBox)
		{
			NewPos.Y -= 1;
		}
		
		if (TargetComp == NorthBodyBox)
		{
			NewPos.X += 1;
		}
		
		if (TargetComp == SouthBodyBox)
		{
			NewPos.X -= 1;
		}
		
		Child->SetBuildPos(NewPos);
		RaftGameState->UpdateBuildMetaData(NewPos, Child);	
		if (ABuildingFloor* ChildFloor = Cast<ABuildingFloor>(Child))
		{
			ChildFloor->UpdateWireframeBoxInfo();
		}
	}
}

void ABuildingFloor::SetCenter()
{
	Super::SetCenter();
	if (ARaftGameState* RaftGameState = GetWorld()->GetGameState<ARaftGameState>())
	{
		RaftGameState->UpdateBuildMetaData(GetBuildPos(), this);
	}
}

void ABuildingFloor::UpdateWireframeBoxInfo()
{
	if (const ARaftGameState* RaftGameState = GetWorld()->GetGameState<ARaftGameState>())
	{
		constexpr int MoveToX[4] = {0, 0, 1, -1};
		constexpr int MoveToY[4] = {1, -1, 0, 0};
		constexpr EBlockPos MoveTo[4] = {EBlockPos::East, EBlockPos::West, EBlockPos::North, EBlockPos::South};
		
		TArray<FVector> SearchVectorList;
		// 자기 자신은 무조건 포함하고 계산해야 한다.
		SearchVectorList.Push(GetBuildPos());

		// 설치한 위치를 기준으로 주변에 같이 검사할 바닥 블록 검사
		for (int i = 0; i < 4; i++)
		{
			FVector TempPos = GetBuildPos();
			TempPos.X += MoveToX[i];
			TempPos.Y += MoveToY[i];

			// 데이터에 존재하면 검색 트리에 추가
			if (RaftGameState->GetRaftBuildMetaData().Find(TempPos))
			{
				SearchVectorList.Add(TempPos);
			}
		}

		// 검색할 노드들 순회
		for (const FVector SearchPos : SearchVectorList)
		{
			UE_LOG(LogTemp, Display, TEXT("주변 탐색: %s"), *SearchPos.ToString())
			for (int i = 0; i < 4; i++)
			{
				FVector TempPos = SearchPos;
				TempPos.X += MoveToX[i];
				TempPos.Y += MoveToY[i];

				// 포인터의 포인터를 찾는 거라 역 포인터로 가져와야 함.
				// TODO: 이렇게 안하는 방법에 대해 생각해볼 필요가 있다.
				ABuildingActor* FoundBuild = RaftGameState->GetRaftBuildPointerData().FindRef(TempPos);
				const bool IsAlreadyBound = IsValid(FoundBuild);
			
				UE_LOG(LogTemp, Display, TEXT("%s 탐색 결과: %d"),
					*FEnumHelper::GetClassEnumKeyAsString(MoveTo[i]), IsAlreadyBound)

				ABuildingActor* SetBuild = RaftGameState->GetRaftBuildPointerData().FindRef(SearchPos);
				if (const ABuildingFloor* SearchBuildFloorResult = Cast<ABuildingFloor>(SetBuild))
				{
					if (MoveTo[i] == EBlockPos::East)
					{
						SearchBuildFloorResult->RightBodyBox->SetCollisionEnabled(
							IsAlreadyBound ?
							ECollisionEnabled::Type::NoCollision :
							ECollisionEnabled::Type::QueryOnly);
					}
					if (MoveTo[i] == EBlockPos::West)
					{
						SearchBuildFloorResult->LeftBodyBox->SetCollisionEnabled(
							IsAlreadyBound ?
							ECollisionEnabled::Type::NoCollision :
							ECollisionEnabled::Type::QueryOnly);
					}
					if (MoveTo[i] == EBlockPos::North)
					{
						SearchBuildFloorResult->NorthBodyBox->SetCollisionEnabled(
							IsAlreadyBound ?
							ECollisionEnabled::Type::NoCollision :
							ECollisionEnabled::Type::QueryOnly);
					}
					if (MoveTo[i] == EBlockPos::South)
					{
						SearchBuildFloorResult->SouthBodyBox->SetCollisionEnabled(
							IsAlreadyBound ?
							ECollisionEnabled::Type::NoCollision :
							ECollisionEnabled::Type::QueryOnly);
					}
				}
			}
		}
	}
}





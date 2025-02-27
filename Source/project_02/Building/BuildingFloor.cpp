﻿#include "BuildingFloor.h"

#include "BuildingWall.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/DataTable/BuildData.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/HY/Raft/Raft.h"

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
	NorthWallBodyBox->SetRelativeLocation({85.526552, 0, 0});
	NorthWallBodyBox->SetRelativeScale3D({0.5, 1.75, 0.5});

	NorthWallSceneVector = CreateDefaultSubobject<USceneComponent>("North Wall Scene Vector");
	NorthWallSceneVector->SetupAttachment(GetRootComponent());
	NorthWallSceneVector->SetRelativeLocation({-85.526552, 0, 120});
	NorthWallSceneVector->SetRelativeRotation({0, -90, 0});
	
	SouthWallBodyBox = CreateDefaultSubobject<UBoxComponent>("South Wall Body Box");
	SouthWallBodyBox->SetupAttachment(GetRootComponent());
	SouthWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	SouthWallBodyBox->SetRelativeLocation({-85.526552, 0, 0});
	SouthWallBodyBox->SetRelativeScale3D({0.5, 1.75, 0.5});

	SouthWallSceneVector = CreateDefaultSubobject<USceneComponent>("South Wall Scene Vector");
	SouthWallSceneVector->SetupAttachment(GetRootComponent());
	SouthWallSceneVector->SetRelativeLocation({-85.526552, 0, 120});
	SouthWallSceneVector->SetRelativeRotation({0, -90, 0});
	
	EastWallBodyBox = CreateDefaultSubobject<UBoxComponent>("East Wall Body Box");
	EastWallBodyBox->SetupAttachment(GetRootComponent());
	EastWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	EastWallBodyBox->SetRelativeLocation({0, 85.526552, 0});
	EastWallBodyBox->SetRelativeScale3D({1.75, 0.5, 0.5});

	EastWallSceneVector = CreateDefaultSubobject<USceneComponent>("East Wall Scene Vector");
	EastWallSceneVector->SetupAttachment(GetRootComponent());
	EastWallSceneVector->SetRelativeLocation({0, -85.526552, 120});
	EastWallSceneVector->SetRelativeRotation({0, 180, 0});
	
	WestWallBodyBox = CreateDefaultSubobject<UBoxComponent>("West Wall Body Box");
	WestWallBodyBox->SetupAttachment(GetRootComponent());
	WestWallBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel3, ECR_Block);
	WestWallBodyBox->SetRelativeLocation({0, -85.526552, 0});
	WestWallBodyBox->SetRelativeScale3D({1.75, 0.5, 0.5});

	WestWallSceneVector = CreateDefaultSubobject<USceneComponent>("West Wall Scene Vector");
	WestWallSceneVector->SetRelativeLocation({0, -85.526552, 120});
	WestWallSceneVector->SetRelativeRotation({0, 180, 0});
	WestWallSceneVector->SetupAttachment(GetRootComponent());

	CrackDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("CrackDecal"));
	CrackDecal->SetupAttachment(RootComponent);
	CrackDecal->SetVisibility(false);
}
void ABuildingFloor::BeginPlay()
{
	Super::BeginPlay();

	WestWallBodyBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWallBodyBeginOverlap);
	EastWallBodyBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWallBodyBeginOverlap);
	NorthWallBodyBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWallBodyBeginOverlap);
	SouthWallBodyBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnWallBodyBeginOverlap);
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

void ABuildingFloor::UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild)
{
	if (const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PC->GetPlayerRaft())
		{
			return;
		}
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

		if (TargetComp == NorthWallBodyBox)
		{
			NewPos.X += 0.5;
		}

		if (TargetComp == SouthWallBodyBox)
		{
			NewPos.X -= 0.5;
		}

		if (TargetComp == EastWallBodyBox)
		{
			NewPos.Y += 0.5;
		}

		if (TargetComp == WestWallBodyBox)
		{
			NewPos.Y -= 0.5;
		}

		// 자식에게 새로운 좌표값 지정
		ChildBuild->SetBuildPos(NewPos);
		PC->GetPlayerRaft()->UpdateBuildMetaData(NewPos, ChildBuild);
		
		if (ABuildingFloor* ChildFloor = Cast<ABuildingFloor>(ChildBuild))
		{
			ChildFloor->UpdateWireframeBoxInfo();
		}
	}
	
	Super::UpdateBuildData(TargetComp, ChildBuild);
}

void ABuildingFloor::AddDurability(const int8 AddValue)
{
	Super::AddDurability(AddValue);
	if (GetCurrentDurability() == 0)
	{
		CrackDecal->SetVisibility(true);
	}
}

void ABuildingFloor::UpdateWireframeBoxInfo()
{
	if (const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PC->GetPlayerRaft())
		{
			return;
		}
		
		constexpr int MoveToX[4] = { 0, 0, 1, -1 };
		constexpr int MoveToY[4] = { 1, -1, 0, 0 };
		constexpr EBlockPos MoveTo[4] = { EBlockPos::East,
			EBlockPos::West, EBlockPos::North, EBlockPos::South };
		
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
			if (PC->GetPlayerRaft()->GetRaftBuildMetaData().Find(TempPos))
			{
				SearchVectorList.Add(TempPos);
			}
		}

		// 검색할 노드들 순회
		for (const FVector SearchPos : SearchVectorList)
		{
			for (int i = 0; i < 4; i++)
			{
				FVector TempPos = SearchPos;
				TempPos.X += MoveToX[i];
				TempPos.Y += MoveToY[i];

				const ABuildingActor* FoundBuild = PC->GetPlayerRaft()->GetRaftBuildPointerData().FindRef(TempPos);
				const bool IsAlreadyBound = IsValid(FoundBuild);
			
				ABuildingActor* SetBuild = PC->GetPlayerRaft()->GetRaftBuildPointerData().FindRef(SearchPos);
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

void ABuildingFloor::OnWallBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ABuildingFloor::StaticClass()))
	{
		OverlappedComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	}
}

TObjectPtr<UBoxComponent> ABuildingFloor::GetFloorBoxByDirection(const EBlockPos Direction, const bool IsReverse)
{
	if (Direction == EBlockPos::East)
	{
		return IsReverse ? LeftBodyBox : RightBodyBox;
	}
	if (Direction == EBlockPos::West)
	{
		return IsReverse ? RightBodyBox : LeftBodyBox;
	}
	if (Direction == EBlockPos::South)
	{
		return IsReverse ? NorthBodyBox : SouthBodyBox;
	}
	return IsReverse ? SouthBodyBox : NorthBodyBox;
}

TObjectPtr<USceneComponent> ABuildingFloor::GetWallPlaceVectorByDirection(const EBlockPos Direction, const bool IsReverse)
{
	if (Direction == EBlockPos::East)
	{
		return IsReverse ? WestWallSceneVector : EastWallSceneVector;
	}
	if (Direction == EBlockPos::West)
	{
		return IsReverse ? EastWallSceneVector : WestWallSceneVector;
	}
	if (Direction == EBlockPos::South)
	{
		return IsReverse ? NorthWallSceneVector : SouthWallSceneVector;
	}
	return IsReverse ? SouthWallSceneVector : NorthWallSceneVector;
}

TObjectPtr<USceneComponent> ABuildingFloor::GetWallPlaceVectorByComponentBox(const UPrimitiveComponent* ComponentBox, const bool IsReverse)
{
	if (ComponentBox == EastWallBodyBox)
	{
		return IsReverse ? WestWallSceneVector : EastWallSceneVector;
	}
	if (ComponentBox == WestWallBodyBox)
	{
		return IsReverse ? EastWallSceneVector : WestWallSceneVector;
	}
	if (ComponentBox == SouthWallBodyBox)
	{
		return IsReverse ? NorthWallSceneVector : SouthWallSceneVector;
	}
	return IsReverse ? SouthWallSceneVector : NorthWallSceneVector;
}


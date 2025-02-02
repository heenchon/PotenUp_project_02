#include "Raft.h"

#include "BuoyancyComponent.h"
#include "../RaftGameState.h"
#include "../Objects/Sail.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Building/BuildingWall.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Game/RaftSaveGame.h"
#include "project_02/Helper/BuildingHelper.h"
#include "project_02/HY/Objects/PlaceObjects.h"
#include "project_02/Player/BasePlayerController.h"

ARaft::ARaft()
{
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->BodyInstance.bLockZRotation = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Water/Caustics/Meshes/CausticsPreviewBase.CausticsPreviewBase'"));

	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);
	
	Buoyancy->AddCustomPontoon(100,"one");
	Buoyancy->AddCustomPontoon(100,"two");
	Buoyancy->AddCustomPontoon(100,"three");
	Buoyancy->AddCustomPontoon(100,"four");
	Buoyancy->BuoyancyData.Pontoons[0].RelativeLocation = {50.0f,50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[1].RelativeLocation = {-50.0f,50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[2].RelativeLocation = {50.0f,-50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[3].RelativeLocation = {-50.0f,-50.0f,75.f};

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Ignore);
}

// Called when the game starts or when spawned
void ARaft::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetHiddenInGame(true);

}

void ARaft::Initialize()
{
	InitializeData();
	SpawnSailActor();
}

void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	
	AddActorLocalOffset(RaftGameState->WindDirection
		* DeltaTime * RaftGameState->WindStrength * SailStrength);
}

void ARaft::SpawnSailActor()
{
	if (ASail* Sail = GetWorld()->SpawnActor<ASail>(ASail::StaticClass()))
	{
		Sail->AttachToActor(RaftBuildPointerData[FVector::Zero()], FAttachmentTransformRules::KeepRelativeTransform);
		Sail->SetRaft(this);
	}
}

void ARaft::UpdateBuildMetaData(const FVector& Pos, ABuildingActor* Build, const bool IsRemove, const bool IsCenter)
{
	if (IsRemove)
	{
		RaftBuildMetaData.Remove(Pos);
		RaftBuildPointerData.Remove(Pos);
	} else
	{
		// 우선은 좌표 값 추가
		Build->SetBuildPos(Pos);
		
		FBuildData BuildData;
		BuildData.BlockType = EBlockType::Wood;
		BuildData.BlockCategory = EBlockCategory::Undefined;
		BuildData.IsMain = IsCenter;
		BuildData.CurrentDurability = Build->GetCurrentDurability();
		
		if (Build->IsA(ABuildingFloor::StaticClass()))
		{
			BuildData.BlockCategory = EBlockCategory::Floor;
		}
		
		if (Build->IsA(ABuildingWall::StaticClass()))
		{
			BuildData.BlockCategory = EBlockCategory::Wall;
		}
		// TODO: 현재는 단순 추가이지만 추후 업데이트 로직도 넣을 필요가 있다.
		RaftBuildMetaData.Add(Pos, BuildData);
		RaftBuildPointerData.Add(Pos, Build);

		if (IsCenter)
		{
			Build->SetCenter();
			CenterBuildActor = Build;
		} else
		{
			Build->SetMainBuild(CenterBuildActor);
		}
	}
}

void ARaft::UpdatePlacedObjectData(const FVector& Pos, const FPlacedObjectData& PlaceData, const bool IsRemove)
{
	if (IsRemove)
	{
		RaftPlacedObjectData.Remove(Pos);
	} else
	{
		if (!RaftPlacedObjectData.Find(Pos))
		{
			RaftPlacedObjectData.Add(Pos);
			
		}
		RaftPlacedObjectData[Pos].Add(PlaceData);
	}
}

void ARaft::InitializeData()
{
	ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!PC)
	{
		return;
	}
	
	// 처음 게임을 플레이하는 경우에 데이터 Initialize
	if (!PC->GetRecentSaveData()->IsAlreadyStart)
	{
		// 루트인 가장 중앙은 무조건 하나 새로 만들고 시작한다.
		if (ABuildingFloor* NewMainFloor = GetWorld()->SpawnActor<ABuildingFloor>(
			FBuildingHelper::GetBuildingClass(GetWorld(), EBlockType::Wood, EBlockCategory::Floor)
		))
		{
			UpdateBuildMetaData(FVector::Zero(), NewMainFloor, false, true);
			NewMainFloor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		}
		return;
	}
	
	PlaceInitialBuild();
	PlaceInitialPlaceObject();
}

void ARaft::PlaceInitialBuild()
{
	ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (!PC)
	{
		return;
	}
	
	TMap<FVector, FBuildData>& RecentSaveRaftMap = PC->GetRecentSaveData()->RaftBuildMetaData;
	
	// BFS를 이용해서 순차적으로 적용한다.
	TQueue<FVector> RaftBuildBfs;
	// 가장 중심점은 Zero이기 때문에, Zero Vector를 중심으로 처리한다.
	RaftBuildBfs.Enqueue(FVector::Zero());

	// 비어있지 않는 이상 계속 순차적으로 적용시킨다.
	while (!RaftBuildBfs.IsEmpty())
	{
		const FVector& TopVector = *RaftBuildBfs.Peek();

		// 중복값이 들어가는 경우가 생길 수 밖에 없는데, 그런 경우는 위에서 한번 다시 걸러준다.
		if (GetRaftBuildPointerData().Find(TopVector))
		{
			RaftBuildBfs.Pop();
			continue;
		}
		
		UE_LOG(LogTemp, Display, TEXT("Search Start Test Raft: %s"), *TopVector.ToString())
		// C++의 구조화된 바인딩으로 JS에서 구조분해할당과 동일한 기능이다.
		// 역시 JS는 C++ 기반이 맞다...
		auto [BlockType, BlockCategory, CurrentDurability, IsMain] = RecentSaveRaftMap[TopVector];

		ABuildingActor* NewBuildingActor =
			GetWorld()->SpawnActor<ABuildingActor>(
				FBuildingHelper::GetBuildingClass(GetWorld(),
					BlockType,
					BlockCategory), GetActorTransform());
		// 처음에는 무조건 와이어프레임 상태이기 때문에 와이어프레임 상태를 종료해야 한다.
		// 우선 메타 정보부터 다시 업데이트 처리를 한다.
		NewBuildingActor->SetCurrentDurability(CurrentDurability);
		UpdateBuildMetaData(TopVector, NewBuildingActor, false, IsMain);

		if (IsMain)
		{
			NewBuildingActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		} else
		{
			NewBuildingActor->AttachToActor(CenterBuildActor, FAttachmentTransformRules::KeepWorldTransform);
		}

		EnqueueNextBuildData(RecentSaveRaftMap, NewBuildingActor, RaftBuildBfs);
		
		EnqueueNextFloorDataByWall(RecentSaveRaftMap, NewBuildingActor, RaftBuildBfs);
		
		RaftBuildBfs.Pop();
	}
}

void ARaft::PlaceInitialPlaceObject()
{
	ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	
	if (!PC)
	{
		return;
	}

	const UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	
	for (const TTuple<FVector, FPlacedObjectDataArray>& RaftPlacedObjectMetaData
		: PC->GetRecentSaveData()->RaftPlacedObjectMetaData)
	{
		for (const FPlacedObjectData& ObjectData : RaftPlacedObjectMetaData.Value.ObjectArray)
		{
			FItemInfoData ItemData = GI->GetItemInfoList()[ObjectData.ObjectId];

			AActor* NewActor = GetWorld()->SpawnActor(ItemData.GetShowItemActor());
			ABuildingActor* NewBuild = RaftBuildPointerData.FindRef(RaftPlacedObjectMetaData.Key);

			if (!IsValid(NewBuild))
			{
				continue;
			}
			
			NewActor->SetActorLocation(ObjectData.RelativeLoc);
			NewActor->AttachToActor(NewBuild, FAttachmentTransformRules::KeepRelativeTransform);
			UpdatePlacedObjectData(RaftPlacedObjectMetaData.Key, ObjectData);

			APlaceObjects* PlaceObject = Cast<APlaceObjects>(NewActor);
			if (!IsValid(PlaceObject))
			{
				continue;
			}
			PlaceObject->Place();
		}
	}
}

void ARaft::EnqueueNextFloorDataByWall(const TMap<FVector, FBuildData>& RecentMap, ABuildingActor* Target, TQueue<FVector>& RaftBuildBfs)
{
	const FVector& TopVector = *RaftBuildBfs.Peek();
	
	if (ABuildingWall* NewBuildingWall = Cast<ABuildingWall>(Target))
	{
		InitializeAttachWall(NewBuildingWall);
			
		constexpr float MoveTo[2] = { -0.5, 0.5 };
			
		if (FMath::Fractional(TopVector.X) != 0)
		{
			for (int i = 0; i < 2; i++)
			{
				FVector TempPos = TopVector;
				TempPos.Z += 1;
				TempPos.X += MoveTo[i];
				if (RecentMap.Find(TempPos) && !GetRaftBuildPointerData().Find(TempPos))
				{
					RaftBuildBfs.Enqueue(TempPos);
				}
			}
		}
			
		if (FMath::Fractional(TopVector.Y) != 0)
		{
			for (int i = 0; i < 2; i++)
			{
				FVector TempPos = TopVector;
				TempPos.Z += 1;
				TempPos.Y += MoveTo[i];
				if (RecentMap.Find(TempPos) && !GetRaftBuildPointerData().Find(TempPos))
				{
					RaftBuildBfs.Enqueue(TempPos);
				}
			}
		}
	}
}


void ARaft::EnqueueNextBuildData(const TMap<FVector, FBuildData>& RecentMap, ABuildingActor* Target, TQueue<FVector>& RaftBuildBfs)
{
	const FVector& TopVector = *RaftBuildBfs.Peek();
	
	if (ABuildingFloor* NewBuildingFloor = Cast<ABuildingFloor>(Target))
	{
		// 우선 가져온 좌표를 기반으로 부착을 진행한다.
		InitializeAttachFloor(NewBuildingFloor);

		// 아래에서는 동일한 높이의 상하좌우 바닥 값에 대한 검증과
		// 내 블럭을 기준으로 근처에 설치할 벽이 있는지 검증한다.
		// 벽은 상하좌우 다 검사할 필요 없이, 상, 우 기준으로만 검증해도
		// 어차피 어디선가 무조건 설치되기 때문에 무리할 필요 없이 처리해도 무방하다.
		// 이 서비스에서 벽의 상하좌우에 대해 예민하게 처리하지 않기 때문이다.
		// 오히려 중간 점이기 때문에 크게 신경쓸 필요가 없다.
		constexpr float CheckToX[8] = { 1, -1, 0, 0, 0.5, -0.5, 0, 0 };
		constexpr float CheckToY[8] = { 0, 0, 1, -1, 0, 0, 0.5, -0.5 };

		for (int i = 0; i < 8; i++)
		{
			FVector TempPos = TopVector;
			TempPos.X += CheckToX[i];
			TempPos.Y += CheckToY[i];
			// 최근 저장된 데이터에는 존재하지만, 현재 설치되지 않은 정보에 대해서 처리
			if (RecentMap.Find(TempPos) && !GetRaftBuildPointerData().Find(TempPos))
			{
				RaftBuildBfs.Enqueue(TempPos);
			}
		}
	}
}


void ARaft::InitializeAttachFloor(ABuildingFloor* NewFloor)
{
	// 당장 와이어 프레임 상태는 해제한다.
	NewFloor->SetWireframe(false);

	// 상하좌우 로 이동 체크하기 위한 상수 값들
	constexpr int8 CheckToX[4] = {1, -1, 0, 0};
	constexpr int8 CheckToY[4] = {0, 0, 1, -1};
	constexpr EBlockPos MoveTo[4] = { EBlockPos::North,
		EBlockPos::South, EBlockPos::East, EBlockPos::West };

	// 상하좌우를 둘러보는 로직 여기서 이미 발견된 경우
	// 즉 부착할 바닥이 옆에 있는 경우라면 밑으로 내려가지않는다.
	for (int i = 0; i < 4; i++)
	{
		FVector TempPos = NewFloor->GetBuildPos();
		TempPos.X += CheckToX[i];
		TempPos.Y += CheckToY[i];

		// 만약 상하좌우에 액터가 존재하는 경우에 대해서
		if (GetRaftBuildPointerData().FindRef(TempPos))
		{
			// 무조건 해당되어야 한다. 예외처리를 하지 않은 이유는
			// 해당되지 않으면 게임 데이터가 망가졌음을 의미하기 때문이다.
			ABuildingFloor* ParentFloor = Cast<ABuildingFloor>(GetRaftBuildPointerData()[TempPos]);
					
			// 주변에 부착할 부모 액터를 찾았으면, 그 액터의 상하좌우 중 부착할 컴포넌트 위치를
			// 가져와서 World Location을 변경해준다.
			// 다만 실제 부착할 액터는 방향의 반대 방향 소켓에 부착을 시켜줘야 한다.
			NewFloor->SetActorLocation(ParentFloor
				->GetFloorBoxByDirection(MoveTo[i], true)->GetComponentLocation());

			// 부착될 액터와 부착할 액터 둘다 검증을 해줘야 한다.
			ParentFloor->UpdateWireframeBoxInfo();
			NewFloor->UpdateWireframeBoxInfo();
			// 하나라도 걸리면 더 이상 계산할 필요가 없다.
			return;
		}
	}

	// 주변에 바닥이 없다면 벽에 부착을 해야만 한다.
	constexpr float CheckToWallX[4] = { 0.5, -0.5, 0, 0 };
	constexpr float CheckToWallY[4] = { 0, 0, 0.5, -0.5 };
	constexpr EBlockPos MoveToWall[4] = { EBlockPos::North,
		EBlockPos::South, EBlockPos::North, EBlockPos::South };

	for (int i = 0; i < 4; i++)
	{
		// 현재 설치된 벽 정보를 찾는 것이 목적
		FVector TempPos = NewFloor->GetBuildPos();
		TempPos.X += CheckToWallX[i];
		TempPos.Y += CheckToWallY[i];
		TempPos.Z -= 1;

		ABuildingWall* TargetWall = Cast<ABuildingWall>(GetRaftBuildPointerData().FindRef(TempPos));
		if (!TargetWall)
		{
			continue;
		}

		// Target이 있다라는 의미는 즉 벽이 있다라는 의미다.
		// 즉 벽에 맞춰서 그 벽의 앞 뒤에 붙여주면 된다.
		NewFloor->SetActorLocation(TargetWall
			->GetBoxByDirection(MoveToWall[i], true)->GetComponentLocation());
		// 선택된 벽도 주변에 블록이 있는지 확인하고 업데이트 처리를 해줘야 한다.
		TargetWall->UpdateWireframeBoxInfo();

		return;
	}
}

void ARaft::InitializeAttachWall(ABuildingWall* NewWall)
{
	// 당장 와이어 프레임 상태는 해제한다.
	NewWall->SetWireframe(false);
	
	// 부착시킬 앞 뒤로 이동 체크하기 위한 상수 값들
	constexpr float CheckToX[4] = { 0.5, -0.5, 0, 0 };
	constexpr float CheckToY[4] = { 0, 0, 0.5, -0.5 };
	constexpr EBlockPos MoveTo[4] = { EBlockPos::North,
		EBlockPos::South, EBlockPos::East, EBlockPos::West };
	
	for (int i = 0; i < 4; i++)
	{
		// 소숫점을 가져올 때 0이 된다는 것은 방향을 찾았다라는 의미다.
		if (
			UKismetMathLibrary::Fraction(NewWall->GetBuildPos().X + CheckToX[i]) == 0
			&& UKismetMathLibrary::Fraction(NewWall->GetBuildPos().Y + CheckToY[i]) == 0
		)
		{
			FVector TempPos = NewWall->GetBuildPos();
			TempPos.X += CheckToX[i];
			TempPos.Y += CheckToY[i];

			if (GetRaftBuildPointerData().FindRef(TempPos))
			{
				ABuildingFloor* ParentFloor = Cast<ABuildingFloor>(GetRaftBuildPointerData()[TempPos]);
				// 주변에 부착할 부모 액터를 찾았으면, 그 액터의 상하좌우 중 부착할 컴포넌트 위치를
				// 가져와서 World Location을 변경해준다.
				// 다만 실제 부착할 액터는 방향의 반대 방향 소켓에 부착을 시켜줘야 한다.
				NewWall->SetActorLocation(ParentFloor
				->GetWallPlaceVectorByDirection(MoveTo[i], true)->GetComponentLocation());
				NewWall->SetActorRotation(ParentFloor
					->GetWallPlaceVectorByDirection(MoveTo[i], true)->GetComponentRotation());

				NewWall->UpdateWireframeBoxInfo();
				ParentFloor->UpdateWireframeBoxInfo();
			}
		}
	}
}
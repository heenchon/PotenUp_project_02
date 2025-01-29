#include "Raft.h"

#include "BuoyancyComponent.h"
#include "../RaftGameState.h"
#include "../Objects/Sail.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Building/BuildingWall.h"
#include "project_02/Game/RaftSaveGame.h"
#include "project_02/Helper/BuildingHelper.h"
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
		// 초기 값 설정. 임시 값
		SetActorLocation(PC->GetPawn()->GetActorLocation());
		
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

	// 처음 로드할 때 읽어오기 편하도록 처리함
	// 최근 저장 정보를 기반으로 새로 실행 및 데이터 이전
	PC->GetRecentSaveData()->RaftBuildMetaData.KeySort([](const FVector& A, const FVector& B)
	{
		// 피타고라스 정리를 이용해서 대각선의 길이로 거리를 판별한다.
		const float MagnitudeA = FMath::Sqrt(FMath::Square(A.X) + FMath::Square(A.Y));
		const float MagnitudeB = FMath::Sqrt(FMath::Square(B.X) + FMath::Square(B.Y));
		return MagnitudeA < MagnitudeB;
	});
	
	SetActorTransform(PC->GetRecentSaveData()->LastRaftTransform);
	// 처음 시작이 아닌 경우는 전부 빌딩을 실행한다.
	for (TTuple<FVector, FBuildData> BuildData : PC->GetRecentSaveData()->RaftBuildMetaData)
	{
		// 일단 뭐든 데이터 기반으로 액터들 스폰 처리
		ABuildingActor* NewBuildingActor =
			GetWorld()->SpawnActor<ABuildingActor>(
				FBuildingHelper::GetBuildingClass(GetWorld(),
					BuildData.Value.BlockType,
					BuildData.Value.BlockCategory), GetActorTransform());
		// 처음에는 무조건 와이어프레임 상태이기 때문에 와이어프레임 상태를 종료해야 한다.
		// 우선 메타 정보부터 다시 업데이트 처리를 한다.
		UpdateBuildMetaData(BuildData.Key, NewBuildingActor, false, BuildData.Value.IsMain);

		// 이게 바닥이라는 가정하에 처리 로직 추가
		if (ABuildingFloor* NewFloor = Cast<ABuildingFloor>(NewBuildingActor))
		{
			// 당장 와이어 프레임 상태는 해제한다.
			NewFloor->SetWireframe(false);

			// 상하좌우 로 이동 체크하기 위한 상수 값들
			constexpr int8 CheckToX[4] = {1, -1, 0, 0};
			constexpr int8 CheckToY[4] = {0, 0, 1, -1};
			constexpr EBlockPos MoveTo[4] = { EBlockPos::North,
				EBlockPos::South, EBlockPos::East, EBlockPos::West };

			// 상하좌우를 둘러보는 로직
			for (int i = 0; i < 4; i++)
			{
				FVector TempPos = BuildData.Key;
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
					break;
				}
			}
		}

		// 벽이라는 가정하에 처리하기.
	}
}
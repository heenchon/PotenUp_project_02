#include "Raft.h"

#include "BuoyancyComponent.h"
#include "../RaftGameState.h"
#include "../Objects/Sail.h"
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

void ARaft::UpdateBuildMetaData(const FVector& Pos, ABuildingActor* Build, const bool IsRemove)
{
	if (IsRemove)
	{
		RaftBuildMetaData.Remove(Pos);
		RaftBuildPointerData.Remove(Pos);
	} else
	{
		FBuildData BuildData;
		BuildData.BlockType = EBlockType::Wood;
		BuildData.BlockCategory = EBlockCategory::Undefined;
		
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
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		if (!PC->GetRecentSaveData()->IsAlreadyStart)
		{
			// 초기 값 설정. 임시 값
			SetActorLocation(PC->GetPawn()->GetActorLocation());
			
			// 루트인 가장 중앙은 무조건 하나 새로 만들고 시작한다.
			if (ABuildingFloor* NewMainFloor = GetWorld()->SpawnActor<ABuildingFloor>(
				FBuildingHelper::GetBuildingClass(GetWorld(), EBlockType::Wood, EBlockCategory::Floor)
			))
			{
				RaftBuildPointerData.Add(FVector::Zero(), NewMainFloor);
				NewMainFloor->SetCenter();
				NewMainFloor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			}
			
			return;
		}

		// 처음 로드할 때 읽어오기 편하도록 처리함
		// 정렬은 최대한 중앙에 가깝게 처리하기 위해서 X와 Y의 절대값이 최대한 작은 것을 기준으로 한다.
		// ex. (1, 1), (-1, 1), (1, 2), (-1, 2) 같은 느낌으로...
		PC->GetRecentSaveData()->RaftBuildMetaData.KeySort([](const FVector& A, const FVector& B)
		{
			return abs(A.X) < abs(B.X) && abs(A.Y) < abs(B.Y);
		});
		
		SetActorTransform(PC->GetRecentSaveData()->LastRaftTransform);
		// 처음 시작이 아닌 경우는 전부 빌딩을 실행한다.
		for (TTuple<FVector, FBuildData> BuildData : PC->GetRecentSaveData()->RaftBuildMetaData)
		{
			if (BuildData.Value.BlockCategory == EBlockCategory::Floor)
			{
				GetWorld()->SpawnActor<ABuildingFloor>();
			}
			UE_LOG(LogTemp, Display, TEXT("%s"), *BuildData.Key.ToString());
		}
	}
}

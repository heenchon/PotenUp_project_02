#include "Raft.h"

#include "BuoyancyComponent.h"
#include "../RaftGameState.h"
#include "../Objects/Sail.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Building/BuildingActor.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Building/BuildingWall.h"
#include "project_02/Game/RaftSaveGame.h"
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
	if (ASail* sail = GetWorld()->SpawnActor<ASail>(ASail::StaticClass()))
	{
		sail->AttachToActor(MainFloor, FAttachmentTransformRules::KeepRelativeTransform);
		sail->SetRaft(this);
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
		// 처음 시작인 경우는 새로 만들기만 해준다.
		if (!PC->GetRecentSaveData()->IsAlreadyStart)
		{
			// 초기 값 설정. 임시 값
			SetActorLocation(PC->GetPawn()->GetActorLocation());
			if (ABuildingFloor* NewMainFloor = GetWorld()->SpawnActor<ABuildingFloor>(MainFloorClass))
			{
				MainFloor = NewMainFloor;
				MainFloor->SetCenter();
				MainFloor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			}
			return;
		}

		SetActorTransform(PC->GetRecentSaveData()->LastRaftTransform);
		// 처음 시작이 아닌 경우는 전부 빌딩을 실행한다.
		for (TTuple<FVector, FBuildData> BuildData : PC->GetRecentSaveData()->RaftBuildMetaData)
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *BuildData.Key.ToString());
		}
	}
}

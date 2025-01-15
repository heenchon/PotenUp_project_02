#include "Trash.h"
#include "BuoyancyComponent.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/DataTable/ItemInfoData.h"
// #include "../Raft/Raft.h"


ATrash::ATrash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);
	
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	Buoyancy->AddCustomPontoon(100,"center");

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Ignore);
	StaticMesh->BodyInstance.bLockRotation = true;
}

void ATrash::BeginPlay()
{
	Super::BeginPlay();
	
	const FItemInfoData ItemData = FItemHelper::GetItemInfoById(GetWorld(), ItemId);
	
	ItemMetaInfo.SetId(ItemId);
	// TODO: 해당 하드코딩은 추후 랜덤 값 or 지정 값으로 변경 가능성 있음
	ItemMetaInfo.SetCurrentCount(1);
	ItemMetaInfo.SetMetaData(ItemData.GetMetaData());
	
	//TODO: 윈드 매니저 추가 후 cpp 수정
	// if (Raft)
	// {
	// 	WindDirection=Raft->WindDirection;
	// 	WindStrength=Raft->WindStrength;
	// }
	// else UE_LOG(LogTemp,Error,TEXT("폐품은 Raft를 찾고있어요.. detail 패널 설정 부탁"));
}

void ATrash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// SetActorLocation(GetActorLocation()+WindDirection*DeltaTime*WindStrength);
}

void ATrash::UpdateItemInfo(const uint32 RemainCount)
{
	if (RemainCount == 0)
	{
		Destroy();
	} else
	{
		ItemMetaInfo.SetCurrentCount(RemainCount);
	}
}


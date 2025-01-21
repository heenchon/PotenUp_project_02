#include "Trash.h"
#include "BuoyancyComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/HY/RaftGameState.h"


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
}

void ATrash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (const ARaftGameState* GS = Cast<ARaftGameState>(UGameplayStatics::GetGameState(GetWorld())))
	{
		// TODO: -2는 바람의 반대 방향으로 보내기 위한 하드코딩
		if (!IsStop)
		{
			SetActorLocation(GetActorLocation() + GS->WindDirection * DeltaTime * GS->WindStrength * -2);
		}
	}
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


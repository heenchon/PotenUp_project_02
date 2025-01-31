#include "BuildingActor.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Helper/BuildingHelper.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/HY/Raft/Raft.h"


ABuildingActor::ABuildingActor()
{
	RootComp = CreateDefaultSubobject<USceneComponent>("Root Comp");
	SetRootComponent(RootComp);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(RootComp);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BodyMesh->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
}

void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
	OriginMaterials = BodyMesh->GetMaterials();

	// 값 초기화
	CurrentDurability = MaxDurability;
	
	if (IsMain)
	{
		OnWireframeInactive();
		return;
	}
	
	if (!IsWireframe)
	{
		OnWireframeInactive();
	} else
	{
		OnWireframeActive();
	}
}

void ABuildingActor::SetCenter()
{
	IsMain = true;
	MainBuild = this;
	BuildPos = FVector::ZeroVector;
	OnWireframeInactive();
}

void ABuildingActor::UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild)
{
	ChildBuild->SetWireframe(false);
	ChildBuild->SetDefaultMaterial();
}

void ABuildingActor::OnWireframeActive()
{
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingActor::OnWireframeInactive()
{
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	if (!IsMain)
	{
		AttachToComponent(
        				MainBuild->GetRootComponent(),
        				FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ABuildingActor::AttachWireframeToComponent(ABuildingActor* TargetBlock, UPrimitiveComponent* TargetComp)
{
	if (IsWireframe)
	{
		return;
	}
	
	TargetBlock->AttachToComponent(
		TargetComp,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void ABuildingActor::SetWireframe(const bool NewIsWireframe)
{
	IsWireframe = NewIsWireframe;
	if (IsWireframe)
	{
		OnWireframeActive();	
	} else
	{
		OnWireframeInactive();
	}
}

void ABuildingActor::SetWireframeMaterial(UMaterial* NewMaterial)
{
	for (int i = 0; i < BodyMesh->GetMaterials().Num(); i++)
	{
		BodyMesh->SetMaterial(i, NewMaterial);
	}
}

void ABuildingActor::SetDefaultMaterial()
{
	for (int i = 0; i < OriginMaterials.Num(); i++)
	{
		BodyMesh->SetMaterial(i, OriginMaterials[i]);
	}
}

void ABuildingActor::AddDurability(const int8 AddValue)
{
	if (CurrentDurability == 0)
	{
		return;
	}
	
	// 음수 방지용
	CurrentDurability = FMath::Max(CurrentDurability - AddValue, 0);
	UE_LOG(LogTemp, Display, TEXT("남은 내구도: %d"), CurrentDurability);

	if (const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		// 내구도에 따른 처리 이후에 동작시키기
		// TODO: 내구성 정보를 저장하는 기능을 추가한다.
		// PC->GetPlayerRaft()->UpdateBuildMetaData(
		// 	GetBuildPos(), this, true);
	}
}
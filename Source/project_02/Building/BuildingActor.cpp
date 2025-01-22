#include "BuildingActor.h"



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
	
	if (!IsWireframe)
	{
		OnWireframeInactive();
	} else
	{
		OnWireframeActive();
	}
}

float ABuildingActor::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	Destroy();
	return DamageAmount;
}


void ABuildingActor::OnWireframeActive()
{
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingActor::OnWireframeInactive()
{
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
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




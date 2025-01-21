#include "BuildingActor.h"

#include "Components/BoxComponent.h"


ABuildingActor::ABuildingActor()
{
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	SetRootComponent(BodyMesh);
	BodyMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);

	RightBodyBox = CreateDefaultSubobject<UBoxComponent>("Right Body Box");
	RightBodyBox->SetupAttachment(BodyMesh);
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	LeftBodyBox = CreateDefaultSubobject<UBoxComponent>("Left Body Box");
	LeftBodyBox->SetupAttachment(BodyMesh);
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	NorthBodyBox = CreateDefaultSubobject<UBoxComponent>("North Body Box");
	NorthBodyBox->SetupAttachment(BodyMesh);
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	
	SouthBodyBox = CreateDefaultSubobject<UBoxComponent>("South Body Box");
	SouthBodyBox->SetupAttachment(BodyMesh);
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingActor::BeginPlay()
{
	Super::BeginPlay();
	OriginMaterials = BodyMesh->GetMaterials();
	
	if (!IsWireframe)
	{
		OnWireframeDeactive();
	}
}

void ABuildingActor::OnWireframeActive()
{
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
}

void ABuildingActor::OnWireframeDeactive()
{
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
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
		OnWireframeDeactive();
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




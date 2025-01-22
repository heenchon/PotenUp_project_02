#include "BuildingFloor.h"

#include "Components/BoxComponent.h"

// Sets default values
ABuildingFloor::ABuildingFloor()
{
	RightBodyBox = CreateDefaultSubobject<UBoxComponent>("Right Body Box");
	RightBodyBox->SetupAttachment(GetBodyMesh());
	RightBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	RightBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	RightBodyBox->SetRelativeLocation({0, 89, 0});
	RightBodyBox->SetRelativeScale3D({1.4, 1.4, 0.25});
	
	LeftBodyBox = CreateDefaultSubobject<UBoxComponent>("Left Body Box");
	LeftBodyBox->SetupAttachment(GetBodyMesh());
	LeftBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	LeftBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	LeftBodyBox->SetRelativeLocation({0, -89, 0});
	LeftBodyBox->SetRelativeScale3D({1.4, 1.4, 0.25});
	
	NorthBodyBox = CreateDefaultSubobject<UBoxComponent>("North Body Box");
	NorthBodyBox->SetupAttachment(GetBodyMesh());
	NorthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	NorthBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	NorthBodyBox->SetRelativeLocation({89, 0, 0});
	NorthBodyBox->SetRelativeScale3D({1.4, 1.4, 0.25});
	
	SouthBodyBox = CreateDefaultSubobject<UBoxComponent>("South Body Box");
	SouthBodyBox->SetupAttachment(GetBodyMesh());
	SouthBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	SouthBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	SouthBodyBox->SetRelativeLocation({-89, 0, 0});
	SouthBodyBox->SetRelativeScale3D({1.4, 1.4, 0.25});
}

void ABuildingFloor::BeginPlay()
{
	Super::BeginPlay();
	
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



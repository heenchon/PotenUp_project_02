#include "BuildingWall.h"

#include "Components/BoxComponent.h"


ABuildingWall::ABuildingWall()
{
	ForwardFloorBodyBox = CreateDefaultSubobject<UBoxComponent>("Forward Floor Body Box");
	ForwardFloorBodyBox->SetupAttachment(GetRootComponent());
	ForwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	ForwardFloorBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	ForwardFloorBodyBox->SetRelativeLocation({0, 88.408905, 98.0});
	ForwardFloorBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
	
	BackwardFloorBodyBox = CreateDefaultSubobject<UBoxComponent>("Backward Floor Body Box");
	BackwardFloorBodyBox->SetupAttachment(GetRootComponent());
	BackwardFloorBodyBox->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	BackwardFloorBodyBox->SetCollisionResponseToChannel(ECC_EngineTraceChannel2, ECR_Block);
	BackwardFloorBodyBox->SetRelativeLocation({0, -90.929366, 98.0});
	BackwardFloorBodyBox->SetRelativeScale3D({2.8, 2.8, 0.25});
}

void ABuildingWall::BeginPlay()
{
	Super::BeginPlay();
	
}

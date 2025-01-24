#include "Hammer.h"

#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/BuildingComponent.h"


AHammer::AHammer()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(GetRootComponent());
}

void AHammer::BeginPlay()
{
	Super::BeginPlay();
}

void AHammer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->GetBuildingComponent()->SetBuildMode(false);
		Player->GetBuildingComponent()->ClearWireframe();
	}
}

void AHammer::StartInteractive()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->GetBuildingComponent()->BuildWireframe();
	}
}

void AHammer::OnAttached()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		Player->GetBuildingComponent()->SetBuildMode(true);
		// 기본 값은 바닥으로 시작한다.
		Player->GetBuildingComponent()->SetBuildType(EBuildType::Floor);
	}
}




#include "HookRope.h"
#include "CableComponent.h"

AHookRope::AHookRope()
{
	PrimaryActorTick.bCanEverTick = true;

	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	HookMesh->SetupAttachment(RootComponent);
	
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(RootComponent);
}

void AHookRope::BeginPlay()
{
	Super::BeginPlay();
	Cable->EndLocation = GetActorLocation();
}


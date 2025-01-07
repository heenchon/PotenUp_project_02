#include "HookRope.h"

#include "project_02/Tool/InteractiveHook.h"
#include "CableComponent.h"

AHookRope::AHookRope()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(RootComponent);
	Cable->CableWidth = 4;
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(RootComponent);
}

void AHookRope::BeginPlay()
{
	Super::BeginPlay();
	
	FVector NewLocation = GetActorLocation();
	NewLocation.Z -= 25;
	
	if (const AInteractiveHook* Hook = GetWorld()->SpawnActor<AInteractiveHook>(HookItem, NewLocation, GetActorRotation()))
	{
		Cable->SetAttachEndToComponent(Hook->GetHookMesh(), "Start");
	}
}



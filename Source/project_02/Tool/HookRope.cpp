#include "HookRope.h"
#include "CableComponent.h"

AHookRope::AHookRope()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(RootComponent);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(RootComponent);
	
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	HookMesh->SetupAttachment(RootComponent);
}

void AHookRope::BeginPlay()
{
	Super::BeginPlay();
}


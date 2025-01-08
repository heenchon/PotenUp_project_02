#include "HookRope.h"

#include "project_02/Tool/InteractiveHook.h"
#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AHookRope::AHookRope()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(RootComponent);
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(RootComponent);
}

void AHookRope::BeginPlay()
{
	Super::BeginPlay();
	Cable->SetVisibility(false);
}


void AHookRope::OnHoldInteractive()
{
	Power = UKismetMathLibrary::Min(Power + 1, MaxPower);
}

void AHookRope::OnEndInteractive()
{
	if (IsValid(ControlledHook))
	{
		// 이미 훅이 있으면 굳이 처리할 이유가 없다.
		return;
	}
	const APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector NewLocation = GetActorLocation() + Player->GetActorForwardVector() * 50;
	NewLocation.Z += 50;
	
	if (ControlledHook = GetWorld()->SpawnActor<AInteractiveHook>(HookItem, NewLocation, GetActorRotation()))
	{
		Cable->SetVisibility(true);
		Cable->SetAttachEndToComponent(ControlledHook->GetHookMesh(), "Start");
		
		ControlledHook->StartLaunch(Player->GetControlRotation().Vector().GetSafeNormal(1) + Player->GetActorForwardVector(), Power);
	}
	
	Power = 0;
}



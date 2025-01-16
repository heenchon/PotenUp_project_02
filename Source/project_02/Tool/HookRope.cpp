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


void AHookRope::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (ControlledHook)
		{
			ControlledHook->Destroy();
		}
	}
}


void AHookRope::OnHoldInteractive()
{
	if (Status == EInteractiveToolStatus::Completed)
	{
		return;
	}

	Status = EInteractiveToolStatus::Interacting;

	if (IsValid(ControlledHook))
	{
		if (ControlledHook->GetHookStatus() == EHookStatus::Fixed || ControlledHook->GetHookStatus() == EHookStatus::Pulled)
		{
			if (ControlledHook->PullHook(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()))
			{
				Status = EInteractiveToolStatus::Completed;
				Cable->SetVisibility(false);
			}
		}
		return;
	}
	
	Power = UKismetMathLibrary::Min(Power + 1, MaxPower);
}

void AHookRope::OnEndInteractive()
{
	if (Status == EInteractiveToolStatus::Completed)
	{
		Status = EInteractiveToolStatus::Idle;
		return;
	}
	
	if (IsValid(ControlledHook))
	{
		if (ControlledHook->GetHookStatus() == EHookStatus::Pulled)
		{
			ControlledHook->SetHookStatus(EHookStatus::Fixed);
		}
		return;
	}
	
	const APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FVector NewLocation = GetActorLocation() + Player->GetActorForwardVector() * 50;
	NewLocation.Z += 50;

	ControlledHook = GetWorld()->SpawnActor<AInteractiveHook>(HookItem, NewLocation, GetActorRotation());
	if (ControlledHook)
	{
		Cable->SetVisibility(true);
		Cable->SetAttachEndToComponent(ControlledHook->GetHookMesh(), "Start");
		ControlledHook->StartLaunch(Player->GetControlRotation().Vector().GetSafeNormal(1) + Player->GetActorForwardVector(), Power);
	}
	
	Power = 0;

	Status = EInteractiveToolStatus::Idle;
}



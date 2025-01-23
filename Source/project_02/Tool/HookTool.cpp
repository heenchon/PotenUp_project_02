#include "HookTool.h"

#include "CableComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/Tool/InteractiveHook.h"

#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"

AHookTool::AHookTool()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	Cable = CreateDefaultSubobject<UCableComponent>("Cable");
	Cable->SetupAttachment(RootComponent);

	Cable->EndLocation = FVector::ZeroVector;
	Cable->AttachEndToSocketName = "Hook Mesh";
	
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>("Body Mesh");
	BodyMesh->SetupAttachment(RootComponent);
}

void AHookTool::BeginPlay()
{
	Super::BeginPlay();
	Cable->SetVisibility(false);
}

void AHookTool::EndPlay(const EEndPlayReason::Type EndPlayReason)
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

void AHookTool::OnInteractiveHold()
{
	Super::OnInteractiveHold();

	if (IsValid(ControlledHook))
	{
		if (ControlledHook->GetHookStatus() == EHookStatus::Fixed || ControlledHook->GetHookStatus() == EHookStatus::Pulled)
		{
			if (ControlledHook->PullHook(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation()))
			{
				ChangeStatus(EInteractiveStatus::Completed);
				Cable->SetVisibility(false);
			}
		}
		return;
	}
	
	Power = UKismetMathLibrary::Min(Power + 2, MaxPower);
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC)
		
		PC->GetPlayerUI()->SetProgressPercent(static_cast<float>(Power) / MaxPower);
	}
}

bool AHookTool::EndInteractive()
{
	if (!Super::EndInteractive())
	{
		return false;
	}
	
	if (IsValid(ControlledHook))
	{
		if (ControlledHook->GetHookStatus() == EHookStatus::Pulled)
		{
			ControlledHook->SetHookStatus(EHookStatus::Fixed);
		}
		return true;
	}
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		FVector NewLocation = GetActorLocation() + Player->GetActorForwardVector() * 50;
		NewLocation.Z += 50;

		ControlledHook = GetWorld()->SpawnActor<AInteractiveHook>(HookItem, NewLocation, GetActorRotation());
		if (ControlledHook)
		{
			Cable->SetVisibility(true);
			Cable->SetAttachEndToComponent(ControlledHook->GetHookMesh(), "Start");
			ControlledHook->StartLaunch(Player->GetControlRotation().Vector(), Power);
		}
	
		Power = 0;
	
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC)
		
		PC->GetPlayerUI()->SetProgressPercent(0);
	}

	return true;
}
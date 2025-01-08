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
	if (IsValid(ControlledHook))
	{
		if (ControlledHook->GetHookStatus() == EHookStatus::Fixed)
		{
			// TODO: 이 PullHook은 끌어오고 그 결과를 가져오는데, 이게 true가 되는 경우에 한해 특정 애니메이션 진행과 이 Hold Event를 강제 종료한다.
			ControlledHook->PullHook(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation());
		}
		return;
	}
	
	Power = UKismetMathLibrary::Min(Power + 1, MaxPower);
}

void AHookRope::OnEndInteractive()
{
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
	
	if (ControlledHook = GetWorld()->SpawnActor<AInteractiveHook>(HookItem, NewLocation, GetActorRotation()))
	{
		Cable->SetVisibility(true);
		Cable->SetAttachEndToComponent(ControlledHook->GetHookMesh(), "Start");
		
		ControlledHook->StartLaunch(Player->GetControlRotation().Vector().GetSafeNormal(1) + Player->GetActorForwardVector(), Power);
	}
	
	Power = 0;
}



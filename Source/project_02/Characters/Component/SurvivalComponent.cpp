#include "SurvivalComponent.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/DataTable/EntityAnimationData.h"
#include "project_02/DataTable/EntityInfoData.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"

USurvivalComponent::USurvivalComponent()
{
	
}

void USurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitialSurvivalData();
}

void USurvivalComponent::SetHealth(const uint8 NewValue)
{
	HealthInfo.Key = NewValue;
	OnChangedHealth();	
}

void USurvivalComponent::SetHunger(const uint8 NewValue)
{
	HungerInfo.Key = NewValue;
	OnChangedHunger();	
}

void USurvivalComponent::SetThirst(const uint8 NewValue)
{
	ThirstInfo.Key = NewValue;
	OnChangedThirst();	
}

void USurvivalComponent::AddDamage(const uint8 NewValue)
{
	if (IsDied) return;
	HealthInfo.Key = UKismetMathLibrary::Max(HealthInfo.Key - NewValue, 0);
	OnChangedHealth();	
}

void USurvivalComponent::DecreaseHunger(const uint8 NewValue)
{
	if (IsDied) return;
	HungerInfo.Key = UKismetMathLibrary::Max(HungerInfo.Key - NewValue, 0);
	OnChangedHunger();	
}

void USurvivalComponent::IncreaseHunger(const uint8 NewValue)
{
	HungerInfo.Key = UKismetMathLibrary::Max(HungerInfo.Key - NewValue, 100);
	OnChangedHunger();	
}

void USurvivalComponent::DecreaseThirst(const uint8 NewValue)
{
	if (IsDied) return;
	ThirstInfo.Key = UKismetMathLibrary::Max(ThirstInfo.Key - NewValue, 0);
	OnChangedThirst();	
}

void USurvivalComponent::IncreaseThirst(const uint8 NewValue)
{
	ThirstInfo.Key = UKismetMathLibrary::Max(ThirstInfo.Key + NewValue, 100);
	OnChangedThirst();
}

// TODO: 옵저버 패턴을 이용해 Health가 변경될 때 마다 실행시켜도 무방할 것 같음
void USurvivalComponent::OnChangedHealth()
{
	if (HealthInfo.Key == 0)
	{
		IsDied = true;
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
		{
			ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
			check(PC);
			PC->OnDied();
		}
	}
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (IsDied)
		{
			const FEntityAnimationData* AnimData =
				Player->GetAnimationData().GetRow<FEntityAnimationData>("");

			check(AnimData);
			Player->bBlockInput = true;
			Player->PlayAnimMontage(AnimData->DiedAnimation.Get());
		}
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		PC->GetPlayerUI()->SetHealthPercent(HealthInfo.Key, HealthInfo.Value);
	}
}

void USurvivalComponent::OnChangedHunger()
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		
		
		PC->GetPlayerUI()->SetHungerPercent(HungerInfo.Key, HungerInfo.Value);
	}
}

void USurvivalComponent::OnChangedThirst()
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		PC->GetPlayerUI()->SetThirstPercent(ThirstInfo.Key, ThirstInfo.Value);
	}
}

void USurvivalComponent::InitialSurvivalData()
{
	IsDied = false;
	if (const FEntityInfoData* Data = EntityInfo.GetRow<FEntityInfoData>(""))
	{
		HealthInfo = { Data->MaxHealth, Data->MaxHealth };
		SetHealth(Data->MaxHealth);
		HungerInfo = { Data->MaxHunger, Data->MaxHunger };
		SetHunger(Data->MaxHunger);
		ThirstInfo = { Data->MaxThirst, Data->MaxThirst };
		SetThirst(Data->MaxThirst);
		
		FTimerManager& GlobalTimeManager = GetWorld()->GetTimerManager();
		
		GlobalTimeManager.ClearTimer(HungerDecreaseTimer);
		GlobalTimeManager.ClearTimer(ThirstDecreaseTimer);

		GlobalTimeManager.SetTimer(HungerDecreaseTimer,
			 FTimerDelegate::CreateUObject(this, &USurvivalComponent::DecreaseHunger, Data->HungerDecreaseGage)
			 , Data->HungerDecreaseTime, true);
	
		GlobalTimeManager.SetTimer(ThirstDecreaseTimer,
			 FTimerDelegate::CreateUObject(this, &USurvivalComponent::DecreaseThirst, Data->ThirstDecreaseGage)
			 , Data->ThirstDecreaseTime, true);
	}
}


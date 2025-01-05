#include "SurvivalComponent.h"

#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/DataTable/FEntityAnimationData.h"
#include "project_02/DataTable/FEntityInfoData.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/PlayerGameUI.h"


USurvivalComponent::USurvivalComponent()
{
	
}


void USurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (const FEntityInfoData* Data = EntityInfo.GetRow<FEntityInfoData>(""))
    {
    	HealthInfo = { Data->MaxHealth, Data->MaxHealth };
    	HungerInfo = { Data->MaxHunger, Data->MaxHunger };
    	ThirstInfo = { Data->MaxThirst, Data->MaxThirst };
		
		InitialSurvivalData();
    }
}

void USurvivalComponent::AddDamage(const uint8 NewValue)
{
	if (IsDied) return;
	HealthInfo.Key -= NewValue;
	OnChangedHealth();	
}

void USurvivalComponent::DecreaseHunger(const uint8 NewValue)
{
	if (IsDied) return;
	HungerInfo.Key -= NewValue;
	OnChangedHunger();	
}

void USurvivalComponent::DecreaseThirst(const uint8 NewValue)
{
	if (IsDied) return;
	ThirstInfo.Key -= NewValue;
	OnChangedThirst();	
}

// TODO: 옵저버 패턴을 이용해 Health가 변경될 때 마다 실행시켜도 무방할 것 같음
void USurvivalComponent::OnChangedHealth()
{
	if (HealthInfo.Key == 0)
	{
		IsDied = true;
	}
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (IsDied)
		{
			const FEntityAnimationData* AnimData =
				Player->GetAnimationData().GetRow<FEntityAnimationData>("");

			check(AnimData);
			Player->bBlockInput = true;
			Player->PlayAnimMontage(AnimData->DiedAnimation);
		}
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		UPlayerGameUI* GameUI = Cast<UPlayerGameUI>(PC->GetPlayerUI());
		check(GameUI);
		
		GameUI->SetHealthPercent(HealthInfo.Key, HealthInfo.Value);
	}
}

void USurvivalComponent::OnChangedHunger()
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		UPlayerGameUI* GameUI = Cast<UPlayerGameUI>(PC->GetPlayerUI());
		check(GameUI);
		
		GameUI->SetHungerPercent(HungerInfo.Key, HungerInfo.Value);
	}
}

void USurvivalComponent::OnChangedThirst()
{
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		UPlayerGameUI* GameUI = Cast<UPlayerGameUI>(PC->GetPlayerUI());
		check(GameUI);
		
		GameUI->SetThirstPercent(ThirstInfo.Key, ThirstInfo.Value);
	}
}

void USurvivalComponent::InitialSurvivalData()
{
	if (const FEntityInfoData* Data = EntityInfo.GetRow<FEntityInfoData>(""))
	{
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


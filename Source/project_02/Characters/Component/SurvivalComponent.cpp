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
    }
}

void USurvivalComponent::SetHealth(const uint8 NewValue)
{
	if (IsDied) return;
	HealthInfo.Key = NewValue;
	OnChangedHealth();
}

void USurvivalComponent::AddDamage(const uint8 NewValue)
{
	if (IsDied) return;
	HealthInfo.Key -= NewValue;
	UE_LOG(LogTemp, Display, TEXT("Test Value: %d"), HealthInfo.Key);
	OnChangedHealth();	
}

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



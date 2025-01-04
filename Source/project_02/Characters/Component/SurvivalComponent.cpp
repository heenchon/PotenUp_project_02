#include "SurvivalComponent.h"

#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/DataTable/FPlayerInfoData.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/PlayerGameUI.h"


USurvivalComponent::USurvivalComponent()
{
	
}


void USurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	// TODO: 해당 데이터는 플레이어 혹은 게임 인스턴스에 저장 필요
	if (const FPlayerInfoData* Data = PlayerInfoData.DataTable->FindRow<FPlayerInfoData>("DefaultPlayer", ""))
    {
    	HealthInfo = { Data->MaxHealth, Data->MaxHealth };
    	HungerInfo = { Data->MaxHunger, Data->MaxHunger };
    	ThirstInfo = { Data->MaxThirst, Data->MaxThirst };
    }
}

void USurvivalComponent::SetHealth(const uint8 NewValue)
{
	HealthInfo.Key = NewValue;
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC);
		
		UPlayerGameUI* GameUI = Cast<UPlayerGameUI>(PC->GetPlayerUI());
		check(GameUI);
		
		GameUI->SetHealthPercent(HealthInfo.Key, HealthInfo.Value);
	}
}



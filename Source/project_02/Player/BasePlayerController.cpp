// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "BasePlayerState.h"
#include "Blueprint/UserWidget.h"
// TODO: 하나의 파일에 여러 구조체가 있는 경우에 생기는 문제?로 추측은 하는데
// 확실하지 않다. 불필요한 Import를 줄이기 위해 추후 해당 header 파일의
// 조정이 필요해보인다.
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SurvivalComponent.h"

void ABasePlayerController::BeginPlay()
{
	if (PlayUIClass)
	{
		ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
		PS->InitializeData();
		
		PlayUI = CreateWidget(this, PlayUIClass);
		PlayUI->AddToViewport();
	}
}

void ABasePlayerController::OnDied()
{
	if (PlayerRespawnUIClass)
	{
		PlayerRespawnUI = CreateWidget(this, PlayerRespawnUIClass);
		PlayerRespawnUI->AddToViewport();
		SetShowMouseCursor(true);
	}
}

void ABasePlayerController::Respawn()
{
	const APlayerCharacter* PrevPlayer = Cast<APlayerCharacter>(GetPawn());
	PrevPlayer->GetSurvivalComponent()->InitialSurvivalData();
	PlayerRespawnUI->RemoveFromParent();
	SetShowMouseCursor(false);
}

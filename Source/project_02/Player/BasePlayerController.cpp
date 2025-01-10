// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "Blueprint/UserWidget.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SurvivalComponent.h"

void ABasePlayerController::BeginPlay()
{
	if (PlayUIClass)
	{
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
	APlayerCharacter* PrevPlayer = Cast<APlayerCharacter>(GetPawn());
	PrevPlayer->SurvivalComponent->InitialSurvivalData();
	PlayerRespawnUI->RemoveFromParent();
	SetShowMouseCursor(false);
}

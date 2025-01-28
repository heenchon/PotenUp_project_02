// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUI.h"

#include "Components/Button.h"
#include "project_02/Game/RaftGameMode.h"

void UMainUI::NativeOnInitialized()
{
	StartGameButton->OnClicked.AddDynamic(
		this, &ThisClass::OnClickStartGameButton);
}

void UMainUI::OnClickStartGameButton()
{
	GetWorld()->GetAuthGameMode<ARaftGameMode>()->GoToMainLevel();
	RemoveFromParent();
}


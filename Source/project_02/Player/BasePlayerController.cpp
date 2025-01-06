// Fill out your copyright notice in the Description page of Project Settings.


#include "BasePlayerController.h"

#include "Blueprint/UserWidget.h"

void ABasePlayerController::BeginPlay()
{
	if (PlayUIClass)
	{
		PlayUI = CreateWidget(this, PlayUIClass);
		PlayUI->AddToViewport();
	}
}

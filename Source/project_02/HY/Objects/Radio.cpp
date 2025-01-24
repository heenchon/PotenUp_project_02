// Fill out your copyright notice in the Description page of Project Settings.


#include "Radio.h"

#include "Blueprint/UserWidget.h"


ARadio::ARadio()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ARadio::BeginPlay()
{
	Super::BeginPlay();
}

void ARadio::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARadio::Interact()
{
	Super::Interact();
	EndingUI = CreateWidget(GetWorld(),EndingWidgetClass);
	
	if (EndingUI)
	{
		EndingUI->AddToViewport();
	}
}


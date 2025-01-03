// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalComponent.h"

#include "project_02/DataTable/FPlayerInfoData.h"


// Sets default values for this component's properties
USurvivalComponent::USurvivalComponent()
{
	
}


// Called when the game starts
void USurvivalComponent::BeginPlay()
{
	Super::BeginPlay();
	if (const FPlayerInfoData* Data = PlayerInfoData.DataTable->FindRow<FPlayerInfoData>("DefaultPlayer", ""))
    {
    	CurrentHealth = Data->MaxHealth;
    	CurrentHunger = Data->MaxHunger;
    	CurrentThirst = Data->MaxThirst;
    }
}


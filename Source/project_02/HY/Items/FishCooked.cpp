// Fill out your copyright notice in the Description page of Project Settings.


#include "FishCooked.h"
#include "project_02/Characters/Component/SurvivalComponent.h"

// Sets default values
AFishCooked::AFishCooked()
{
}

// Called when the game starts or when spawned
void AFishCooked::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFishCooked::Use()
{
	Super::Use();
	SurvivalComponent->IncreaseHunger(10);
	UE_LOG(LogTemp, Warning, TEXT("물고기 냠냠"));
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Fish.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"
#include "Slate/SGameLayerManager.h"


// Sets default values
AFish::AFish()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFish::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SurvivalComponent = Player->FindComponentByClass<USurvivalComponent>();
}

// Called every frame
void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFish::Use()
{
	Super::Use();
	if (bUsable)
	{
		SurvivalComponent->IncreaseHunger(10);
		UE_LOG(LogTemp, Warning, TEXT("물고기 냠냠"));
	}
}


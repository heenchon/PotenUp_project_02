// Fill out your copyright notice in the Description page of Project Settings.


#include "FishRaw.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"


// Sets default values
AFishRaw::AFishRaw()
{
}

// Called when the game starts or when spawned
void AFishRaw::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SurvivalComponent = Player->FindComponentByClass<USurvivalComponent>();
}

void AFishRaw::PutOnGrill()
{
	//플레이어가 들고 있는 물고기 개수 -1
	this->Destroy();
}


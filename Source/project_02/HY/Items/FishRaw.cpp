// Fill out your copyright notice in the Description page of Project Settings.


#include "FishRaw.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"


// Sets default values
AFishRaw::AFishRaw()
{
}

// Called when the game starts or when spawned
void AFishRaw::BeginPlay()
{
	Super::BeginPlay();
}

void AFishRaw::PutOnGrill()
{
	this->Destroy();
}


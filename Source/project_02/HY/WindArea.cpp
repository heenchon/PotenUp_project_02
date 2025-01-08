// Fill out your copyright notice in the Description page of Project Settings.


#include "WindArea.h"


// Sets default values
AWindArea::AWindArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWindArea::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWindArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


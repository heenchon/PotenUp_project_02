// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);

}

// Called every frame
void ASharkAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Cup.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"


// Sets default values
ACup::ACup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACup::BeginPlay()
{
	Super::BeginPlay();

	APawn* Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SurvivalComponent = Player->FindComponentByClass<USurvivalComponent>();
}

// Called every frame
void ACup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACup::Use()
{
	Super::Use();
	if (bUsable)
	{
		SurvivalComponent->IncreaseThirst(10);
		UE_LOG(LogTemp, Warning, TEXT("꿀꺽"));
	}
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "Cup.h"

#include "MovieSceneTracksComponentTypes.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"


// Sets default values
ACup::ACup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
	WaterMesh->SetupAttachment(Mesh);
	WaterMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WaterMesh->SetVisibility(false);
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
	if (bIsFresh)
	{
		bIsEmpty = true; bIsFresh = false;
		WaterMesh->SetVisibility(false);
		SurvivalComponent->IncreaseThirst(10);
		UE_LOG(LogTemp, Warning, TEXT("꿀꺽"));
	}
}

void ACup::FillSeaWater()
{
	if (bIsEmpty)
	{
		bIsEmpty = false;
		bIsSea = true;
		WaterMesh->SetVisibility(true);
	}
}

void ACup::FillFreshWater()
{
	if (bIsEmpty)
	{
		bIsEmpty = false;
		bIsFresh = true;
		WaterMesh->SetVisibility(true);
	}
}

void ACup::EmptyCup()
{
	bIsSea = false; bIsFresh = false;
	bIsEmpty = true;
	WaterMesh->SetVisibility(false);
}


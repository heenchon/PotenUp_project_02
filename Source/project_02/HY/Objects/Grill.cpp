// Fill out your copyright notice in the Description page of Project Settings.


#include "Grill.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/HY/Items/FishRaw.h"
#include "project_02/Player/BasePlayerState.h"


// Sets default values
AGrill::AGrill()
{
	ProcessDuration = 5.0f;
	FoodPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FoodPoint"));
	FoodPoint->SetupAttachment(StaticMesh);
	RawFoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	RawFoodMesh->SetupAttachment(StaticMesh);
	RawFoodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RawFoodMesh->SetVisibility(false);
}


// Called when the game starts or when spawned
void AGrill::BeginPlay()
{
	Super::BeginPlay();
}

void AGrill::Interact(AUsable_Item* input, int curItemIndex)
{
	Super::Interact(input, curItemIndex);
	if (!bIsFood)
	{
		if (AFishRaw* fishRaw = Cast<AFishRaw>(input))
		{
			UE_LOG(LogTemp,Warning,TEXT("물고기 굽기 시작."));
			bIsFood = true;
			PS->DropItem(curItemIndex, 1);
			RawFoodMesh->SetVisibility(true);
			fishRaw->PutOnGrill();
			ProcessStart();
		}
	}
}

void AGrill::ProcessComplete()
{
	Super::ProcessComplete();
	UE_LOG(LogTemp,Warning,TEXT("물고기 조리 완료."));
	AUsable_Item* fishCooked = GetWorld()->SpawnActor<AUsable_Item>(FishCookedTemp,FoodPoint->GetComponentTransform());
	RawFoodMesh->SetVisibility(false);
	bIsFood = false;
}





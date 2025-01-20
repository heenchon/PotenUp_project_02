// Fill out your copyright notice in the Description page of Project Settings.


#include "Grill.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/HY/Items/FishRaw.h"
// #include "project_02/HY/Items/FishCooked.h"
#include "project_02/HY/Trash/Trash.h"
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
	// UE_LOG(LogTemp,Warning,TEXT("현재 아이템 인덱스 %d"),curItemIndex);
	if (bIsFood)
	{
		bIsFood = false;
		return;
	}
	else
	{
		if (AFishRaw* fishRaw = Cast<AFishRaw>(input))
		{
			PS->DropItem(curItemIndex, 1);
			UE_LOG(LogTemp,Warning,TEXT("물고기 굽기 시작."));
			RawFoodMesh->SetVisibility(true);
			//플레이어가 들고 있는 물고기 삭제
			fishRaw->PutOnGrill();
			ProcessStart();
		}
	}
}

void AGrill::ProcessComplete()
{
	Super::ProcessComplete();
	UE_LOG(LogTemp,Warning,TEXT("물고기 조리 완료."));
	ATrash* fishCooked = GetWorld()->SpawnActor<ATrash>(FishCookedTemp,FoodPoint->GetComponentTransform());
	RawFoodMesh->SetVisibility(false);
	// fishCooked->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	bIsFood = true;
}



// Fill out your copyright notice in the Description page of Project Settings.


#include "Grill.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/HY/Items/FishRaw.h"
#include "project_02/Player/BasePlayerState.h"


// Sets default values
AGrill::AGrill()
{
	ProcessDuration = 5.0f;
}

// Called when the game starts or when spawned
void AGrill::BeginPlay()
{
	Super::BeginPlay();
}

void AGrill::Interact(AUsable_Item* input)
{
	Super::Interact(input);
	if (bIsFood)
	{
		//인벤에 아이템 추가
		bIsFood = false;
		return;
	}
	if (AFishRaw* fishRaw = Cast<AFishRaw>(input))
	{
		UE_LOG(LogTemp,Warning,TEXT("물고기 굽기 시작"));
		//플레이어가 들고 있는 물고기 삭제
		fishRaw->PutOnGrill();
		//조리 시작
		ProcesStart();
	}
}

void AGrill::ProcesComplete()
{
	Super::ProcesComplete();
	UE_LOG(LogTemp,Warning,TEXT("물고기 조리 완료."));
	bIsFood = true;
}



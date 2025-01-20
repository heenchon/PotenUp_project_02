// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterPurifier.h"

#include "project_02/HY/Items/Cup.h"


// Sets default values
AWaterPurifier::AWaterPurifier()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ProcessDuration = 5.0f;
}

// Called when the game starts or when spawned
void AWaterPurifier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWaterPurifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterPurifier::Interact(AUsable_Item* input)
{
	Super::Interact(input);
	if (ACup* cup = Cast<ACup>(input))
	{
		if (bIsPurified)
		{
			cup->FillFreshWater();
			bIsPurified = false;
			return;
		}
		
		//컵 속의 물을 없애고, 정수 시작
		if (cup->bIsSea)
		{
			UE_LOG(LogTemp, Warning, TEXT("정수기에 바닷물 넣기"));
			cup->EmptyCup();
			ProcesStart();
		}
	}
}

void AWaterPurifier::ProcesComplete()
{
	Super::ProcesComplete();
	UE_LOG(LogTemp, Warning, TEXT("정수 완료."));
	bIsPurified = true;
}


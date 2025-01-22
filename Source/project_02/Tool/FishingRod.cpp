// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"

#include "FishingFloat.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/Player/BasePlayerState.h"


// Sets default values
AFishingRod::AFishingRod()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	RodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RodMesh"));
	RodMesh->SetupAttachment(Root);
	RodPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RodEndpoint"));
	RodPoint->SetupAttachment(RodMesh);
}

// Called when the game starts or when spawned
void AFishingRod::BeginPlay()
{
	Super::BeginPlay();
}

void AFishingRod::StartInteractive()
{
	Super::StartInteractive();
	
	if (bIsFish)
	{
		Float->Destroy();
		GetFish();
		return;
	}
	ChargeStart();
}

void AFishingRod::OnInteractiveHold(float DeltaTime)
{
	Super::OnInteractiveHold();
	Charging(DeltaTime);
}

bool AFishingRod::EndInteractive()
{
	if (!Super::EndInteractive())
	{
		return false;
	}
	if (bIsCharging)
	{
		ChargeEnd();
	}
	return true;
}

void AFishingRod::ChargeStart()
{
	Power = 0.0f;
	bIsCharging = true;
}

void AFishingRod::Charging(float deltaTime)
{
	Power += deltaTime * 600.0f;
}

void AFishingRod::ChargeEnd()
{
	bIsCharging = false;

	Float = GetWorld()->SpawnActor<AFishingFloat>(FloatClass,RodPoint->GetComponentTransform());
	if (Float)
	{
		Float->SetFishingRod(this);
		Float->StartThrow(RodPoint->GetComponentLocation(),Power);
	}
}


void AFishingRod::StartFishing()
{
	bIsFish = true;
	UE_LOG(LogTemp,Warning,TEXT("미끼를 물었다!"));
}


void AFishingRod::GetFish()
{
	bIsFish = false;
	FItemMetaInfo NewFish = FItemHelper::GetInitialItemMetaDataById(GetWorld(), 8);
	ABasePlayerState* PS = UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetPlayerState<ABasePlayerState>();
	PS->AddItem(NewFish);
}

void AFishingRod::FishingFail()
{
	bIsFish = false;
}


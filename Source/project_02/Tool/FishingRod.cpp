// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"

#include "FishingFloat.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Helper/ItemHelper.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"


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

void AFishingRod::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		if (Float)
		{
			Float->Destroy();
		}
	}
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
	if (!bIsWaiting)
	{
		ChargeStart();
	}
}

void AFishingRod::OnInteractiveHold(float DeltaTime)
{
	Super::OnInteractiveHold();
	if (bIsCharging)
	{
		Charging(DeltaTime);
	}
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
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
		check(PC)
		
		PC->GetPlayerUI()->SetProgressPercent(static_cast<float>(Power) / MaxPower);
	}
}

void AFishingRod::ChargeEnd()
{
	bIsCharging = false;
	
	const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	ABasePlayerController* PC = Cast<ABasePlayerController>(Player->GetController());
	
	check(PC)
	
	PC->GetPlayerUI()->SetProgressPercent(0);
	
	Float = GetWorld()->SpawnActor<AFishingFloat>(FloatClass,RodPoint->GetComponentTransform());
	if (Float)
	{
		bIsWaiting = true;
		Float->SetFishingRod(this);
		Float->StartThrow(RodPoint->GetComponentLocation(),Power);
	}
}


void AFishingRod::StartFishing()
{
	bIsFish = true;
	bIsWaiting = false;
	UE_LOG(LogTemp,Display,TEXT("미끼를 물었다!"));
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


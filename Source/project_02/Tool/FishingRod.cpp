// Fill out your copyright notice in the Description page of Project Settings.


#include "FishingRod.h"

#include "FishingFloat.h"
#include "Kismet/GameplayStatics.h"


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
	ChargeEnd();

	return true;
}

void AFishingRod::ChargeStart()
{
	FishingPoint = RodPoint->GetComponentLocation();
	Power = 0.0f;
	bIsCharging = true;
}

void AFishingRod::Charging(float deltaTime)
{
	Power += deltaTime * 500.0f;
}

void AFishingRod::ChargeEnd()
{
	UE_LOG(LogTemp,Warning,TEXT("%f"),Power);
	bIsCharging = false;

	Float = GetWorld()->SpawnActor<AFishingFloat>(FloatClass,RodPoint->GetComponentTransform());
	if (Float)
	{
		Float->StartThrow(RodPoint->GetComponentLocation(),Power);
	}
}


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
	// UE_LOG(LogTemp,Warning,TEXT("낚시 차지 시작"));
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	FishingPoint = RodPoint->GetComponentLocation();
	bIsCharging = true;
}

void AFishingRod::Charging(float deltaTime)
{
	Power += deltaTime * 20.0f;
	if (Power < MaxPower)
	{
		FishingPoint = FishingPoint + Player->GetActorForwardVector()*Power;
	}
}

void AFishingRod::ChargeEnd()
{
	UE_LOG(LogTemp,Warning,TEXT("%f"),Power);
	bIsCharging = false;
	FishingPoint = FishingPoint + Player->GetActorForwardVector()*Power;
	FishingPoint.Z = 0.0f; // WaterHeight 입니다....ㅋㅋㅋ
	// DrawDebugSphere(GetWorld(), FishingPoint, 10.0f, 12, FColor::Red,false,2.0f,0,0.0f);
	Power = 0.0f;

	Float = GetWorld()->SpawnActor<AFishingFloat>(FloatClass,RodPoint->GetComponentTransform());
	if (Float)
	{
		Float->StartThrow(RodPoint->GetComponentLocation(),FishingPoint);
	}
}


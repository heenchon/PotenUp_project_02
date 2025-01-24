// Fill out your copyright notice in the Description page of Project Settings.


#include "Cup.h"

#include "MovieSceneTracksComponentTypes.h"
#include "WaterBodyOceanActor.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"

bool ACup::bIsFresh = false;
bool ACup::bIsSea = false;
bool ACup::bIsEmpty = true;
bool ACup::bLookingSea = false;

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

	Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SurvivalComponent = Player->FindComponentByClass<USurvivalComponent>();
	CamManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(),0);
}

// Called every frame
void ACup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsEmpty)
	{
		bLookingSea = IsLookingSea();
		//TODO: UI띄우기
	}
}

void ACup::Use()
{
	Super::Use();
	if (bIsFresh)
	{
		UE_LOG(LogTemp, Warning, TEXT("담수 먹기"));
		bIsEmpty = true; bIsFresh = false;
		WaterMesh->SetVisibility(false);
		SurvivalComponent->IncreaseThirst(10);
		return;
	}
	if (bIsSea)
	{
		UE_LOG(LogTemp, Warning, TEXT("바닷물 먹기"));
		bIsEmpty = true; bIsSea = false;
		WaterMesh->SetVisibility(false);
		SurvivalComponent->DecreaseThirst(10);
		return;
	}
	if (bLookingSea)
	{
		UE_LOG(LogTemp, Warning, TEXT("바닷물 채우기"));
		bIsEmpty = false; bIsSea = true;
		WaterMesh->SetVisibility(true);
	}
}

void ACup::FillSeaWater()
{
	if (bIsEmpty)
	{
		bIsEmpty = false;
		bIsSea = true;
		WaterMesh->SetMaterial(0,Ocean);
		WaterMesh->SetVisibility(true);
	}
}

void ACup::FillFreshWater()
{
	if (bIsEmpty)
	{
		bIsEmpty = false;
		bIsFresh = true;
		WaterMesh->SetMaterial(0,Fresh);
		WaterMesh->SetVisibility(true);
	}
}

void ACup::EmptyCup()
{
	bIsSea = false; bIsFresh = false;
	bIsEmpty = true;
	WaterMesh->SetVisibility(false);
}

bool ACup::IsLookingSea()
{
	FVector start = CamManager->GetCameraLocation();
	FVector end = CamManager->GetCameraLocation()+ CamManager->GetActorForwardVector()*3000.0f;

	FHitResult hit;
	TArray<AActor*> ActorsToIgnore;
	FCollisionQueryParams params;
	ActorsToIgnore.Add(Player);
	
	bool bHit = UKismetSystemLibrary::LineTraceSingle(GetWorld(), start, end,
		ETraceTypeQuery::TraceTypeQuery6,
		true,
		ActorsToIgnore, EDrawDebugTrace::None,
		hit,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		1
	);
	
	if (bHit)
	{
		AWaterBodyOcean* ocean = Cast<AWaterBodyOcean>(hit.GetActor());
		if (ocean)
		{
			return true;
		}
	}
	return false;
}


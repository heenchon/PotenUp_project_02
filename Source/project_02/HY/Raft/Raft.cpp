// Fill out your copyright notice in the Description page of Project Settings.


#include "Raft.h"
#include "project_02/Building/BuildingActor.h"
#include "../RaftGameState.h"
#include "../Objects/Sail.h"
#include "project_02/Building/BuildingFloor.h"

// Sets default values
ARaft::ARaft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->BodyInstance.bLockZRotation = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Water/Caustics/Meshes/CausticsPreviewBase.CausticsPreviewBase'"));

	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);
	
	Buoyancy->AddCustomPontoon(100,"one");
	Buoyancy->AddCustomPontoon(100,"two");
	Buoyancy->AddCustomPontoon(100,"three");
	Buoyancy->AddCustomPontoon(100,"four");
	Buoyancy->BuoyancyData.Pontoons[0].RelativeLocation = {50.0f,50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[1].RelativeLocation = {-50.0f,50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[2].RelativeLocation = {50.0f,-50.0f,75.f};
	Buoyancy->BuoyancyData.Pontoons[3].RelativeLocation = {-50.0f,-50.0f,75.f};

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Ignore);
}

// Called when the game starts or when spawned
void ARaft::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetHiddenInGame(true);
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	if (RaftGameState)
	{
		WindDirection = RaftGameState->WindDirection;
		WindStrength = RaftGameState->WindStrength;
	}
	
	if (ABuildingFloor* NewMainFloor = GetWorld()->SpawnActor<ABuildingFloor>(MainFloorClass))
	{
		MainFloor = NewMainFloor;
		MainFloor->SetCenter();
		MainFloor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
	}
	
	SpawnSailActor();
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}

void ARaft::SpawnSailActor()
{
	if (ASail* sail = GetWorld()->SpawnActor<ASail>(ASail::StaticClass()))
	{
		sail->AttachToActor(MainFloor, FAttachmentTransformRules::KeepRelativeTransform);
		sail->SetRaft(this);
	}
}

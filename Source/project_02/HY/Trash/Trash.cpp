// Fill out your copyright notice in the Description page of Project Settings.


#include "Trash.h"
#include "BuoyancyComponent.h"
#include "../Raft/Raft.h"


// Sets default values
ATrash::ATrash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetSimulatePhysics(true);
	
	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);
	
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	Buoyancy->AddCustomPontoon(100,"center");
}

// Called when the game starts or when spawned
void ATrash::BeginPlay()
{
	Super::BeginPlay();
	//TODO: 윈드 매니저 추가 후 cpp 수정
	if (Raft)
	{
		WindDirection=Raft->WindDirection;
		WindStrength=Raft->WindStrength;
	}
	else UE_LOG(LogTemp,Error,TEXT("폐품은 Raft를 찾고있어요.. detail 패널 설정 부탁"));
}

// Called every frame
void ATrash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation()+WindDirection*DeltaTime*WindStrength);
}


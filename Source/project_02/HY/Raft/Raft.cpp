// Fill out your copyright notice in the Description page of Project Settings.


#include "Raft.h"

// Sets default values
ARaft::ARaft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	
	// RaftMesh를 루트 컴포넌트로 설정
	RootComponent = StaticMesh;

	ConstructorHelpers::FObjectFinder<UStaticMesh>DefaultMesh(TEXT("/Script/Engine.StaticMesh'/Water/Caustics/Meshes/CausticsPreviewBase.CausticsPreviewBase'"));

	if (DefaultMesh.Succeeded()) StaticMesh->SetStaticMesh(DefaultMesh.Object);

	Buoyancy->AddCustomPontoon(100,"one");
	Buoyancy->AddCustomPontoon(100,"two");
	Buoyancy->AddCustomPontoon(100,"three");
	Buoyancy->AddCustomPontoon(100,"four");

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_PhysicsBody);
	StaticMesh->SetCollisionResponseToChannel(ECC_Pawn,ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_PhysicsBody,ECR_Ignore);
}

// Called when the game starts or when spawned
void ARaft::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}

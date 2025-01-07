// Fill out your copyright notice in the Description page of Project Settings.


#include "Raft.h"
#include "Sail.h"

// Sets default values
ARaft::ARaft()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RaftMesh"));
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	// RaftMesh를 루트 컴포넌트로 설정
	RootComponent = StaticMesh;

	Buoyancy->AddCustomPontoon(100,"one");
	Buoyancy->AddCustomPontoon(100,"two");
	Buoyancy->AddCustomPontoon(100,"three");
	Buoyancy->AddCustomPontoon(100,"four");
	
}

// Called when the game starts or when spawned
void ARaft::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;  // 부모 설정

	Sail = GetWorld()->SpawnActor<ASail>(ASail::StaticClass(), GetActorLocation(), GetActorRotation(), SpawnParams);

	if (Sail)
	{
		Sail->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		Sail->SetActorRelativeLocation(FVector(0.f, 0.f, 10.f)); 
		UE_LOG(LogTemp, Warning, TEXT("돛 부착조이고"));
	}
}

// Called every frame
void ARaft::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(GetActorLocation()+WindDirection*DeltaTime*WindStrength*SailStrength);
}


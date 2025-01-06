// Fill out your copyright notice in the Description page of Project Settings.


#include "Sail.h"


// Sets default values
ASail::ASail()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SailMesh = FindComponentByClass<UStaticMeshComponent>();
	SetMobility(EComponentMobility::Movable);
}

// Called when the game starts or when spawned
void ASail::BeginPlay()
{
	Super::BeginPlay();
	Raft = Cast<ARaft>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("래프트 가져왔나요? %s"), *(Raft->GetName()));
	WindDirection = Raft->WindDirection;
	WindStrength = Raft->WindStrength;
}

// Called every frame
void ASail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MyDirection = this->GetActorRotation().Vector();
	ChangeStrength(CompareDirection(MyDirection, WindDirection));
	// UE_LOG(LogTemp, Warning, TEXT("포워드 벡터: %s"), *MyDirection.ToString());
}

void ASail::ChangeStrength(float myStrength)
{
	Raft->SailStrength = myStrength;
}

float ASail::CompareDirection(FVector3d myDir, FVector3d windDir)
{
	float ForceMultiplier = FMath::Clamp(FVector::DotProduct(myDir, windDir), 0.2f, 1.0f);
	// UE_LOG(LogTemp, Warning, TEXT("바람 방향: %s  가중값: %f"), *windDir.ToString(), ForceMultiplier);
	return (MaxSailStrength * ForceMultiplier);
}




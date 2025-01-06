// Fill out your copyright notice in the Description page of Project Settings.


#include "Sail.h"


// Sets default values
ASail::ASail()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FindComponentByClass<UStaticMeshComponent>()->SetStaticMesh(LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/EditorMeshes/EditorPlane.EditorPlane")));
}

// Called when the game starts or when spawned
void ASail::BeginPlay()
{
	Super::BeginPlay();
	Raft = Cast<ARaft>(GetParentActor());
	WindDirection = Raft->WindDirection;
	WindStrength = Raft->WindStrength;
	UE_LOG(LogTemp, Display, TEXT("An Actor's name is %s"), *Raft->GetName());
}

// Called every frame
void ASail::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ChangeStrength(CompareDirection(GetActorForwardVector(), WindDirection));
	UE_LOG(LogTemp, Warning, TEXT("The vector value is: %s"), *GetActorForwardVector().ToString());
}

void ASail::ChangeStrength(float myStrength)
{
	Raft->SailStrength = myStrength;
}

float ASail::CompareDirection(FVector3d myDir, FVector3d windDir)
{
	float ForceMultiplier = FMath::Clamp(FVector::DotProduct(myDir, windDir), 0.0f, 1.0f);
	// UE_LOG(LogTemp, Warning, TEXT("The float value is: %f"), MaxSailStrength * ForceMultiplier);
	return (MaxSailStrength * ForceMultiplier);
}




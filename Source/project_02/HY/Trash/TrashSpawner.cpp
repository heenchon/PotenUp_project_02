// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashSpawner.h"

#include "Trash.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/HY/RaftGameState.h"


ATrashSpawner::ATrashSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	DestroyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DestroyCollision"));
}

ATrash* ATrashSpawner::SpawnPooledObject(FVector SpawnLocation, FRotator SpawnRotation)
{
	for (int32 i = 0; i < PooledObjects.Num(); i++)
	{
		if (PooledObjects[i] && !PooledObjects[i]->IsActorTickEnabled())
		{
			PooledObjects[i]->SetActorLocation(SpawnLocation);
			PooledObjects[i]->SetActorRotation(SpawnRotation);
			PooledObjects[i]->StaticMesh->SetSimulatePhysics(true);
			PooledObjects[i]->SetActorHiddenInGame(false);
			PooledObjects[i]->SetActorEnableCollision(true);
			PooledObjects[i]->SetActorTickEnabled(true);
			// UE_LOG(LogTemp, Display, TEXT("Index: %d, Name: %s, TickEnabled: %s, Hidden: %s"), 
		 //   i, 
		 //   *PooledObjects[i]->GetName(), 
		 //   PooledObjects[i]->IsActorTickEnabled() ? TEXT("true") : TEXT("false"),
		 //   PooledObjects[i]->IsHidden() ? TEXT("true") : TEXT("false"));
			return PooledObjects[i];
		}
	}
	return nullptr;
}

void ATrashSpawner::ReturnPooledObject(ATrash* ObjectToReturn)
{
	if (ObjectToReturn)
	{
		ObjectToReturn->SetActorHiddenInGame(true);
		ObjectToReturn->SetActorEnableCollision(false);
		ObjectToReturn->StaticMesh->SetSimulatePhysics(false);
		ObjectToReturn->SetActorTickEnabled(false);
	}
}

TSubclassOf<ATrash> ATrashSpawner::GetRandomTrash()
{
	if (TrashClasses.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, TrashClasses.Num() - 1);
		return TrashClasses[RandomIndex];
	}
	return nullptr;
}

FVector ATrashSpawner::UpdatePosition()
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	const ARaftGameState* GS = Cast<ARaftGameState>(UGameplayStatics::GetGameState(GetWorld()));
	if (Player)
	{
		FVector PlayerLocation = Player->GetActorLocation();
		FVector TargetLocation = PlayerLocation + GS->WindDirection*3000.0f;

		//TODO: 임시 코드. 바람 방향 변했을 때 고려 X.
		float randF= FMath::FRandRange(-2000.0f, 2000.0f);
		TargetLocation.X = TargetLocation.X + randF*0.5f;
		TargetLocation.Y = PlayerLocation.Y + randF;
		UE_LOG(LogTemp,Warning,TEXT("X: %f / Y: %f"), TargetLocation.X, TargetLocation.Y);

		FVector NewDestroyLocation = PlayerLocation + GS->WindDirection* -1000.0f;
		DestroyCollision->SetWorldLocation(NewDestroyLocation);
		
		return TargetLocation;
	}
	return FVector::ZeroVector;
}

void ATrashSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	for (int32 i = 0; i < PoolSize; i++)
	{
		TSubclassOf<ATrash> selectTrash = GetRandomTrash();
		if (selectTrash)
		{
			ATrash* NewObject = GetWorld()->SpawnActor<ATrash>(selectTrash);
			if (NewObject)
			{
				NewObject->SetActorEnableCollision(false);
				NewObject->SetActorHiddenInGame(true);
				NewObject->SetActorTickEnabled(false);
				PooledObjects.Add(NewObject);
			}
		}
	}
	
	for (int32 i = 0; i < 20; i++)
	{
		SpawnPooledObject(UpdatePosition(), FRotator(0.0f, 0.0f, 0.0f));
	}
}



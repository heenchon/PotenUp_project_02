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

void ATrashSpawner::MyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATrash* trash = Cast<ATrash>(OtherActor);
	if (trash)
	{
		ReturnPooledObject(trash);
	}
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
		FVector TargetLocation = PlayerLocation + GS->WindDirection*SpawnDistance;
		
		float randX= FMath::FRandRange(-X, X);
		float randY= FMath::FRandRange(-Y, Y);
		TargetLocation.X = TargetLocation.X + randX;
		TargetLocation.Y = PlayerLocation.Y + randY;

		FVector NewDestroyLocation = PlayerLocation + GS->WindDirection*(-DestroyDistance);
		DestroyCollision->SetWorldLocation(NewDestroyLocation);
		
		return TargetLocation;
	}
	return FVector::ZeroVector;
}

void ATrashSpawner::BeginPlay()
{
	Super::BeginPlay();
	DestroyCollision->OnComponentBeginOverlap.AddDynamic(this,&ATrashSpawner::MyOverlap);
	
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



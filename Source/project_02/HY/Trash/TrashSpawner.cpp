// Fill out your copyright notice in the Description page of Project Settings.


#include "TrashSpawner.h"

#include "Trash.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "project_02/HY/Raft/Raft.h"
#include "project_02/HY/RaftGameState.h"
#include "project_02/Player/BasePlayerController.h"

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
		DeactivePooledObject(trash);
	}
}

void ATrashSpawner::Initialize()
{
	DestroyCollision->OnComponentBeginOverlap.AddDynamic(this,&ATrashSpawner::MyOverlap);
	
	BaseRaft = GetWorld()->GetFirstPlayerController<ABasePlayerController>()->GetPlayerRaft();
	GS = Cast<ARaftGameState>(UGameplayStatics::GetGameState(GetWorld()));
	
	PooledObjects.Empty(PoolSize);
	for (int32 i = 0; i < PoolSize; i++)
	{
		PooledObjects.Add(NewTrashSpawn());
	}
	
	UpdateDestroyLocation();
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATrashSpawner::UpdatePooledObject,SpawnDuration,true);
}

void ATrashSpawner::UpdatePooledObject()
{
	ActivePooledObject(GetRandomLocation(),FRotator(0.0f));
	ActivePooledObject(GetRandomLocation(),FRotator(0.0f));
	ActivePooledObject(GetRandomLocation(),FRotator(0.0f));
	UpdateDestroyLocation();
}

ATrash* ATrashSpawner::ActivePooledObject(FVector SpawnLocation, FRotator SpawnRotation)
{
	for (int32 i = 0; i < PooledObjects.Num(); i++)
	{
		if (PooledObjects[i])
		{
			if (PooledObjects[i] && !PooledObjects[i]->IsActorTickEnabled())
			{
				PooledObjects[i]->SetActorLocation(SpawnLocation);
				PooledObjects[i]->SetActorRotation(SpawnRotation);
				PooledObjects[i]->SetActorHiddenInGame(false);
				PooledObjects[i]->StaticMesh->SetSimulatePhysics(true);
				PooledObjects[i]->SetActorEnableCollision(true);
				PooledObjects[i]->SetActorTickEnabled(true);
				return PooledObjects[i];
			}
		}
	}
	return nullptr;
}

void ATrashSpawner::DeactivePooledObject(ATrash* ObjectToReturn)
{
	if (ObjectToReturn)
	{
		ObjectToReturn->SetActorHiddenInGame(true);
		ObjectToReturn->StaticMesh->SetSimulatePhysics(false);
		ObjectToReturn->SetActorEnableCollision(false);
		ObjectToReturn->SetActorTickEnabled(false);
	}
}

TSubclassOf<ATrash> ATrashSpawner::GetRandomTrash()
{
	if (TrashClasses.Num() > 0)
	{
		int32 randIndex = FMath::RandRange(0, TrashClasses.Num() - 1);
		return TrashClasses[randIndex];
	}
	return nullptr;
}

FVector ATrashSpawner::GetRandomLocation() // 오브젝트 스폰할 랜덤 위치 지정
{
	if (BaseRaft)
	{
		FVector RaftLocation = BaseRaft->GetActorLocation();
		FVector targetLoc = RaftLocation + GS->WindDirection*SpawnDistance;
		
		float randX= FMath::FRandRange(-X, X);
		float randY= FMath::FRandRange(-Y, Y);
		
		return FVector(targetLoc.X + randX, targetLoc.Y + randY, 15.0f);
	}
	return FVector::ZeroVector;
}

void ATrashSpawner::RespawnTrashAt(ATrash* Trash)
{
	int index = PooledObjects.Find(Trash);
	if (index >= 0 && index < PooledObjects.Num())
	{
		PooledObjects[index]=nullptr;
		PooledObjects[index]=NewTrashSpawn();
		// UE_LOG(LogTemp,Display,TEXT("쓰레기 다시 만들기"));
	}
}

void ATrashSpawner::UpdateDestroyLocation()
{
	if (BaseRaft)
	{
		FVector newDestroyLoc = BaseRaft->GetActorLocation() + GS->WindDirection * -1.0f*(SpawnDistance+DestroyDistance);
		DestroyCollision->SetWorldLocation(FVector(newDestroyLoc.X, newDestroyLoc.Y, 0.0f));
	}
}

ATrash* ATrashSpawner::NewTrashSpawn()
{
	TSubclassOf<ATrash> selectTrash = GetRandomTrash();
	
	ATrash* newObject = GetWorld()->SpawnActor<ATrash>(selectTrash,FVector(0.0f,0.0f,0.0f),FRotator(0.0f));
	if (newObject)
	{
		newObject->StaticMesh->SetSimulatePhysics(false);
		newObject->SetActorEnableCollision(false);
		newObject->SetActorHiddenInGame(true);
		newObject->SetActorTickEnabled(false);
		return newObject;
	}
	return nullptr;
}


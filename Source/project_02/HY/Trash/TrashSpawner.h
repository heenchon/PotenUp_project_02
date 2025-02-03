// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashSpawner.generated.h"

class UBoxComponent;
class ATrash;
class APawn;
class ARaftGameState;

UCLASS()
class PROJECT_02_API ATrashSpawner : public AActor
{
	GENERATED_BODY()

public:
	ATrashSpawner();
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* DestroyCollision;

	UPROPERTY()
	APawn* Player;
	UPROPERTY()
	const ARaftGameState* GS;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int32 PoolSize = 40;
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	float SpawnDuration = 5.0f;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool|Trash List")
	TArray<TSubclassOf<ATrash>> TrashClasses;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool|Random Spawn Range")
	float X = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Object Pool|Random Spawn Range")
	float Y = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "Object Pool|Distance")
	float SpawnDistance = 3000.0f;
	UPROPERTY(EditAnywhere, Category = "Object Pool|Distance")
	float DestroyDistance = 1000.0f;
	
protected:
	virtual void BeginPlay() override;

private:
	TArray<ATrash*> PooledObjects;
	FTimerHandle SpawnTimer;

public:
	UFUNCTION()
	void MyOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	ATrash* NewTrashSpawn();
	UFUNCTION()
	ATrash* ActivePooledObject(FVector SpawnLocation, FRotator SpawnRotation);
	UFUNCTION()
	void DeactivePooledObject(ATrash* ObjectToReturn);

	void UpdatePooledObject();
	void UpdateDestroyLocation();
	TSubclassOf<ATrash> GetRandomTrash();
	FVector GetRandomLocation();

	void RespawnTrashAt(ATrash* Trash);
};

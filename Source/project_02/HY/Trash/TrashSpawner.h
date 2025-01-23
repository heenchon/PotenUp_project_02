// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashSpawner.generated.h"

class UBoxComponent;
class ATrash;
UCLASS()
class PROJECT_02_API ATrashSpawner : public AActor
{
	GENERATED_BODY()

public:
	ATrashSpawner();
	
	UPROPERTY(EditAnywhere, Category = "Object Pool")
	int32 PoolSize = 40;
	
	UPROPERTY(EditAnywhere, Category = "Object Pool|Trash List")
	TArray<TSubclassOf<ATrash>> TrashClasses;
	TSubclassOf<ATrash> GetRandomTrash();
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* DestroyCollision;
	
protected:
	virtual void BeginPlay() override;

private:
	TArray<ATrash*> PooledObjects;

public:
	UFUNCTION(BlueprintCallable)
	ATrash* SpawnPooledObject(FVector SpawnLocation, FRotator SpawnRotation);
	UFUNCTION(BlueprintCallable)
	void ReturnPooledObject(ATrash* ObjectToReturn);

	FVector UpdatePosition();
	
};

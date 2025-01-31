// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BrokenFloor.generated.h"

UCLASS()
class PROJECT_02_API ABrokenFloor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABrokenFloor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
};

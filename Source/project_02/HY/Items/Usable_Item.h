// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Usable_Item.generated.h"

UCLASS()
class PROJECT_02_API AUsable_Item : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUsable_Item();
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	bool bUsable =true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Use();
	virtual void ApplyTo();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookRope.generated.h"

class UCableComponent;

UCLASS()
class PROJECT_02_API AHookRope : public AActor
{
	GENERATED_BODY()

public:
	AHookRope();

	void ThrowHook();

protected:
	virtual void BeginPlay() override;

	// virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HookItem", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCableComponent> Cable;
	
	UPROPERTY(EditDefaultsOnly, Category = "HookItem", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> HookMesh;
};

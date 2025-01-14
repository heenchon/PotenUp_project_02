// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaftGameState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_02_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	FVector WindDirection = FVector(1, 0, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	float WindStrength = 60.0f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	// float SailStrength = 1.5f;

	UFUNCTION(BlueprintCallable, Category = "Wind")
	void SetWind(FVector newDir, float newStrength);
	
};

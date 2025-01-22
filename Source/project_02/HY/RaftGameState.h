// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RaftGameState.generated.h"

class UBuildingInstance;

UCLASS()
class PROJECT_02_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	FVector WindDirection = FVector(1, 0, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	float WindStrength = 40.0f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	// float SailStrength = 1.5f;
	
	UFUNCTION(BlueprintCallable, Category = "Wind")
	void SetWind(FVector newDir, float newStrength);

	FORCEINLINE TObjectPtr<UBuildingInstance> GetBuildingInstance() const { return BuildingInstance; }

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
	TObjectPtr<UBuildingInstance> BuildingInstance;
};

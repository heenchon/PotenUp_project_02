// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RaftTop.generated.h"

class ARaftGameState;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API URaftTop : public UStaticMeshComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	URaftTop();

	UPROPERTY(EditAnywhere)
	class ARaftGameState* RaftGameState;
	
	UPROPERTY(EditAnywhere, Category = "Move")
	FVector WindDirection;
	UPROPERTY(EditAnywhere, Category = "Move")
	float WindStrength;
	UPROPERTY(EditAnywhere, Category = "Move")
	float SailStrength;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "RaftTop.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API URaftTop : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	URaftTop();
	
	UPROPERTY(EditAnywhere, Category = "Move")
	FVector WindDirection = {1,0,0};

	//TODO: 윈드 매니저 추가 후 cpp 수정
	UPROPERTY(EditAnywhere, Category = "Move")
	float WindStrength = 150.0f;
	UPROPERTY(EditAnywhere, Category = "Move")
	float SailStrength = 1.0f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,FActorComponentTickFunction* ThisTickFunction) override;

};

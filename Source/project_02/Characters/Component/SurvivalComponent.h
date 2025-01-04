// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivalComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API USurvivalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USurvivalComponent();

	void SetHealth(const uint8 NewValue);
	
	void SetHunger(const uint8 NewValue);

	void SetThirst(const uint8 NewValue);
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = true))
	FDataTableRowHandle PlayerInfoData;
	
	TPair<uint8, uint8> HealthInfo;
	
	TPair<uint8, uint8> HungerInfo;
	
	TPair<uint8, uint8> ThirstInfo;
};

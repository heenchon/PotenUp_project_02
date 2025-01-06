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

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsDied() const { return IsDied; }
	
	void AddDamage(const uint8 NewValue);
	
	UFUNCTION()
	void DecreaseHunger(const uint8 NewValue);
	UFUNCTION()
	void DecreaseThirst(const uint8 NewValue);
	
protected:
	virtual void BeginPlay() override;

private:
	bool IsDied = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = true))
	FDataTableRowHandle EntityInfo;
	
	TPair<uint8, uint8> HealthInfo;
	
	TPair<uint8, uint8> HungerInfo;
	FTimerHandle HungerDecreaseTimer;
	
	TPair<uint8, uint8> ThirstInfo;
	FTimerHandle ThirstDecreaseTimer;

	void InitialSurvivalData();
	
	void OnChangedHealth();
	void OnChangedHunger();
	void OnChangedThirst();
};

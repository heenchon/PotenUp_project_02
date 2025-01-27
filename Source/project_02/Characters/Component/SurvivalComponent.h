#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivalComponent.generated.h"

class UEntityStatInfo;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API USurvivalComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USurvivalComponent();

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsDied() const { return IsDied; }
	FORCEINLINE TPair<uint8, uint8> GetHealthMap() const { return HealthInfo; }
	FORCEINLINE TPair<uint8, uint8> GetHungerMap() const { return HungerInfo; }
	FORCEINLINE TPair<uint8, uint8> GetThirstMap() const { return ThirstInfo; }

	void SetHealth(const uint8 NewValue);
	void SetHunger(const uint8 NewValue);
	void SetThirst(const uint8 NewValue);
	
	void AddDamage(const uint8 NewValue);
	
	UFUNCTION()
	void DecreaseHunger(const uint8 NewValue);
	UFUNCTION()
	void IncreaseHunger(const uint8 NewValue);
	UFUNCTION()
	void DecreaseThirst(const uint8 NewValue);
	UFUNCTION()
	void IncreaseThirst(const uint8 NewValue);
	
	void InitialSurvivalData();
	
protected:
	virtual void BeginPlay() override;

private:
	bool IsDied = false;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data", meta = (AllowPrivateAccess = true))
	TObjectPtr<UEntityStatInfo> EntityStatInfo;
	
	TPair<uint8, uint8> HealthInfo;
	
	TPair<uint8, uint8> HungerInfo;
	FTimerHandle HungerDecreaseTimer;
	
	TPair<uint8, uint8> ThirstInfo;
	FTimerHandle ThirstDecreaseTimer;
	
	void OnChangedHealth();
	void OnChangedHunger();
	void OnChangedThirst();
};

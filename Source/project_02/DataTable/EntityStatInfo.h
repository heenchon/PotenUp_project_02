#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntityStatInfo.generated.h"


UCLASS()
class PROJECT_02_API UEntityStatInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	uint8 MaxHealth;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 MaxHunger;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 MaxThirst;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 HealthDecreaseTime;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 HungerDecreaseGage;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 ThirstDecreaseGage;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 HungerDecreaseTime;
	
	UPROPERTY(EditDefaultsOnly)
	uint8 ThirstDecreaseTime;
};

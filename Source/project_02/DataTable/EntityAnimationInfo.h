﻿#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EntityAnimationInfo.generated.h"

UCLASS()
class PROJECT_02_API UEntityAnimationInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> DiedAnimation;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AttackAnimation;
};

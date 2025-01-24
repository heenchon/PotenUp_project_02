#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Spear.generated.h"

UCLASS()
class PROJECT_02_API ASpear : public AWeaponBase
{
	GENERATED_BODY()

public:
	ASpear();

	virtual void Attack() override;
	
protected:
	virtual void BeginPlay() override;
};

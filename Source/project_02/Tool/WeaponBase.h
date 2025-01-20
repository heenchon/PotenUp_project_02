#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class PROJECT_02_API AWeaponBase : public AActor
{
	GENERATED_BODY()

public:
	AWeaponBase();

	virtual void Attack();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> WeaponBody;
};

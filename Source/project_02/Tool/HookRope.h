#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookRope.generated.h"

class AInteractiveHook;
class AHook;
class UCableComponent;
class USphereComponent;
class UBuoyancyComponent;

UCLASS()
class PROJECT_02_API AHookRope : public AActor
{
	GENERATED_BODY()

public:
	AHookRope();

	UFUNCTION()
	void OnHoldInteractive();
	
	UFUNCTION()
	void OnEndInteractive();
	
protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<AInteractiveHook> ControlledHook;
	
	uint8 Power = 0;
	uint8 MaxPower = 100;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCableComponent> Cable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AInteractiveHook> HookItem;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultRoot;
};

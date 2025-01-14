#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookRope.generated.h"

class UInputAction;
class AInteractiveHook;
class AHook;
class UCableComponent;
class USphereComponent;
class UBuoyancyComponent;

UENUM()
enum class EInteractiveToolStatus : uint8
{
	Idle,
	Interacting,
	Completed,
};

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

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	EInteractiveToolStatus Status = EInteractiveToolStatus::Idle;
	
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

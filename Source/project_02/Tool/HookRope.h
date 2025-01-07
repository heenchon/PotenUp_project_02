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

	// void ThrowHook();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UCableComponent> Cable;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<AInteractiveHook> HookItem;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultRoot;
};

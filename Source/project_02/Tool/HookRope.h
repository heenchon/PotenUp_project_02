#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HookRope.generated.h"

class AHook;
class UCableComponent;

UCLASS()
class PROJECT_02_API AHookRope : public AActor
{
	GENERATED_BODY()

public:
	AHookRope();

	void ThrowHook();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "HookItem", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCableComponent> Cable;
	
	UPROPERTY(EditDefaultsOnly, Category = "HookItem", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> HookMesh;
	
	UPROPERTY(EditDefaultsOnly, Category = "HookItem", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USceneComponent> DefaultRoot;
};

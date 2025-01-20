#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "HookTool.generated.h"

class AInteractiveHook;
class UCableComponent;

UCLASS()
class PROJECT_02_API AHookTool : public AInteractiveItem
{
	GENERATED_BODY()

public:
	AHookTool();

	virtual void OnInteractiveHold() override;

	virtual bool EndInteractive() override;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
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

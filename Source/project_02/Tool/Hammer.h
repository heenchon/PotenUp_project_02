#pragma once

#include "CoreMinimal.h"
#include "InteractiveItem.h"
#include "Hammer.generated.h"

UCLASS()
class PROJECT_02_API AHammer : public AInteractiveItem
{
	GENERATED_BODY()

public:
	AHammer();

	virtual void StartInteractive() override;

	virtual void OnAttached() override;
	
protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> DefaultRoot;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;
};

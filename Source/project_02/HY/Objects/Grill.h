#pragma once

#include "CoreMinimal.h"
#include "PlaceObjects.h"
#include "Grill.generated.h"


UCLASS()
class PROJECT_02_API AGrill : public APlaceObjects
{
	GENERATED_BODY()

public:
	AGrill();

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* FoodPoint;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* RawFoodMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AUsable_Item> FishCookedTemp;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundWave> GrillingSound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> GrillingSoundComponent;
	
	bool bIsCooking;
	bool bIsCooked = false;
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Interact(AUsable_Item* input, int curItemIndex) override;
	virtual void ProcessComplete() override;

	virtual FString GetDisplayText() const override;
};

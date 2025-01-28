#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "project_02/DataTable/BuildData.h"
#include "RaftGameState.generated.h"

class APlaceObjects;
class ABuildingActor;

UCLASS()
class PROJECT_02_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	FVector WindDirection = FVector(1, 0, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	float WindStrength = 40.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Wind")
	void SetWind(FVector newDir, float newStrength);
};

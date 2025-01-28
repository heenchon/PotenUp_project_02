#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaftGameMode.generated.h"

UCLASS()
class PROJECT_02_API ARaftGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	FORCEINLINE FString GetMapName() const { return MapName; }
	void StartPlayGame(const FString& NewMapName);

protected:
	virtual void BeginPlay() override;

private:
	FString MapName;
	
	bool IsLoading = false;

	UFUNCTION()
	void OnLevelLoadComplete();
};

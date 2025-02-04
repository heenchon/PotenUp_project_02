#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaftGameMode.generated.h"

class ULoadingUI;

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
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<ULoadingUI> LoadingUIClass;

	UPROPERTY()
	TObjectPtr<ULoadingUI> LoadingUI;
	
	FString MapName;
	
	bool IsLoading = false;
	
	FTimerHandle LoadingEndTimer;

	UFUNCTION()
	void OnLevelLoadComplete();

	void SaveMapNameList();

	void MoveToGame();
};

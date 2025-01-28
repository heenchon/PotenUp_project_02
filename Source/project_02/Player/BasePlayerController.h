#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class URaftSaveGame;
class UMainUI;
class APlayerCharacter;
class UInventorySlot;
class UPlayerRespawnUI;
class UPlayerGameUI;

UCLASS()
class PROJECT_02_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UPlayerGameUI> GetPlayerUI() { return PlayUI; }
	FORCEINLINE TObjectPtr<UMainUI> GetMainUI() { return MainUI; }
	FORCEINLINE TObjectPtr<URaftSaveGame> GetRecentSaveData() { return RecentSaveData; }

	void OnDied();
	
	void Respawn();

	FORCEINLINE void SetSelectedSlot(const TObjectPtr<UInventorySlot>& NewSlot)
	{
		SelectedInventorySlot = NewSlot;
	}
	
	void RemoveDraggedSelectedSlot();

	void SaveGame();
	
	void LoadGame();

	void Initialize();

	void ShowMainUI();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Player", meta = (AllowPrivateAccess = true))
	TSubclassOf<APlayerCharacter> PlayerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UMainUI> MainUIClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerGameUI> PlayUIClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerRespawnUI> PlayerRespawnUIClass;

	UPROPERTY()
	TObjectPtr<UMainUI> MainUI;
	
	UPROPERTY()
	TObjectPtr<UPlayerGameUI> PlayUI;
	
	UPROPERTY()
	TObjectPtr<UPlayerRespawnUI> PlayerRespawnUI;
	
	UPROPERTY()
	TObjectPtr<UInventorySlot> SelectedInventorySlot;

	UPROPERTY()
	TObjectPtr<URaftSaveGame> RecentSaveData;
};

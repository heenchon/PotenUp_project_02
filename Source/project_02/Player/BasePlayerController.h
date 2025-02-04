#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

class ARaft;
class APlayerCharacter;
class UMainUI;
class URaftSaveGame;
class UPlayerGameUI;
class UInventorySlot;
class UPlayerRespawnUI;

UCLASS()
class PROJECT_02_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UPlayerGameUI> GetPlayerUI() { return PlayUI; }
	FORCEINLINE TObjectPtr<UMainUI> GetMainUI() { return MainUI; }
	FORCEINLINE TObjectPtr<URaftSaveGame> GetRecentSaveData() { return RecentSaveData; }
	FORCEINLINE TObjectPtr<ARaft> GetPlayerRaft() const { return Raft; }

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
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Player", meta = (AllowPrivateAccess = true))
	FVector DefaultSpawnPoint = {-23900, 0, 350};
	
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

	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	TSubclassOf<ARaft> RaftClass;

	UPROPERTY()
	TObjectPtr<ARaft> Raft;

	void InitializeData();

	void SpawnRaft();
};

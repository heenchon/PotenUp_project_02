#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

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

	void OnDied();
	
	void Respawn();

	FORCEINLINE void SetSelectedSlot(const TObjectPtr<UInventorySlot>& NewSlot)
	{
		SelectedInventorySlot = NewSlot;
	}
	
	void RemoveDraggedSelectedSlot();

	void SaveGame();

	void Initialize();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Player", meta = (AllowPrivateAccess = true))
	TSubclassOf<APlayerCharacter> PlayerClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerGameUI> PlayUIClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerRespawnUI> PlayerRespawnUIClass;

	UPROPERTY()
	TObjectPtr<UPlayerGameUI> PlayUI;
	
	UPROPERTY()
	TObjectPtr<UPlayerRespawnUI> PlayerRespawnUI;
	
	UPROPERTY()
	TObjectPtr<UInventorySlot> SelectedInventorySlot;
};

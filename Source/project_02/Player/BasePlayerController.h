#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

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
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerGameUI> PlayUIClass;

	UPROPERTY()
	TObjectPtr<UPlayerGameUI> PlayUI;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UPlayerRespawnUI> PlayerRespawnUIClass;

	UPROPERTY()
	TObjectPtr<UPlayerRespawnUI> PlayerRespawnUI;
};

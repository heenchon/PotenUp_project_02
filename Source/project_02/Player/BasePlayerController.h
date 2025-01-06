#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"


UCLASS()
class PROJECT_02_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	FORCEINLINE TObjectPtr<UUserWidget> GetPlayerUI() { return PlayUI; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "UI", meta = (AllowPrivateAccess = true))
	TSubclassOf<UUserWidget> PlayUIClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> PlayUI;
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaftGameMode.generated.h"

UCLASS()
class PROJECT_02_API ARaftGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void GoToMainLevel();

protected:
	virtual void BeginPlay() override;

private:
	bool IsLoading = false;
};

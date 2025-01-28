#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RaftSaveList.generated.h"

UCLASS()
class PROJECT_02_API URaftSaveList : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FString> MapNameList;
};

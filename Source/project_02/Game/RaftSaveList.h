#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RaftSaveList.generated.h"

USTRUCT()
struct FSaveData
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString MapName;

	UPROPERTY()
	FDateTime LastPlayDateTime;
};

UCLASS()
class PROJECT_02_API URaftSaveList : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FSaveData> MapNameList;
};

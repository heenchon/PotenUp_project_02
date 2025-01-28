#include "RaftGameMode.h"

#include "RaftSaveList.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Player/BasePlayerController.h"

void ARaftGameMode::BeginPlay()
{
	// 메인 화면 UI Level 노출 처리
	UGameplayStatics::LoadStreamLevel(GetWorld(), "MainLevel",
	true, true, FLatentActionInfo());
	
	ABasePlayerController* MainPC = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	MainPC->ShowMainUI();
}

void ARaftGameMode::StartPlayGame(const FString& NewMapName)
{
	MapName = NewMapName;
	IsLoading = true;

	if (URaftSaveList* NewSaveGame = Cast<URaftSaveList>(
		UGameplayStatics::CreateSaveGameObject(URaftSaveList::StaticClass())))
	{
		
		const URaftSaveList* SaveData = Cast<URaftSaveList>(
						UGameplayStatics::LoadGameFromSlot("SaveList", 0));

		bool IsUpdate = false;
		
		// 이미 세이브 정보가 있는 경우에 대한 대응
		if (SaveData)
		{
			const int32 FindDuplicatedMapIndex = SaveData->MapNameList.IndexOfByPredicate([&](const FSaveData& Data)
				{
					return Data.MapName == NewMapName;
				});

			// 이미 있는 정보면 그냥 바로 저장한다.
			if (FindDuplicatedMapIndex != -1)
			{
				NewSaveGame->MapNameList.Append(SaveData->MapNameList);
				NewSaveGame->MapNameList[FindDuplicatedMapIndex].LastPlayDateTime = FDateTime::Now();
				UGameplayStatics::SaveGameToSlot(NewSaveGame, "SaveList", 0);

				IsUpdate = true;
			}
		}

		// 없는 값이기 때문에 새로 만들때의 경우에 대한 대응
		if (!IsUpdate)
		{
			FSaveData NewSaveData;
			NewSaveData.MapName = NewMapName;
			NewSaveData.LastPlayDateTime = FDateTime::Now();

			if (SaveData)
			{
				NewSaveGame->MapNameList.Append(SaveData->MapNameList);
			}
			NewSaveGame->MapNameList.Add(NewSaveData);
		
			UGameplayStatics::SaveGameToSlot(NewSaveGame, "SaveList", 0);
		}
		
		FLatentActionInfo LatentInfo;
		LatentInfo.CallbackTarget = this;
		LatentInfo.ExecutionFunction = FName("OnLevelLoadComplete");
		LatentInfo.Linkage = 0;
	
		// 동기 처리기 때문에 이 동작이 완료되지 않는 이상 아래 로직이 수행되지 않는다.
		UGameplayStatics::LoadStreamLevel(GetWorld(), "Ocean_Test",
		true, true, LatentInfo);
	
		// 메인 UI는 그냥 비동기로 UnLoad해도 문제는 없어보인다.
		UGameplayStatics::UnloadStreamLevel(GetWorld(), "MainLevel", FLatentActionInfo(), false);
	}
}

void ARaftGameMode::OnLevelLoadComplete()
{
	ABasePlayerController* MainPC = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	MainPC->Initialize();
	
	IsLoading = false;
}
#include "RaftGameMode.h"

#include "RaftSaveList.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/Lobby/LoadingUI.h"

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
	LoadingUI = CreateWidget<ULoadingUI>(GetWorld()->GetFirstPlayerController(), LoadingUIClass);

	LoadingUI->AddToViewport();
	LoadingUI->PlayStart();

	SaveMapNameList();
	
	LoadingUI->PlayEnd();
	
	GetWorld()->GetTimerManager().SetTimer(LoadingEndTimer,
		FTimerDelegate::CreateUObject(this, &ARaftGameMode::MoveToGame), 4, false);
}

void ARaftGameMode::MoveToGame()
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("OnLevelLoadComplete");
	LatentInfo.Linkage = 0;
	
	// 메인 맵의 크기가 크기 때문에 동기 처리가 필요하다. 그동안 로딩 UI를 노출시킨다.
	UGameplayStatics::UnloadStreamLevel(GetWorld(), "MainLevel", FLatentActionInfo(), true);
	
	// 동기 처리기 때문에 이 동작이 완료되지 않는 이상 아래 로직이 수행되지 않는다.
	UGameplayStatics::LoadStreamLevel(GetWorld(), "OceanLevel",
	true, true, LatentInfo);
}


void ARaftGameMode::SaveMapNameList()
{
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
					return Data.MapName == MapName;
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
			NewSaveData.MapName = MapName;
			NewSaveData.LastPlayDateTime = FDateTime::Now();

			if (SaveData)
			{
				NewSaveGame->MapNameList.Append(SaveData->MapNameList);
			}
			NewSaveGame->MapNameList.Add(NewSaveData);
		
			UGameplayStatics::SaveGameToSlot(NewSaveGame, "SaveList", 0);
		}
	}
}

void ARaftGameMode::OnLevelLoadComplete()
{
	ABasePlayerController* MainPC = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	MainPC->Initialize();
	
	IsLoading = false;
}
#include "BasePlayerController.h"

#include "BasePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SurvivalComponent.h"
#include "project_02/Game/RaftGameMode.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"
#include "project_02/Widgets/HUD/PlayerRespawnUI.h"
#include "project_02/Widgets/Lobby/MainUI.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"
#include "project_02/Game/RaftSaveGame.h"
#include "project_02/HY/RaftGameState.h"
#include "project_02/HY/Raft/Raft.h"

void ABasePlayerController::Initialize()
{
	// 초기화 시에는 마우스 커서 없애는 작업 수행
	SetShowMouseCursor(false);
	LoadGame();
	
	if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
	{
		PS->InitializeData();
	}

	InitializeData();
	SpawnRaft();
}

void ABasePlayerController::InitializeData()
{
	// 플레이어 스폰 로직
	APawn* PrevPawn = GetPawn();

	FTransform SpawnTransform;
	
	if (RecentSaveData->IsAlreadyStart)
	{
		SpawnTransform.SetLocation(FVector(0, 0, 100));
	} else
	{
		SpawnTransform = RecentSaveData->LastPlayerTransform;
	}
	
	if (APlayerCharacter* PlayerPawn = GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass, SpawnTransform))
	{
		Possess(PlayerPawn);
		// 이전 Pawn은 제거한다.
		PrevPawn->Destroy();

		if (MainUI)
		{
			MainUI->RemoveFromParent();
		}
	
		if (PlayUIClass)
		{
			PlayUI = CreateWidget<UPlayerGameUI>(this, PlayUIClass);
			PlayUI->AddToViewport();
		}
	}
}

void ABasePlayerController::SpawnRaft()
{
	// 배가 이미 있으면 아무것도 하지 않는다.
	if (IsValid(Raft))
	{
		return;
	}
	
	if (RaftClass)
	{
		Raft = GetWorld()->SpawnActor<ARaft>(RaftClass);
	}
}


void ABasePlayerController::ShowMainUI()
{
	if (MainUIClass)
	{
		MainUI = CreateWidget<UMainUI>(this, MainUIClass);
		MainUI->AddToViewport();
		SetShowMouseCursor(true);
	}
}

void ABasePlayerController::OnDied()
{
	if (PlayerRespawnUIClass)
	{
		PlayerRespawnUI = CreateWidget<UPlayerRespawnUI>(this, PlayerRespawnUIClass);
		PlayerRespawnUI->AddToViewport();
		SetShowMouseCursor(true);
	}
}

void ABasePlayerController::Respawn()
{
	const APlayerCharacter* PrevPlayer = Cast<APlayerCharacter>(GetPawn());
	PrevPlayer->GetSurvivalComponent()->InitialSurvivalData();
	PlayerRespawnUI->RemoveFromParent();
	SetShowMouseCursor(false);
}

void ABasePlayerController::RemoveDraggedSelectedSlot()
{
	if (SelectedInventorySlot)
	{
		SelectedInventorySlot->RemoveDragDropSlot();
		SelectedInventorySlot = nullptr;
	}
}

// 데이터를 로드함.
void ABasePlayerController::LoadGame()
{
	// 로드된 데이터 정보가 있는 경우
	if (URaftSaveGame* RaftSaveGame = Cast<URaftSaveGame>(UGameplayStatics::LoadGameFromSlot(
		GetWorld()->GetAuthGameMode<ARaftGameMode>()->GetMapName(), 0)))
	{
		// 이동연산자를 활용해 값만 넣어주고, 이전 값은 제거해준다
		RecentSaveData = MoveTemp(RaftSaveGame);
	} else
	{
		// 실패 시 새로운 데이터 저장
		if (URaftSaveGame* SaveGame = Cast<URaftSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URaftSaveGame::StaticClass())))
		{
			FAsyncSaveGameToSlotDelegate SavedDelegate;

			if (const APlayerCharacter* PlayerInfo = GetPawn<APlayerCharacter>())
			{
				SaveGame->CurrentHealth =
					PlayerInfo->GetSurvivalComponent()->GetHealthMap().Key;
				SaveGame->MaxHealth =
					PlayerInfo->GetSurvivalComponent()->GetHealthMap().Value;
			
				SaveGame->CurrentHunger =
					PlayerInfo->GetSurvivalComponent()->GetHungerMap().Key;
				SaveGame->MaxHunger =
					PlayerInfo->GetSurvivalComponent()->GetHungerMap().Value;
			
				SaveGame->CurrentThirst =
					PlayerInfo->GetSurvivalComponent()->GetThirstMap().Key;
				SaveGame->MaxThirst =
					PlayerInfo->GetSurvivalComponent()->GetThirstMap().Value;
			}

			if (const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
			{
				SaveGame->PlayerInventoryList.Append(PS->GetPlayerInventoryList());
			}

			if (const ARaftGameState* GameState =
				GetWorld()->GetGameState<ARaftGameState>())
			{
				SaveGame->RaftBuildMetaData.Append(Raft->GetRaftBuildMetaData());

				for (TTuple<FVector, TArray<FPlacedObjectData>> RaftPlacedObjectData : Raft->GetRaftPlacedObjectData())
				{
					FPlacedObjectDataArray DataArray;
					DataArray.ObjectArray.Append(RaftPlacedObjectData.Value);
					SaveGame->RaftPlacedObjectMetaData.Add(RaftPlacedObjectData.Key, DataArray);
				}
			}
		
			UGameplayStatics::SaveGameToSlot(SaveGame, GetWorld()->GetAuthGameMode<ARaftGameMode>()->GetMapName(), 0);
			// 이동 연산자로 방금 저장한 정보를 다시 불러옴
			RecentSaveData = MoveTemp(SaveGame);
		}
	}
}

// 데이터를 새로 저장함, 이 코드는 보통 게임 시작 전이 아닌 인게임 중 사용된다.
void ABasePlayerController::SaveGame()
{
	// 기존 정보를 무시하고 데이터를 새로 저장함
	if (URaftSaveGame* SaveGame = Cast<URaftSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URaftSaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;

		// 여기는 임시로 하드코딩 처럼 들어가는데, 한번 저장할 때는 무조건 튜토리얼 완료,
		// 즉 게임 시작을 이미 하였음을 의미한다.
		SaveGame->IsAlreadyStart = true;
		
		if (const APlayerCharacter* PlayerInfo = GetPawn<APlayerCharacter>())
		{
			SaveGame->CurrentHealth =
				PlayerInfo->GetSurvivalComponent()->GetHealthMap().Key;
			SaveGame->MaxHealth =
				PlayerInfo->GetSurvivalComponent()->GetHealthMap().Value;
			
			SaveGame->CurrentHunger =
				PlayerInfo->GetSurvivalComponent()->GetHungerMap().Key;
			SaveGame->MaxHunger =
				PlayerInfo->GetSurvivalComponent()->GetHungerMap().Value;
			
			SaveGame->CurrentThirst =
				PlayerInfo->GetSurvivalComponent()->GetThirstMap().Key;
			SaveGame->MaxThirst =
				PlayerInfo->GetSurvivalComponent()->GetThirstMap().Value;
		}

		if (const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
		{
			SaveGame->PlayerInventoryList.Append(PS->GetPlayerInventoryList());
		}

		if (const ARaftGameState* GameState =
			GetWorld()->GetGameState<ARaftGameState>())
		{
			SaveGame->RaftBuildMetaData.Append(Raft->GetRaftBuildMetaData());

			for (TTuple<FVector, TArray<FPlacedObjectData>> RaftPlacedObjectData : Raft->GetRaftPlacedObjectData())
			{
				FPlacedObjectDataArray DataArray;
				DataArray.ObjectArray.Append(RaftPlacedObjectData.Value);
				SaveGame->RaftPlacedObjectMetaData.Add(RaftPlacedObjectData.Key, DataArray);
			}
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGame, GetWorld()->GetAuthGameMode<ARaftGameMode>()->GetMapName(), 0);
		// 이동 연산자 사용
		RecentSaveData = MoveTemp(SaveGame);
	}
}
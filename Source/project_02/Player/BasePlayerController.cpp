#include "BasePlayerController.h"

#include "BasePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SurvivalComponent.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"
#include "project_02/Widgets/HUD/PlayerRespawnUI.h"
#include "project_02/Widgets/Lobby/MainUI.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"
#include "project_02/Game/RaftSaveGame.h"
#include "project_02/HY/RaftGameState.h"

void ABasePlayerController::Initialize()
{
	// 초기화 시에는 마우스 커서 없애는 작업 수행
	SetShowMouseCursor(false);
	
	if (ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
	{
		PS->InitializeData();
	}

	APawn* PrevPawn = GetPawn();
	const FVector SpawnPoint(0,0, 100);
	const FRotator SpawnRotation(0,0,0);
	if (APlayerCharacter* PlayerPawn = GetWorld()->SpawnActor<APlayerCharacter>(PlayerClass, SpawnPoint, SpawnRotation))
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

void ABasePlayerController::SaveGame()
{
	if (const URaftSaveGame* RaftSaveGame = Cast<URaftSaveGame>(UGameplayStatics::LoadGameFromSlot("TestSlot", 0)))
	{
		RaftSaveGame->LastPlayerTransform;
		RaftSaveGame->RaftBuildMetaData;
		UE_LOG(LogTemp, Display, TEXT("데이터 로드에 성공하였습니다"))
	} else
	{
		UE_LOG(LogTemp, Display, TEXT("데이터 로드에 실패하였습니다"))
	}
	
	if (URaftSaveGame* SaveGame = Cast<URaftSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URaftSaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;

		// TODO: 테스트를 위한 하드코딩은 제거할 필요가 있음
		SaveGame->MapName = "TestMap";

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
			SaveGame->RaftBuildMetaData.Append(GameState->GetRaftBuildMetaData());

			for (TTuple<FVector, TArray<FPlacedObjectData>> RaftPlacedObjectData : GameState->GetRaftPlacedObjectData())
			{
				FPlacedObjectDataArray DataArray;
				DataArray.ObjectArray.Append(RaftPlacedObjectData.Value);
				SaveGame->RaftPlacedObjectMetaData.Add(RaftPlacedObjectData.Key, DataArray);
			}
		}
		
		UGameplayStatics::SaveGameToSlot(SaveGame, "TestSlot", 0);
	}
}

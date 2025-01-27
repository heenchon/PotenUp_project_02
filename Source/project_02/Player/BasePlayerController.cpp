#include "BasePlayerController.h"

#include "BasePlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
// TODO: 하나의 파일에 여러 구조체가 있는 경우에 생기는 문제?로 추측은 하는데
// 확실하지 않다. 불필요한 Import를 줄이기 위해 추후 해당 header 파일의
// 조정이 필요해보인다.
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SurvivalComponent.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"
#include "project_02/Widgets/HUD/PlayerRespawnUI.h"
#include "project_02/Widgets/Inventory/Module/InventorySlot.h"
#include "project_02/Game/RaftSaveGame.h"
#include "project_02/HY/RaftGameState.h"

void ABasePlayerController::BeginPlay()
{
	if (PlayUIClass)
	{
		ABasePlayerState* PS = GetPlayerState<ABasePlayerState>();
		PS->InitializeData();
		
		PlayUI = CreateWidget<UPlayerGameUI>(this, PlayUIClass);
		PlayUI->AddToViewport();
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
	if (URaftSaveGame* SaveGame = Cast<URaftSaveGame>(
		UGameplayStatics::CreateSaveGameObject(URaftSaveGame::StaticClass())))
	{
		FAsyncSaveGameToSlotDelegate SavedDelegate;

		// TODO: 테스트를 위한 하드코딩은 제거할 필요가 있음
		SaveGame->MapName = "TestMap";

		if (const APlayerCharacter* PlayerInfo = GetPawn<APlayerCharacter>())
		{
			SaveGame->HealthInfo =
				PlayerInfo->GetSurvivalComponent()->GetHealthMap();
			SaveGame->HungerInfo =
				PlayerInfo->GetSurvivalComponent()->GetHungerMap();
			SaveGame->ThirstInfo =
				PlayerInfo->GetSurvivalComponent()->GetThirstMap();

			FTransform TransformValue;
			TransformValue.SetLocation(PlayerInfo->GetActorLocation());
			TransformValue.SetRotation(PlayerInfo->GetActorRotation().Quaternion());
			TransformValue.SetScale3D({1,1,1,});
			
			SaveGame->LastPlayerTransform = TransformValue;
		}

		if (const ABasePlayerState* PS = GetPlayerState<ABasePlayerState>())
		{
			SaveGame->PlayerInventoryList =
				PS->GetPlayerInventoryList();
		}

		if (const ARaftGameState* GameState =
			GetWorld()->GetGameState<ARaftGameState>())
		{
			SaveGame->RaftBuildMetaData =
				GameState->GetRaftBuildMetaData();
			SaveGame->RaftPlacedObjectMetaData =
				GameState->GetRaftPlacedObjectData();
		}

		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, "TestSlot", 0);
	}
}

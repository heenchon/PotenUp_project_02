#include "RaftGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Player/BasePlayerController.h"

void ARaftGameMode::BeginPlay()
{
	// 메인 화면 UI Level 노출 처리
	UGameplayStatics::LoadStreamLevel(GetWorld(), "MainLevel",
	true, true, FLatentActionInfo());
}

void ARaftGameMode::StartPlayGame(const FString& NewMapName)
{
	MapName = NewMapName;
	
	IsLoading = true;
	
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

void ARaftGameMode::OnLevelLoadComplete()
{
	ABasePlayerController* MainPC = GetWorld()->GetFirstPlayerController<ABasePlayerController>();
	MainPC->Initialize();
	
	IsLoading = false;
}
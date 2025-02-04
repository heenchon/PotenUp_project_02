#include "WaterPurifier.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/InventoryComponent.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Player/BasePlayerState.h"
#include "project_02/HY/Items/Cup.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Widgets/HUD/ItemUI.h"
#include "project_02/Widgets/HUD/PlayerGameUI.h"


AWaterPurifier::AWaterPurifier()
{
	PrimaryActorTick.bCanEverTick = true;
	ProcessDuration = 5.0f;
	
	WaterMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WaterMesh"));
	WaterMesh->SetupAttachment(Root);
	WaterMesh->SetVisibility(false);

	Id = 14;
}

void AWaterPurifier::BeginPlay()
{
	Super::BeginPlay();
	BoilingSoundComponent = UGameplayStatics::SpawnSoundAttached(WaterBoilingSound, GetRootComponent());
	BoilingSoundComponent->Deactivate();
}

void AWaterPurifier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWaterPurifier::Interact(AUsable_Item* input, int curItemIndex)
{
	Super::Interact(input, curItemIndex);
	if (ACup* cup = Cast<ACup>(input))
	{
		if (bIsPurified)
		{
			cup->FillFreshWater();
			bIsPurified = false;
			WaterMesh->SetVisibility(false);

			//TODO: id 교체
			const UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
			const FItemInfoData ItemInfoData = GI->GetItemInfoList()[17];
			
			ABasePlayerController* PC = Cast<ABasePlayerController>(GetWorld()->GetFirstPlayerController());
			check(PC)
			PC->GetPlayerUI()->ItemMainUI->AddItemGetUI(1,ItemInfoData.GetDisplayName(),ItemInfoData.GetThumbnail());
			if (BoilingSoundComponent)
			{
				BoilingSoundComponent->Deactivate();
			}
			return;
		}
		//컵 속의 물을 없애고, 정수 시작
		if (cup->bIsSea)
		{
			UE_LOG(LogTemp, Display, TEXT("정수기에 바닷물 넣기"));
			if (BoilingSoundComponent)
			{
				BoilingSoundComponent->Activate();
				BoilingSoundComponent->Play();
			}
			WaterMesh->SetVisibility(true);
			WaterMesh->SetMaterial(0,Ocean);
			cup->EmptyCup();
			ProcessStart();
		}
	}
}

void AWaterPurifier::ProcessComplete()
{
	Super::ProcessComplete();
	UE_LOG(LogTemp, Display, TEXT("정수 완료."));
	WaterMesh->SetMaterial(0,Fresh);
	bIsPurified = true;
}

FString AWaterPurifier::GetDisplayText() const
{
	const APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player)
	{
		return Super::GetDisplayText();
	}
	
	const UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	const FItemMetaInfo ItemMetaData = PS->GetPlayerInventoryList()[Player->GetInventoryComponent()->GetSelectedHotSlotIndex()];
	const FItemInfoData ItemInfoData = GI->GetItemInfoList()[ItemMetaData.GetId()];
	
	if (!bIsPurified)
	{
		return FString::Printf(TEXT("Add Seawater"));
	}
	else
	{
		return FString::Printf(TEXT("Get Fresh Water"));
	}
}

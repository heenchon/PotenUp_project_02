#include "Grill.h"

#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/InventoryComponent.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/HY/Items/FishRaw.h"
#include "project_02/Player/BasePlayerState.h"


AGrill::AGrill()
{
	ProcessDuration = 5.0f;
	FoodPoint = CreateDefaultSubobject<USceneComponent>(TEXT("FoodPoint"));
	FoodPoint->SetupAttachment(StaticMesh);
	RawFoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FoodMesh"));
	RawFoodMesh->SetupAttachment(StaticMesh);
	RawFoodMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RawFoodMesh->SetVisibility(false);

	Id = 13;
}

void AGrill::BeginPlay()
{
	Super::BeginPlay();
	GrillingSoundComponent = UGameplayStatics::SpawnSoundAttached(GrillingSound, GetRootComponent());
	GrillingSoundComponent->Deactivate();
}

void AGrill::Interact(AUsable_Item* input, int curItemIndex)
{
	Super::Interact(input, curItemIndex);
	if (!bIsCooking)
	{
		// TODO: 하드코딩이니까 나중에 리팩토링 필요함.
		
		if (AFishRaw* fishRaw = Cast<AFishRaw>(input))
		{
			PS->DropItem(curItemIndex, 1);
			fishRaw->PutOnGrill();
			bIsCooking = true;
			RawFoodMesh->SetVisibility(true);
			if (GrillingSoundComponent)
			{
				GrillingSoundComponent->Activate();
				GrillingSoundComponent->Play();	
			}
			ProcessStart();
		}
	}
}

void AGrill::ProcessComplete()
{
	Super::ProcessComplete();
	AUsable_Item* fishCooked = GetWorld()->SpawnActor<AUsable_Item>(FishCookedTemp, FoodPoint->GetRelativeTransform());
	fishCooked->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	RawFoodMesh->SetVisibility(false);
	if (GrillingSoundComponent)
	{
		GrillingSoundComponent->Deactivate();
	}
	bIsCooking = false;
}

FString AGrill::GetDisplayText() const
{
	if (!bIsCooking)
	{
		return TEXT("Add Food");
	}
	
	const APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player)
	{
		return Super::GetDisplayText();
	}

	const UBaseGameInstance* GI = GetGameInstance<UBaseGameInstance>();
	const FItemMetaInfo ItemMetaData = PS->GetPlayerInventoryList()[Player->GetInventoryComponent()->GetSelectedHotSlotIndex()];
	const FItemInfoData ItemInfoData = GI->GetItemInfoList()[ItemMetaData.GetId()];

	return Super::GetDisplayText();
}
#include "Grill.h"

#include "project_02/Helper/ItemHelper.h"
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
}


// Called when the game starts or when spawned
void AGrill::BeginPlay()
{
	Super::BeginPlay();
}

void AGrill::Interact(AUsable_Item* input, int curItemIndex)
{
	Super::Interact(input, curItemIndex);
	if (!bIsCooking)
	{
		// if (FString* CookedTo = FItemHelper::GetItemInfoById(GetWorld(),
		// 	PS->GetPlayerInventoryList()[curItemIndex].GetId())
		// 	.GetOptionData().Find(EOptionDataKey::CookedTo))
		// {
		// 	// CookedTo
		// }
		// TODO: 하드코딩이니까 나중에 리팩토링 필요함.
		if (AFishRaw* fishRaw = Cast<AFishRaw>(input))
		{
			PS->DropItem(curItemIndex, 1);
			fishRaw->PutOnGrill();
			UE_LOG(LogTemp, Warning, TEXT("물고기 굽기 시작."));
			bIsCooking = true;
			RawFoodMesh->SetVisibility(true);
			ProcessStart();
		}
	}
}

void AGrill::ProcessComplete()
{
	Super::ProcessComplete();
	UE_LOG(LogTemp,Warning,TEXT("물고기 조리 완료."));
	AUsable_Item* fishCooked = GetWorld()->SpawnActor<AUsable_Item>(FishCookedTemp, FoodPoint->GetRelativeTransform());
	fishCooked->AttachToActor(this,FAttachmentTransformRules::KeepRelativeTransform);
	RawFoodMesh->SetVisibility(false);
	bIsCooking = false;
}





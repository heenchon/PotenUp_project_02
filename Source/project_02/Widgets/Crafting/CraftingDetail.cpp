// Fill out your copyright notice in the Description page of Project Settings.


#include "CraftingDetail.h"

#include "Components/Overlay.h"
#include "project_02/DataTable/CraftingData.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Helper/ItemHelper.h"

void UCraftingDetail::UpdateRequireList()
{
	const UBaseGameInstance* GI =
		static_cast<UBaseGameInstance*>(GetWorld()->GetGameInstance());

	if (FCraftingData* CraftData = GI->GetCraftingInfoMap().Find(CraftingId))
	{
		UE_LOG(LogTemp, Display, TEXT("테스트 길이: %d"), CraftData->Recipe.Array().Num());
		for (const TPair<uint32, uint32> NewRecipe : CraftData->Recipe.Array())
		{
			UE_LOG(LogTemp, Display, TEXT("테스트 :%d, %d"), NewRecipe.Key, NewRecipe.Value);
			FItemHelper::GetItemInfoById(GetWorld(), NewRecipe.Key);
		}
	} else
	{
		UE_LOG(LogTemp, Display, TEXT("테스트 실패"));
	}
}

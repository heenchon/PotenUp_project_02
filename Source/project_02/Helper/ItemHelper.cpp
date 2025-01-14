// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemHelper.h"

#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Game/BaseGameInstance.h"

FItemInfoData UItemHelper::GetItemInfoById(const UWorld* World, const uint16 Id)
{
	const UBaseGameInstance* GI = static_cast<UBaseGameInstance*>(World->GetGameInstance());
	return GI->GetItemInfoList()[Id];
}

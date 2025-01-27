// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "project_02/DataTable/BuildData.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "RaftSaveGame.generated.h"


UCLASS()
class PROJECT_02_API URaftSaveGame : public USaveGame
{
	GENERATED_BODY()

// 어차피 이 정보는 Temp 정보고 실제로 인스턴스 만들고 바로 넣는 용도기에
// 별도로 캡슐화를 위한 처리를 하지는 않는다.
// 캡슐화를 위한 함수 정보보다 SaveGame 인스턴스는 순수한 객체 정보만
// 담아둬서 불필요한 저장을 방지한다.
public:
	FString MapName;

	// Scale은 그냥 1,1,1 저장할 예정
	FTransform LastPlayerTransform;

	// 체력, 허기, 갈증 정보 저장
	TPair<uint8, uint8> HealthInfo;
	TPair<uint8, uint8> HungerInfo;
	TPair<uint8, uint8> ThirstInfo;

	// 배 건축 정보 저장하기
	TMap<FVector, FBuildData> RaftBuildMetaData;
	TMap<FVector, TArray<FPlacedObjectData>> RaftPlacedObjectMetaData;

	// 플레이어 인벤토리 정보
	TArray<FItemMetaInfo> PlayerInventoryList;
};

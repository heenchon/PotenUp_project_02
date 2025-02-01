#pragma once

#include "CoreMinimal.h"
#include "BuildData.generated.h"
/**
 * TMap 형태로 데이터 정보 저장
 * Key -> 좌표 정보 값 (ex. FVector가 된다면 x, y, z 좌표를, 아니라면 "(x)_(y)_(z)" 형식의 key로 저장
 * Value ->
 *		BuildInfo: 현재 설치된 구조물의 정보 (ex. 벽 재질, 벽 타입 등)
 * 
 *
 * 대략 저렇게 정보를 저장한다. 상황에 따라 Value를 구조체로 변경해 Type을 저장하는 것도 방법으로 보임
 *
 * 아이템 구조물 관련 정보
 * Map 형태로 저장
 * Key: 설치된 순서대로 반영되는 Id 값.
 * Value: BuildInfo 현재 설치된 구조물 정보 (ex. 구조물 타입, 설치된 블록 좌표 정보, 설치된 블록 좌표 기준으로 대략 어느정도 떨어져 있는지 정보)
*/

class ABuildingActor;

UENUM()
enum class EBlockPos
{
	Undefined,
	North,
	South,
	West,
	East,
	Max,
};

UENUM()
enum class EBlockType
{
	Undefined,
	Wood,
};

UENUM()
enum class EBlockCategory
{
	Undefined,
	Floor,
	Wall,
	Object
};

USTRUCT(BlueprintType)
struct PROJECT_02_API FBuildData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	EBlockType BlockType = EBlockType::Undefined;
	
	UPROPERTY()
	EBlockCategory BlockCategory = EBlockCategory::Undefined;

	UPROPERTY()
	bool IsMain = false;
};

USTRUCT(BlueprintType)
struct PROJECT_02_API FPlacedObjectData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY()
	FVector RelativeLoc = FVector::Zero();

	UPROPERTY()
	uint32 ObjectId = 0;
};

USTRUCT(BlueprintType)
struct PROJECT_02_API FBuildingInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	FORCEINLINE TSubclassOf<ABuildingActor> GetBuildClass() const { return BuildClass; }

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ABuildingActor> BuildClass;
};
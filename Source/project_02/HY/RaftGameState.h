// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "project_02/DataTable/BuildData.h"
#include "RaftGameState.generated.h"

class APlaceObjects;
class ABuildingActor;

UCLASS()
class PROJECT_02_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void UpdateBuildMetaData(const FVector& Pos, ABuildingActor* Build, const bool IsRemove = false);
	void UpdatePlacedObjectData(const FVector& Pos, const FPlacedObjectData& PlaceData, const bool IsRemove = false);

	FORCEINLINE TMap<FVector, FBuildData> GetRaftBuildMetaData() const { return RaftBuildMetaData; }
	FORCEINLINE TMap<FVector, ABuildingActor*> GetRaftBuildPointerData() const { return RaftBuildPointerData; }
	FORCEINLINE TMap<FVector, TArray<FPlacedObjectData>> GetRaftPlacedObjectData() const { return RaftPlacedObjectData; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	FVector WindDirection = FVector(1, 0, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	float WindStrength = 40.0f;
	
	UFUNCTION(BlueprintCallable, Category = "Wind")
	void SetWind(FVector newDir, float newStrength);


protected:
	virtual void BeginPlay() override;
	
private:
	// 벽과 바닥 등 순수 건축물에 대한 정보 메타 데이터 값
	// 벽의 경우는 바닥이 2, 3이라면 그 중간 2.5에 높이는 동일하게 가져간다.
	// 즉 벽은 바닥과 바닥 사이의 정보를 검증하는 역할
	TMap<FVector, FBuildData> RaftBuildMetaData;
	// 실제 게임 저장에는 영향 없는 별도의 데이터로 순수하게 건축물 pointer만 담는다.
	TMap<FVector, ABuildingActor*> RaftBuildPointerData;

	TMap<FVector, TArray<FPlacedObjectData>> RaftPlacedObjectData;
};

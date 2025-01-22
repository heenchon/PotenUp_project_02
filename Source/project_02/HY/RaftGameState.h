// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "project_02/DataTable/BuildData.h"
#include "RaftGameState.generated.h"

class ABuildingActor;

UCLASS()
class PROJECT_02_API ARaftGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	void UpdateBuildMetaData(const FVector& Pos, ABuildingActor* Build, const bool IsRemove = false);

	FORCEINLINE TMap<FVector, FBuildData> GetRaftBuildMetaData() const { return RaftBuildMetaData; }
	FORCEINLINE TMap<FVector, ABuildingActor*> GetRaftBuildPointerData() const { return RaftBuildPointerData; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	FVector WindDirection = FVector(1, 0, 0);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	float WindStrength = 40.0f;
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wind")
	// float SailStrength = 1.5f;
	
	UFUNCTION(BlueprintCallable, Category = "Wind")
	void SetWind(FVector newDir, float newStrength);


protected:
	virtual void BeginPlay() override;
	
private:
	// 벽과 바닥 등 순수 건축물에 대한 정보 메타 데이터 값
	TMap<FVector, FBuildData> RaftBuildMetaData;
	// 실제 게임 저장에는 영향 없는 별도의 데이터로 순수하게 건축물 pointer만 담는다.
	TMap<FVector, ABuildingActor*> RaftBuildPointerData;
};

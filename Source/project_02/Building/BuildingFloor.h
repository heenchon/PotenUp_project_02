#pragma once

#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "project_02/DataTable/BuildData.h"
#include "BuildingFloor.generated.h"

UCLASS()
class PROJECT_02_API ABuildingFloor : public ABuildingActor
{
	GENERATED_BODY()

public:
	ABuildingFloor();
	
	virtual void OnWireframeActive() override;
	virtual void OnWireframeInactive() override;
	virtual void UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild) override;

	TObjectPtr<UBoxComponent> GetFloorBoxByDirection(const EBlockPos Direction, const bool IsReverse = false);
	TObjectPtr<USceneComponent> GetWallPlaceVectorByDirection(const EBlockPos Direction, const bool IsReverse = false);
	TObjectPtr<USceneComponent> GetWallPlaceVectorByComponentBox(const UPrimitiveComponent* ComponentBox, const bool IsReverse = false);
	
	void UpdateWireframeBoxInfo();

protected:
	virtual void BeginPlay() override;

private:
	// TODO: 이 부분 배열과 enum을 고려해보기
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> RightBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> LeftBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> NorthBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> SouthBodyBox;
	
	// TODO: 이 부분 별도의 Actor Component 고려해보기
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> NorthWallBodyBox;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> NorthWallSceneVector;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> SouthWallBodyBox;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SouthWallSceneVector;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> EastWallBodyBox;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> EastWallSceneVector;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> WestWallBodyBox;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> WestWallSceneVector;

	UFUNCTION()
	void OnWallBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	float Durability = 3.0f;
};

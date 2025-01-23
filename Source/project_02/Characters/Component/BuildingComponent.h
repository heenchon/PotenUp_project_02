#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"

class UInputAction;
class ABuildingWall;
class ABuildingFloor;
class UBoxComponent;
class ABuildingActor;

UENUM()
enum class EBuildType
{
	Floor,
	Wall,
	Object
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API UBuildingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBuildingComponent();

	void TraceGroundToBuild(const FVector& TraceTo);

	// 빌딩을 위한 즉 격자를 위한 처리
	void CreateWireframeForGrid(const FHitResult& HitResult);

	void BuildWireframe();

	FORCEINLINE bool GetCanBuildMode() const { return CanBuild; } 
	FORCEINLINE void SetBuildMode(const bool NewMode) { CanBuild = NewMode; } 

	void ClearWireframe();
	void DeleteWireframe();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABuildingFloor> WireframeToFloorClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABuildingWall> WireframeToWallClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterial> WireframeMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true, ClampMin = 0))
	float TraceRange = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BuildChangeAction;

	UFUNCTION()
	void ChangeNextBuildAction();

	UPROPERTY()
	TObjectPtr<ABuildingActor> CurrentWireframeActor;
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CurrentWireframeBox;
	
	UPROPERTY()
	TObjectPtr<AActor> CurrentHitActor;
	
	EBuildType FrameType = EBuildType::Floor;

	ETraceTypeQuery GetCheckTraceChannel() const;
	
	bool CanBuild = false;

	void SpawnFrameFloor(const FHitResult& HitResult);

	void SpawnFrameWall(const FHitResult& HitResult);

	void ReattachFloor(const FHitResult& HitResult);
	
	void ReattachWall(const FHitResult& HitResult);
};
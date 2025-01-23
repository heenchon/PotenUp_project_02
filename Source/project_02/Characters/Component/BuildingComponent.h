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

	void BuildWireframe();

	void SetBuildType(const EBuildType NewType);
	FORCEINLINE bool GetCanBuildMode() const { return CanBuild; } 
	FORCEINLINE void SetBuildMode(const bool NewMode) { CanBuild = NewMode; } 
	FORCEINLINE void SetCustomBuildBlueprint(const TSubclassOf<AActor>& NewBlueprint) { CustomBuildItemClass = NewBlueprint; } 

	void ClearWireframe();
	void DeleteWireframe();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TSubclassOf<AActor> CustomBuildItemClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Building", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABuildingFloor> WireframeToFloorClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Building", meta = (AllowPrivateAccess = true))
	TSubclassOf<ABuildingWall> WireframeToWallClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Wireframe", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterial> WireframeMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options|Wireframe", meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterial> WireframeBlockMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true, ClampMin = 0))
	float TraceRange = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> BuildChangeAction;

	UPROPERTY(EditAnywhere, Category = "Options|Input"
		, meta = (AllowPrivateAccess = true))
	TObjectPtr<UInputAction> InteractiveInputAction;
	
	UFUNCTION()
	void ChangeNextBuildAction();

	UFUNCTION()
	void BuildCustomObject();

	UPROPERTY()
	TObjectPtr<AActor> CurrentWireframeActor;
	
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

	// 빌딩을 위한 즉 격자를 위한 처리
	void CreateWireframeForGrid(const FHitResult& HitResult);

	// 오브젝트 전용 처리
	void CreateWireframeForObject(const FHitResult& HitResult);
};
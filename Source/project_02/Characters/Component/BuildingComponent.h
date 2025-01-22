#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingComponent.generated.h"

class UBoxComponent;
class ABuildingActor;

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

protected:
	virtual void BeginPlay() override;

private:
	// TODO: 해당 정보는 나중에 동적으로 플레이어의 상태에 따라 변경될 예정
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<ABuildingActor> WireframeToBuildClass;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UMaterial> WireframeMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true, ClampMin = 0))
	float TraceRange = 0.f;

	UPROPERTY()
	TObjectPtr<ABuildingActor> CurrentWireframeActor;
	
	UPROPERTY()
	TObjectPtr<UPrimitiveComponent> CurrentWireframeBox;
	
	bool CanBuild = false;
};
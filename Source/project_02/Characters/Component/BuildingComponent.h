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

	void BuildWireframe();

	FORCEINLINE bool GetCanBuildMode() const { return CanBuild; } 

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
	
	// TODO: 해당 값은 추후 플레이어가 망치를 든다던가의 경우에 true로 변환됨
	bool CanBuild = true;

	void ClearWireframe();
};
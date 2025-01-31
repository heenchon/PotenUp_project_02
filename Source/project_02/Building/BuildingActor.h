#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "project_02/DataTable/BuildData.h"
#include "BuildingActor.generated.h"

enum class EBlockType;
enum class EBlockPos;
class UBoxComponent;
class UMaterialInterface;

UCLASS()
class PROJECT_02_API ABuildingActor : public AActor
{
	GENERATED_BODY()

public:
	ABuildingActor();
	
	void SetWireframeMaterial(UMaterial* NewMaterial);
	
	FORCEINLINE bool GetIsWireframe() const { return IsWireframe; }
	void SetWireframe(const bool NewIsWireframe);

	virtual void OnWireframeActive();
	virtual void OnWireframeInactive();
	
	virtual void SetCenter();
	
	virtual void UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild);
	virtual void AddDurability(const int8 AddValue);
	
	FORCEINLINE FVector GetBuildPos() const { return BuildPos; }
	FORCEINLINE void SetBuildPos(const FVector& NewValue) { BuildPos = NewValue; }
	
	FORCEINLINE bool GetIsMain() const { return IsMain; }
	FORCEINLINE void SetMainBuild(const TObjectPtr<ABuildingActor>& NewValue) { MainBuild = NewValue; }
	
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetBodyMesh() const { return BodyMesh; }
	FORCEINLINE TObjectPtr<ABuildingActor> GetMainBuild() const { return MainBuild; }
	FORCEINLINE uint32 GetConsumeCount() const { return ConsumeCount; }
	
	FORCEINLINE EBlockType GetBlockType() const { return BlockType; }
	FORCEINLINE EBlockCategory GetBlockCategory() const { return BlockCategory; }
	FORCEINLINE uint8 GetCurrentDurability() const { return CurrentDurability; }
	
	void SetDefaultMaterial();

	void AttachWireframeToComponent(
		ABuildingActor* TargetBlock,
		UPrimitiveComponent* TargetComp
	);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Options")
	EBlockType BlockType;
	
	UPROPERTY(EditDefaultsOnly, Category="Options")
	EBlockCategory BlockCategory;
	
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	uint8 MaxDurability = 3;
	
	uint8 CurrentDurability = 0;
	
	UPROPERTY()
	TObjectPtr<ABuildingActor> MainBuild;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsMain = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsWireframe = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	uint32 ConsumeCount;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginMaterials;

	FVector BuildPos;
};
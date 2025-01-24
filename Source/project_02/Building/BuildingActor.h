#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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
	void SetWireframeMaterialForOption(const bool Option, UMaterial* SuccessMaterial, UMaterial* FailMaterial);
	
	void SetWireframe(const bool NewIsWireframe);

	virtual void OnWireframeActive();
	virtual void OnWireframeInactive();
	virtual void SetCenter();
	virtual void UpdateBuildData(const UPrimitiveComponent* TargetComp, ABuildingActor* ChildBuild);
	
	FORCEINLINE void SetMainBuild(const TObjectPtr<ABuildingActor>& NewValue) { MainBuild = NewValue; }
	FORCEINLINE bool GetIsWireframe() const { return IsWireframe; }
	FORCEINLINE FVector GetBuildPos() const { return BuildPos; }
	FORCEINLINE void SetBuildPos(const FVector& NewValue) { BuildPos = NewValue; }
	FORCEINLINE bool GetIsMain() const { return IsMain; }
	FORCEINLINE TObjectPtr<ABuildingActor> GetMainBuild() const { return MainBuild; }
	FORCEINLINE EBlockType GetBlockType() const { return BlockType; }
	FORCEINLINE uint32 GetConsumeCount() const { return ConsumeCount; }
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetBodyMesh() const { return BodyMesh; }
	
	void SetDefaultMaterial();

	void AttachWireframeToComponent(
		ABuildingActor* TargetBlock,
		UPrimitiveComponent* TargetComp
	);

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY()
	TObjectPtr<ABuildingActor> MainBuild;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsMain = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsWireframe = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	EBlockType BlockType;
	
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

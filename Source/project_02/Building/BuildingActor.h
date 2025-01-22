#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingActor.generated.h"

class UBoxComponent;
class UMaterialInterface;

UCLASS()
class PROJECT_02_API ABuildingActor : public AActor
{
	GENERATED_BODY()

public:
	ABuildingActor();
	
	void SetWireframeMaterial(UMaterial* NewMaterial);
	
	void SetWireframe(const bool NewIsWireframe);

	virtual void OnWireframeActive();
	virtual void OnWireframeInactive();

	FORCEINLINE void SetDefaultMaterial();
	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetBodyMesh() const { return BodyMesh; }

	void AttachWireframeToComponent(
		ABuildingActor* TargetBlock,
		UPrimitiveComponent* TargetComp
	);

protected:
	virtual void BeginPlay() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsWireframe = true;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> RootComp;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OriginMaterials;
};

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

	void OnWireframeActive();
	void OnWireframeDeactive();

	FORCEINLINE void SetDefaultMaterial();

	void AttachWireframeToComponent(
		ABuildingActor* TargetBlock,
		UPrimitiveComponent* TargetComp
	);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	bool IsWireframe = true;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> BodyMesh;

	UPROPERTY()
	 TArray<TObjectPtr<UMaterialInterface>> OriginMaterials;

	// TODO: 이 부분 배열과 enum을 고려해보기
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> RightBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> LeftBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> NorthBodyBox;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> SouthBodyBox;
};

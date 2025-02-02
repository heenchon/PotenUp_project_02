#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// 직접 사용하기 위해서는 일단은 전방선언 불가함
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Tool/InteractiveInterface.h"
#include "Usable_Item.generated.h"

UCLASS()
class PROJECT_02_API AUsable_Item : public AActor, public IInteractiveInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AUsable_Item();
	UPROPERTY(EditAnywhere)
	USceneComponent* Root;
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;
	
	FORCEINLINE FItemMetaInfo GetItemMetaInfo() const { return ItemMetaInfo; }
	void UpdateItemInfo(const uint32 RemainCount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Use();
	virtual void ApplyTo();
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint32 ItemId;

	FItemMetaInfo ItemMetaInfo;
};

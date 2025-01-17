#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
// 직접 사용하기 위해서는 일단은 전방선언 불가함
#include "project_02/DataTable/ItemInfoData.h"
#include "Trash.generated.h"

UCLASS()
class PROJECT_02_API ATrash : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrash();

	FORCEINLINE FItemMetaInfo GetItemMetaInfo() const { return ItemMetaInfo; }
	void UpdateItemInfo(const uint32 RemainCount);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly)
	class UBuoyancyComponent* Buoyancy;

	//TODO: 윈드 매니저 추가 후 cpp 수정
	UPROPERTY(EditAnywhere)
	class ARaft* Raft;
	
	FVector WindDirection;
	float WindStrength = 100.0f;

	bool IsStop = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint32 ItemId;

	FItemMetaInfo ItemMetaInfo;
};

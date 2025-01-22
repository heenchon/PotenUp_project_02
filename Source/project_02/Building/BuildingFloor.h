#pragma once

#include "CoreMinimal.h"
#include "BuildingActor.h"
#include "BuildingFloor.generated.h"

UCLASS()
class PROJECT_02_API ABuildingFloor : public ABuildingActor
{
	GENERATED_BODY()

public:
	ABuildingFloor();
	
	virtual void OnWireframeActive() override;
	virtual void OnWireframeInactive() override;

protected:
	virtual void BeginPlay() override;

private:
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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractiveItem.generated.h"

UENUM()
enum class EInteractiveStatus : uint8
{
	Idle,
	Interacting,
	Completed,
};

UCLASS()
class PROJECT_02_API AInteractiveItem : public AActor
{
	GENERATED_BODY()

public:
	AInteractiveItem();

	virtual void StartInteractive();
	
	virtual void OnInteractiveHold();
	
	virtual bool EndInteractive();

	FORCEINLINE void ChangeStatus(const EInteractiveStatus NewStatus) { Status = NewStatus; }

protected:
	virtual void Tick(float DeltaSeconds) override;
	
	// 홀딩 활성화 여부 설정
	UPROPERTY(EditDefaultsOnly, Category = "Options|Interactive", meta = (AllowPrivateAccess = true))
	bool IsHold = false;

private:
	EInteractiveStatus Status = EInteractiveStatus::Idle;
	
	// 홀딩 주기 시간 설정, -1이면 별도의 홀딩 시간 없이 틱 마다 실행
	UPROPERTY(EditDefaultsOnly, Category = "Options|Interactive",
		meta = (AllowPrivateAccess = true, ClampMin = -1))
	float HoldDuration = -1;

	float CurrentHoldTime = 0;
};

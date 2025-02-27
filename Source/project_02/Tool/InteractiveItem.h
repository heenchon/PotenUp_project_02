﻿#pragma once

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

	// 특별한 의미는 있지 않지만 부착될 때 별도로 호출하는 함수
	virtual void OnAttached();
	
	virtual void StartInteractive();
	
	virtual void OnInteractiveHold();
	virtual void OnInteractiveHold(float DeltaTime);
	
	virtual bool EndInteractive();

	FORCEINLINE void ChangeStatus(const EInteractiveStatus NewStatus) { Status = NewStatus; }

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	EInteractiveStatus Status = EInteractiveStatus::Idle;
	
	// 홀딩 활성화 여부 설정
	UPROPERTY(EditDefaultsOnly, Category = "Options|Interactive", meta = (AllowPrivateAccess = true))
	bool IsHold = false;
	
	// 홀딩 주기 시간 설정, -1이면 별도의 홀딩 시간 없이 틱 마다 실행
	UPROPERTY(EditDefaultsOnly, Category = "Options|Interactive",
		meta = (AllowPrivateAccess = true, ClampMin = -1))
	float HoldDuration = -1;

	float CurrentHoldTime = 0;
};

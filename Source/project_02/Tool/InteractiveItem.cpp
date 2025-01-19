#include "InteractiveItem.h"


AInteractiveItem::AInteractiveItem()
{
}

void AInteractiveItem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 홀딩 상태가 활성화되어 있어야만 하고, 현재 홀딩 시간이 홀딩 관련
	// 설정 시간보다 커진 경우 홀딩 관련 이벤트 함수 실행
	if (IsHold &&
		Status == EInteractiveStatus::Interacting &&
		(CurrentHoldTime > HoldDuration || HoldDuration == -1))
	{
		OnInteractiveHold();
	}

	CurrentHoldTime += DeltaSeconds;
}


void AInteractiveItem::StartInteractive()
{
	Status = EInteractiveStatus::Interacting;
}

void AInteractiveItem::OnInteractiveHold()
{
}

void AInteractiveItem::EndInteractive()
{
	// 이미 종료된 시점에서는 굳이 무언가를 처리할 필요가 없다.
	if (Status == EInteractiveStatus::Completed)
	{
		Status = EInteractiveStatus::Idle;
		return;
	}
	
	Status = EInteractiveStatus::Idle;
}
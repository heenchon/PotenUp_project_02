// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "SwimmingComponent.generated.h"

class UTimelineComponent;
class APlayerCharacter;
// 캐릭터와 Movement에 의존성이 있는 컴포넌트
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API USwimmingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USwimmingComponent();
	
	FORCEINLINE bool GetIsSwimMode() const { return IsSwimMode; }
	FORCEINLINE int32 GetWaterLevel() const { return WaterLevel; }
	FORCEINLINE float GetFloatingValueInWaterPercent() const { return FloatingValueInWater / 100; }
	void PlayDiving() const;

	bool IsOwnerNearWaterLevel(const FVector& MoveToVector) const;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSwimMode_BP() const { return IsSwimMode; }

protected:
	virtual void BeginPlay() override;

private:
	// 물 속에 들어갈 때 그 물의 높이
	// 보통 물 속은 한번 들어가면 평평한 그 높이가 유지되는 방식이기 때문에
	// 한번만 저장함. (만약 쫄리면 올라올 때 쯤 계속 line-trace해서 구해도 됨)
	int32 WaterLevel;

	bool IsDiving;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Owner;

	UPROPERTY()
	TObjectPtr<UTimelineComponent> DivingTimeline;
	
	UPROPERTY(EditDefaultsOnly, Category="Options", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCurveFloat> DivingTimingCurve;
	
	FOnTimelineFloat DivingCallback;
	
	FOnTimelineEvent DivingFinish;
	
	/* 현재 수영 상태인지 여부 */
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 IsSwimMode:1 = false;
 
	/* 물 위에서 내 Capsule 높이의 어느정도 만큼 떠 있을 시 설정하는 Percent 값 */
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true, ClampMin = 0, ClampMax = 100))
	float FloatingValueInWater = 50;
	
	/* 물 위에서 점프시 앞으로 가는 가속도 값 */
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float DivingMoveForwardPower = 100;
	
	/* 물 위에서 점프하는 힘의 값 */
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	float DivingJumpPower = 800;

	UFUNCTION()
	void OnCheckOverlapInWater(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void OnCheckOverlapOutWater(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	UFUNCTION()
	void OnDivingPlayCallback(float Output);
	
	UFUNCTION()
	void OnDivingFinish();
};

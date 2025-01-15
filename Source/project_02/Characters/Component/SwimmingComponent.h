// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SwimmingComponent.generated.h"

class APlayerCharacter;
// 캐릭터와 Movement에 의존성이 있는 컴포넌트
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT_02_API USwimmingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USwimmingComponent();
	
	FORCEINLINE bool GetIsSwimMode() const { return IsSwimMode; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsSwimMode_BP() const { return IsSwimMode; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> Owner;
	
	/* 현재 수영 상태인지 여부 */
	UPROPERTY(EditDefaultsOnly, Category = "Options", meta = (AllowPrivateAccess = true))
	uint8 IsSwimMode:1 = false;

	UFUNCTION()
	void OnCheckOverlapInWater(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult & SweepResult);

	UFUNCTION()
	void OnCheckOverlapOutWater(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
};

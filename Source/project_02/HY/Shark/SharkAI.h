// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ESharkState.h"
#include "SharkAI.generated.h"

class APlayerCharacter;

UCLASS()
class PROJECT_02_API ASharkAI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASharkAI();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AController> AIController;
	
	class APlayerCharacter* Player;
	AActor* Raft;
	
	//타겟 상태
	UPROPERTY(EditAnywhere)
	AActor* Target;
	UPROPERTY(EditAnywhere)
	bool PlayerIsWater;
	
	UPROPERTY()
	ESharkState CurrentState;
	float CurTime;

	//상어 속성
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed = 700.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed = 1200.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackDuration = 5.0f;

	//Idle 상태 움직임
	FVector IdleLocation;
	FVector ControlPoint;

	//타겟 도달 감지 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DetectionDistance = 500.0f;

	//도망 거리 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDist = 4000.0f;
	float MinDist = 2000.0f;
	FVector RunLocation;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//상태 전환
	void ChangeState(ESharkState newState);
	//상태별 함수
	void Idle(float DeltaTime);
	void MoveToTarget(float DeltaTime);
	void AttackPlayer(float DeltaTime);
	void AttackRaft(float DeltaTime);
	void Runaway(float DeltaTime);
	
	FVector NewRunawayLocation(FVector originLoc, float maxDist, float minDist);
	void NewRandomCurveLocation();
	

//상민띠가 준 Enum to String 기능...
public:
	template <typename EnumType>
	static FString GetClassEnumKeyAsString(const EnumType EnumeratorValue)
	{
		TArray<FString> EnumKey;
		UEnum::GetValueAsString(EnumeratorValue).
			ParseIntoArray(EnumKey, TEXT("::"));

		if (EnumKey.Num() == 1)
		{
			return EnumKey[0];
		}
		return EnumKey[1];
	}
};





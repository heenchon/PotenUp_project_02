// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ESharkState.h"
#include "SharkAI.generated.h"

UCLASS()
class PROJECT_02_API ASharkAI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASharkAI();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	
	AActor* Player;
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
	float SharkBasicSpeed = 300.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed = 500.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackDuration = 5.0f;

	//타겟 도달 거리 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Distance = 200.0f;

	//타겟 도망 거리 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDist = 6000.0f;
	float MinDist = 4500.0f;
	FVector RunLocation = FVector::ZeroVector;
	
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

	FVector RandomLocation(FVector originLoc, float maxDist, float minDist);
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





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
	// Sets default values for this actor's propertiesgit
	ASharkAI();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AController> AIController;

	UPROPERTY()
	APawn* Player;
	UPROPERTY()
	AActor* Raft;
	
	//타겟 상태
	UPROPERTY(EditAnywhere)
	AActor* Target;
	UPROPERTY(EditAnywhere)
	bool PlayerIsWater;
	
	UPROPERTY()
	ESharkState CurrentState;
	ESharkState NextState;
	float CurTimeforAttack;
	float CurTimeforIdle;
	float CurTimeforTurn;


	//상어 속성
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed = 1000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed = 1500.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackDuration = 10.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float IdleMoveDuration = 5.0f;

	//Idle 상태 움직임
	FVector StartLocation;
	FVector TargetLocation;

	//타겟 도달 감지 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DetectionDistance = 500.0f;

	//도망 거리 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDist = 4000.0f;
	float MinDist = 2000.0f;
	
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
	void Turning(float DeltaTime);
	
	FVector NewRunawayLocation(FVector originLoc, float maxDist, float minDist);
	FVector NewIdleLocation();
	
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





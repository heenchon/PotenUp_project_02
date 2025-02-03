// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ESharkState.h"
#include "SharkAI.generated.h"

class UCapsuleComponent;
class USceneComponent;
class ABuildingFloor;
class APlayerCharacter;
class USwimmingComponent;

UCLASS()
class PROJECT_02_API ASharkAI : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's propertiesgit
	ASharkAI();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;
	UPROPERTY(EditAnywhere)
	UCapsuleComponent* Capsule;
	UPROPERTY(EditAnywhere)
	USceneComponent* MouthPosition;

	//상어 속성
	/**기본 이동 속도*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed = 1000.0f;
	/**도망가거나, 공격하러 갈 때 이동 속도*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed = 1500.0f;
	/**바닥을 부수러 가는 주기*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackDuration = 15.0f;
	/**플레이어 주변을 배회할 때, 랜덤 위치로 이동하는 주기*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float IdleMoveDuration = 7.0f;
	/**바닥을 물고 있을 때, 판자 내구도가 줄어드는 시간*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FloorDestroyDuration = 3.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HealthPoint = 30.0f;

	/** 목표 위치에 도달 여부를 감지할 기준*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DetectionDistance = 250.0f;

	/** 도망갈 때 랜덤 위치 범위*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDist = 6000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinDist = 4000.0f;


private:
	UPROPERTY()
	APawn* Player;
	UPROPERTY()
	USwimmingComponent* SwimComponent;
	UPROPERTY()
	ABuildingFloor* Floor;
	
	ESharkState CurrentState;
	ESharkState NextState;
	
	float CurTimeforAttack;
	float CurTimeforIdle;
	int CurHitCount = 0;
	
	//Idle 상태 Lerp 움직임용 위치값
	FVector StartLocation;
	FVector TargetLocation;

	FRotator BiteRotation = FRotator(0, 0, 0);
	FVector BiteOffset = FVector(0, 0, 0);
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USoundWave> HitSound;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	//상태 전환
	void ChangeState(ESharkState newState);
	//상태별 함수
	void Idle(float DeltaTime);
	void MoveToRaft(float DeltaTime);
	void AttackRaft(float DeltaTime);
	void MoveToPlayer(float DeltaTime);
	void AttackPlayer(float DeltaTime);
	void Runaway(float DeltaTime);
	void Turning(float DeltaTime);

private:
	FVector NewRunawayLocation(FVector originLoc, float maxDist, float minDist);
	FVector NewIdleLocation();
	ABuildingFloor* GetFloor();
	FTimerHandle SharkTimerHandle;
	bool IsAttackableFloor(const TArray<FVector>& positionArr ,const FVector& floorPos);
	void DamageFloor();
	void SetBiteRotation(const FVector& dir);
	void Died();
	void Respawn();
};





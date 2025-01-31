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
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkBasicSpeed = 1000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackSpeed = 1500.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float SharkAttackDuration = 15.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float IdleMoveDuration = 7.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FloorDestroyDuration = 5.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HealthPoint = 20.0f;

	//타겟 도달 감지 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DetectionDistance = 400.0f;

	//도망 거리 범위
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxDist = 6000.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MinDist = 3000.0f;


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





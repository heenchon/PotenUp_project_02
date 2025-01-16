// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PaddleTest.generated.h"

UCLASS()
class PROJECT_02_API APaddleTest : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APaddleTest();

	//TODO: 윈드 매니저 추가 후 cpp 수정
	//Raft의 바람 방향을 가져오기 위해
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class ARaftGameState* RaftGameState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class ARaft* Raft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	class APawn* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	bool bIsPaddling = false;
	/** 노를 젓는 힘이 강해져요. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float PaddleForce = 4.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	float PaddleDuration = 1.5f;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	FVector WindOriginDir;
	float WindOriginStrength;
	FTimerHandle PaddleTimerHandle;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void PaddlingStart();

	UFUNCTION()
	void PaddlingEnd();

	UFUNCTION(BlueprintCallable)
	bool GetPlayerRaft();
};

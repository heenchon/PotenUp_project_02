// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
	
}

// Called every frame
void ASharkAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	switch (CurrentState)
	{
	case ESharkState::Idle:
		Idle(DeltaTime);
		
	case ESharkState::MoveToTarget:
		MoveToTarget(DeltaTime, TargetLocation);
		
	case ESharkState::AttackPlayer:
		AttackPlayer(DeltaTime);

	case ESharkState::RunAway:
		Runaway(DeltaTime);

	}
}

void ASharkAI::ChangeState(ESharkState newState)
{
	CurrentState = newState;
}

void ASharkAI::Idle(float DeltaTime)
{
	//돌아다니면서 플레이어가 물에 있는지 체크
	if (PlayerIsWater)
	{
		//물에 있다면 타겟을 플레이어로 전환하고 MoveToTarget
		TargetLocation = Player->GetActorLocation();
		ChangeState(ESharkState::MoveToTarget);
	}
	else
	{
		//아니라면 배 공격 쿨타임을 카운트
		//배 공격 쿨타임이 찼다면 타겟을 배로 전환하고 MoveToTarget
	}
}

void ASharkAI::MoveToTarget(float DeltaTime, FVector targetLoc)
{
	//타겟으로 이동
	SetActorLocation((targetLoc-GetActorLocation().Normalize())*SharkAttackSpeed*DeltaTime+GetActorLocation());
	//목표지점에 도달했고, 타겟이 배면, AttackRaft
	//목표지점에 도달했고, 타겟이 플레이어면, AttackPlayer
	if (targetLoc == GetActorLocation())
	{
		if (PlayerIsWater) ChangeState(ESharkState::AttackPlayer);
		else ChangeState(ESharkState::AttackRaft);
	}
}

void ASharkAI::AttackPlayer(float DeltaTime)
{
	UE_LOG(LogTemp,Warning, TEXT("플레이어 공격"))
	ChangeState(ESharkState::RunAway);
}

void ASharkAI::AttackRaft(float DeltaTime)
{
	//타이머로 배 파괴 함수 실행.
	//배 파괴 함수가 3번 실행되면, Runaway
	//만약 플레이어에게 공격을 3번 당하면, Runaway 
}

void ASharkAI::Runaway(float DeltaTime)
{
	//일정 거리 이상으로 빠르게 이동
	//Idle
}


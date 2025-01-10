// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/HY/Raft/Raft.h"

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
	
	//TODO: GetActorOfClass(Raft)
	// Raft = UGameplayStatics::GetActorOfClass(GetWorld(),TSubclassOf<ARaft>());
	// if (Raft) UE_LOG(LogTemp,Warning,TEXT("%s"),*Raft->GetName());
	
	CurrentState = ESharkState::Idle;
	Target = Player;

	
}

// Called every frame
void ASharkAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Display, TEXT("%s"),GetClassEnumKeyAsString(ESharkState::Idle));
	
	switch (CurrentState)
	{
	case ESharkState::Idle:
		Idle(DeltaTime);
		break;
	case ESharkState::MoveToTarget:
		MoveToTarget(DeltaTime);
		break;
	case ESharkState::AttackPlayer:
		AttackPlayer(DeltaTime);
		break;
	case ESharkState::RunAway:
		Runaway(DeltaTime);
		break;
	default:
		break;
	}
}

void ASharkAI::ChangeState(ESharkState newState)
{
	CurrentState = newState;
}

void ASharkAI::Idle(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning,TEXT("상어 Idle"));
	//돌아다니면서 플레이어가 물에 있는지 체크
	if (PlayerIsWater)
	{
		//물에 있다면 타겟을 플레이어로 전환하고 MoveToTarget
		Target = Player;
		ChangeState(ESharkState::MoveToTarget);
	}
	else
	{
		//아니라면 배 공격 쿨타임을 카운트
		CurTime += DeltaTime;
		//배 공격 쿨타임이 찼다면 타겟을 배로 전환하고 MoveToTarget
		if (CurTime > SharkAttackDuration)
		{
			Target = Raft;
			CurTime = 0.0f;
			ChangeState(ESharkState::MoveToTarget);
		}
	}
}

void ASharkAI::MoveToTarget(float DeltaTime)
{
	//TODO: 임시로 플레이어로 고정
	Target = Player;
	// UE_LOG(LogTemp,Warning,TEXT("%s"), *(Target->GetActorLocation().ToString()));
	
	FVector dir = Target->GetActorLocation()-GetActorLocation();
	dir.Normalize();
	SetActorLocation(dir*DeltaTime*SharkAttackSpeed + this->GetActorLocation());
	if (FVector::Dist(GetActorLocation(),Target->GetActorLocation()) < Distance)
	{
		UE_LOG(LogTemp,Warning,TEXT("타겟에 도착!"));
		if (Target == Player) ChangeState(ESharkState::AttackPlayer);
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
	SetActorLocation(RunLocation = RandomLocation(GetActorLocation(),MaxDist,MinDist));
	ChangeState(ESharkState::Idle);
}

FVector ASharkAI::RandomLocation(FVector originLoc, float maxDist, float minDist)
{
	float Angle = FMath::RandRange(0.0f, 360.0f);
	FVector Direction = FVector(FMath::Cos(FMath::DegreesToRadians(Angle)), FMath::Sin(FMath::DegreesToRadians(Angle)), 0.0f);
	float randDist = FMath::RandRange(minDist, maxDist);
	
	FVector RandomLocation = originLoc + Direction * randDist;
	
	RandomLocation.Z = FMath::Min(originLoc.Z, RandomLocation.Z);

	return RandomLocation;
}


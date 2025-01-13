// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"
#include "SharkAIController.h"
#include "../../Characters/PlayerCharacter.h"
#include "Channels/MovieSceneInterpolation.h"

// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	AIController = ASharkAIController::StaticClass();
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	
	//TODO: GetActorOfClass(Raft)
	// Raft = UGameplayStatics::GetActorOfClass(GetWorld(),TSubclassOf<ARaft>());
	// if (Raft) UE_LOG(LogTemp,Warning,TEXT("%s"),*Raft->GetName());

	NewRandomCurveLocation();
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
	
	FVector NewLocation = FMath::CubicInterp(GetActorLocation(),ControlPoint,IdleLocation,FVector::ZeroVector,DeltaTime);
	SetActorLocation(NewLocation);
	
	// UE_LOG(LogTemp,Warning,TEXT("%s"), *NewLocation.ToString());
	
	if (FVector::Dist(GetActorLocation(),IdleLocation)< 10.0)
	{
		NewRandomCurveLocation();
	}
	
	if (PlayerIsWater)
	{
		Target = Player;
		ChangeState(ESharkState::MoveToTarget);
	}
	else
	{
		CurTime += DeltaTime;
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
	if (FVector::Dist(GetActorLocation(),Target->GetActorLocation()) < DetectionDistance)
	{
		// UE_LOG(LogTemp,Warning,TEXT("타겟에 도착!"));
		if (Target == Player) ChangeState(ESharkState::AttackPlayer);
		else ChangeState(ESharkState::AttackRaft);
	}
}

void ASharkAI::AttackPlayer(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning, TEXT("플레이어 공격"))
	UGameplayStatics::ApplyDamage(Player, 20.f,
		nullptr, this, UDamageType::StaticClass());
	RunLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
	// UE_LOG(LogTemp,Warning, TEXT("도망갈 좌표: %s"),*RunLocation.ToString());
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
	FVector dir = RunLocation-GetActorLocation();
	dir.Normalize();
	SetActorLocation(dir*SharkAttackSpeed*DeltaTime + GetActorLocation());
	
	if (FVector::Dist(GetActorLocation(),RunLocation) < 10.0)
	{
		// UE_LOG(LogTemp,Warning,TEXT("도망완료"));
		NewRandomCurveLocation();
		ChangeState(ESharkState::Idle);
	}
}

FVector ASharkAI::NewRunawayLocation(FVector originLoc, float maxDist, float minDist)
{
	float angle = FMath::RandRange(0.0f, 360.0f);
	FVector direction = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), -0.5f);
	
	float randDist = FMath::RandRange(minDist, maxDist);
	FVector RandomLocation = originLoc +direction * randDist;
	RandomLocation.Z = FMath::Min(originLoc.Z, -1800.f); //바닥 뚫지 않게 제한
	
	return RandomLocation;
	
}

void ASharkAI::NewRandomCurveLocation()
{
	FVector playerLoc = Player->GetActorLocation();
	IdleLocation = playerLoc +FMath::VRand() * 100.0f;
	IdleLocation.Z = FMath::RandRange(-1000.0f, -100.0f); //바다 속으로 제한
	ControlPoint = (GetActorLocation() + IdleLocation) / 2.0f + FVector(0, 0, 200.0f);
	UE_LOG(LogTemp,Warning,TEXT("랜덤 IDLE 위치: %s, 제어점 %s"), *IdleLocation.ToString(), *ControlPoint.ToString());
}


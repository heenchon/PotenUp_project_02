// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"
#include "SharkAIController.h"
#include "project_02/Helper/EnumHelper.h"
#include "project_02/Characters/Component/SwimmingComponent.h"

// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AIController = ASharkAIController::StaticClass();
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SwimComponent = Player->FindComponentByClass<USwimmingComponent>();
	
	//TODO: 임시로 플레이어로 고정
	Target = Player;
	TargetLocation = NewIdleLocation();
	NextState = ESharkState::Idle;
	CurrentState = ESharkState::Turning;
}

// Called every frame
void ASharkAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// UE_LOG(LogTemp, Display, TEXT("%s"), *FEnumHelper::GetClassEnumKeyAsString(CurrentState));
	
	switch (CurrentState)
	{
	case ESharkState::Idle:
		Idle(DeltaTime);
		break;
	case ESharkState::MoveToTarget:
		MoveToTarget(DeltaTime);
		break;
	case ESharkState::Turning:
		Turning(DeltaTime);
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
	// UE_LOG(LogTemp,Warning,TEXT("상어 상태 변경"));
	CurrentState = newState;
}

void ASharkAI::Idle(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning,TEXT("%s"), *NewLocation.ToString());
	CurTimeforIdle += DeltaTime;
	float alpha = FMath::Clamp(CurTimeforIdle / IdleMoveDuration, 0.0f, 1.0f);
	
	FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, alpha);
	SetActorLocation(NewLocation);

	//idle 상태일 때 주기적으로 새로운 위치로 이동
	if (CurTimeforIdle > IdleMoveDuration)
	{
		CurTimeforIdle = 0.0f;
		TargetLocation = NewIdleLocation();
		NextState = ESharkState::Idle;
		ChangeState(ESharkState::Turning);
	}

	//플레이어가 물에 있으면 더 짧은 간격으로 공격
	if (SwimComponent->GetIsSwimMode())
	{
		Target = Player;
		CurTimeforAttack += DeltaTime*2;
	}
	else
	{
		CurTimeforAttack += DeltaTime;
	}
	
	if (CurTimeforAttack > SharkAttackDuration)
	{
		CurTimeforAttack = 0.0f;
		//TODO: 임시로 플레이어로 고정
		TargetLocation = Player->GetActorLocation();
		NextState = ESharkState::MoveToTarget;
		ChangeState(ESharkState::Turning);
	}
}

void ASharkAI::MoveToTarget(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning,TEXT("%s"), *(Target->GetActorLocation().ToString()));

	FVector curLoc = GetActorLocation();
	FVector dir = Target->GetActorLocation()-curLoc;
	dir.Normalize();
	SetActorLocation(dir*DeltaTime*SharkAttackSpeed + curLoc);
	if (FVector::Dist(curLoc,Target->GetActorLocation()) < DetectionDistance)
	{
		// UE_LOG(LogTemp,Warning,TEXT("타겟에 도착! Attack으로 변경"));
		if (Target == Player) ChangeState(ESharkState::AttackPlayer);
		else ChangeState(ESharkState::AttackRaft);
	}
}

void ASharkAI::AttackPlayer(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning, TEXT("플레이어 공격"))
	UGameplayStatics::ApplyDamage(Player, 20.f,
		nullptr, this, UDamageType::StaticClass());
	TargetLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
	NextState = ESharkState::RunAway;
	ChangeState(ESharkState::Turning);
	// UE_LOG(LogTemp,Warning, TEXT("도망갈 좌표: %s"),*RunLocation.ToString());
}

void ASharkAI::AttackRaft(float DeltaTime)
{
	//타이머로 배 파괴 함수 실행.
	//배 파괴 함수가 3번 실행되면, Runaway
	//만약 플레이어에게 공격을 3번 당하면, Runaway 
}

void ASharkAI::Runaway(float DeltaTime)
{
	FVector dir = TargetLocation-GetActorLocation();
	dir.Normalize();
	SetActorLocation(dir*SharkAttackSpeed*DeltaTime + GetActorLocation());
	
	if (FVector::Dist(GetActorLocation(),TargetLocation) < 10.0)
	{
		// UE_LOG(LogTemp,Warning,TEXT("도망완료 idle로 변경"));
		TargetLocation = NewIdleLocation();
		NextState = ESharkState::Idle;
		ChangeState(ESharkState::Turning);
	}
}

FVector ASharkAI::NewRunawayLocation(FVector originLoc, float maxDist, float minDist)
{
	float angle = FMath::RandRange(0.0f, 360.0f);
	FVector direction = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), -0.5f);
	
	float randDist = FMath::RandRange(minDist, maxDist);
	FVector randomLoc = originLoc +direction * randDist;
	randomLoc.Z = FMath::Min(originLoc.Z, -1800.f); //바닥 뚫지 않게 제한
	
	return randomLoc;
}

FVector ASharkAI::NewIdleLocation()
{
	StartLocation = GetActorLocation();
	float angle = FMath::RandRange(0.0f, 360.0f);
	FVector direction = FVector(FMath::Cos(FMath::DegreesToRadians(angle)), FMath::Sin(FMath::DegreesToRadians(angle)), 0.0f);
	
	FVector randomLoc = direction*1000.0f+Player->GetActorLocation();
	randomLoc.Z = FMath::RandRange(-1000.0f,-100.0f); //높이 물속으로 제한
	
	// UE_LOG(LogTemp, Warning, TEXT("새 idle 위치: %s"), *TargetLocation.ToString());
	return randomLoc;
}

void ASharkAI::Turning(float DeltaTime)
{
	CurTimeforIdle += DeltaTime;
	FVector dir = (TargetLocation-GetActorLocation()).GetSafeNormal();
	FRotator rot = dir.Rotation();
	
	float rotSpeed = 1.0f;
	FRotator smoothRot = FMath::RInterpTo(GetActorRotation(), rot, CurTimeforIdle, rotSpeed);
	// UE_LOG(LogTemp, Warning, TEXT("Turning, %f"), CurTimeforIdle);
	
	SetActorRotation(smoothRot);
	if (this->GetActorRotation().Equals(rot,5.0f))
	{
		// UE_LOG(LogTemp, Warning, TEXT("턴 완료"));
		CurTimeforIdle = 0.0f;
		ChangeState(NextState);
	}
}


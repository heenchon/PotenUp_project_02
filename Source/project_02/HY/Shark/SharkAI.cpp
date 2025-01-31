// Fill out your copyright notice in the Description page of Project Settings.


#include "SharkAI.h"
#include "Kismet/GameplayStatics.h"
// #include "project_02/Helper/EnumHelper.h"
#include "Algo/RandomShuffle.h"
#include "Components/CapsuleComponent.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Characters/Component/SwimmingComponent.h"
#include "project_02/HY/Raft/Raft.h"
#include "project_02/Building/BuildingFloor.h"
#include "project_02/Weapon/WeaponBase.h"

// Sets default values
ASharkAI::ASharkAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComponent);
	MouthPosition = CreateDefaultSubobject<USceneComponent>(TEXT("MouthPositionComp"));
	MouthPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASharkAI::BeginPlay()
{
	Super::BeginPlay();
	Player = Cast<APawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	SwimComponent = Player->FindComponentByClass<USwimmingComponent>();

	Capsule->OnComponentBeginOverlap.AddDynamic(this,&ASharkAI::OnMyBeginOverlap);
	
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
	case ESharkState::MoveToRaft:
		MoveToRaft(DeltaTime);
		break;
	case ESharkState::AttackRaft:
		AttackRaft(DeltaTime);
		break;
	case ESharkState::Turning:
		Turning(DeltaTime);
		break;
	case ESharkState::MoveToPlayer:
		MoveToPlayer(DeltaTime);
		break;
	case ESharkState::AttackPlayer:
		AttackPlayer(DeltaTime);
		break;
	case ESharkState::RunAway:
		Runaway(DeltaTime);
		break;
	case ESharkState::Dead:
		break;
	default:
		break;
	}
}

void ASharkAI::OnMyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWeaponBase::StaticClass()))
	{
		--HealthPoint;
		if (HealthPoint <= 0)
		{
			Died();
		}
		if (CurrentState == ESharkState::AttackRaft)
		{
			++CurHitCount;
			UE_LOG(LogTemp,Display,TEXT("%d대 맞음"), CurHitCount);
			if (CurHitCount >= 3)
			{
				CurHitCount = 0;
				GetWorld()->GetTimerManager().ClearTimer(SharkTimerHandle);
				TargetLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
				NextState = ESharkState::RunAway;
				ChangeState(ESharkState::Turning);
			}
		}
	}
}

void ASharkAI::ChangeState(ESharkState newState)
{
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

	//플레이어가 물에 있으면 플레이어 공격
	if (SwimComponent->GetIsSwimMode())
	{
		TargetLocation = Player->GetActorLocation();
		NextState = ESharkState::MoveToPlayer;
		ChangeState(ESharkState::Turning);
	}
	else
	{
		CurTimeforAttack += DeltaTime;
	}
	
	if (CurTimeforAttack > SharkAttackDuration)
	{
		CurTimeforAttack = 0.0f;
		Floor = GetFloor();

		//공격할 수 있는 판자가 없으면 idle 진행
		if (Floor == nullptr)
		{
			UE_LOG(LogTemp, Display, TEXT("공격할 판자 없음"));
			TargetLocation = NewIdleLocation();
			NextState = ESharkState::Idle;
			ChangeState(ESharkState::Turning);
			return;
		}
		TargetLocation = Floor->GetActorLocation();
		NextState = ESharkState::MoveToRaft;
		ChangeState(ESharkState::Turning);
	}
}

void ASharkAI::MoveToRaft(float DeltaTime)
{
	// UE_LOG(LogTemp,Warning,TEXT("%s"), *(Target->GetActorLocation().ToString()));
	FVector curLoc = GetActorLocation();
	FVector targetLoc = Floor->GetActorLocation();
	FVector dir = targetLoc-curLoc;
	dir.Normalize();
	SetActorLocation(dir*DeltaTime*SharkAttackSpeed + curLoc);
	if (FVector::Dist(curLoc,targetLoc) < DetectionDistance)
	{
		//타이머로 배 파괴 함수 실행.
		GetWorld()->GetTimerManager().SetTimer(SharkTimerHandle,this,&ASharkAI::DamageFloor,FloorDestroyDuration,true);
		SetActorRotation(BiteRotation);
		ChangeState(ESharkState::AttackRaft);
	}
}

void ASharkAI::AttackRaft(float DeltaTime)
{
	SetActorLocation(Floor->GetActorLocation() - MouthPosition->GetComponentLocation()+ GetActorLocation());
}

void ASharkAI::DamageFloor()
{
	Floor->DecreaseDurability();
	
	if (Floor->GetDurability() <= 0.0f)
	{
		//배가 부서지면, Runaway
		Floor->Destroy();
		GetWorld()->GetTimerManager().ClearTimer(SharkTimerHandle);
		
		TargetLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
		NextState = ESharkState::RunAway;
		ChangeState(ESharkState::Turning);
	}
	
}

void ASharkAI::MoveToPlayer(float DeltaTime)
{
	//수영을 취소하면 다시 idle로.
	if (!SwimComponent->GetIsSwimMode())
	{
		// UE_LOG(LogTemp, Display, TEXT("배 위로 도망쳣네?"));
		TargetLocation = NewIdleLocation();
		NextState = ESharkState::Idle;
		ChangeState(ESharkState::Turning);
		return;
	}
	
	FVector curLoc = GetActorLocation();
	FVector targetLoc = Player->GetActorLocation();
	FVector dir = targetLoc-curLoc;
	dir.Normalize();
	SetActorLocation(dir*DeltaTime*SharkAttackSpeed + curLoc);
	
	if (FVector::Dist(curLoc,targetLoc) < DetectionDistance)
	{
		ChangeState(ESharkState::AttackPlayer);
	}
}

void ASharkAI::AttackPlayer(float DeltaTime)
{
	UGameplayStatics::ApplyDamage(Player, 10.f,
		nullptr, this, UDamageType::StaticClass());
	TargetLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
	NextState = ESharkState::RunAway;
	ChangeState(ESharkState::Turning);
	// UE_LOG(LogTemp,Display, TEXT("도망갈 좌표: %s"),*RunLocation.ToString());
}



void ASharkAI::Runaway(float DeltaTime)
{
	FVector dir = TargetLocation-GetActorLocation();
	dir.Normalize();
	SetActorLocation(dir*SharkAttackSpeed*DeltaTime + GetActorLocation());
	
	if (FVector::Dist(GetActorLocation(),TargetLocation) < 10.0)
	{
		// UE_LOG(LogTemp,Display,TEXT("도망완료 idle로 변경"));
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
	
	FVector randomLoc = direction*2000.0f+Player->GetActorLocation();
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

ABuildingFloor* ASharkAI::GetFloor()
{
	TArray<FVector> positionArr;
	const ABasePlayerController* PC = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	ARaft* raft = PC->GetPlayerRaft();
	raft->GetRaftBuildPointerData().GetKeys(positionArr);
	
	Algo::RandomShuffle(positionArr);

	//공격 가능한 판자 찾을 때까지 반복
	for (FVector& pos : positionArr)
	{
		ABuildingFloor* floor = Cast<ABuildingFloor>(raft->GetRaftBuildPointerData()[pos]);
		if (floor->GetIsMain())
		{
			continue;
		}
		if (IsAttackableFloor(positionArr, pos))
		{
			UE_LOG(LogTemp, Display, TEXT("공격할 판자 pos X: %f, Y: %f, Z: %f"), pos.X, pos.Y, pos.Z);
			return floor;
		}
	}
	return nullptr;
}

bool ASharkAI::IsAttackableFloor(const TArray<FVector>& positionArr, const FVector& floorPos)
{
	//판자의 동서남북에 해당하는 위치에 다른 판자가 있는지 체크
	FVector dirArr[] = {
		FVector(-1, 0, 0), 
		FVector(0, -1, 0),  
		FVector(0, 1, 0), 
		FVector(1, 0, 0)  
	};

	for (FVector& dir : dirArr)
	{
		//main판자는 항상 있는 것으로 판정
		if (floorPos+dir == FVector(0, 0, 0))
		{
			continue;
		}

		//어느 한 방향에 판자가 없으면, 그 판자는 공격 가능
		if (!positionArr.Contains(floorPos+dir))
		{
			//판자 공격할 때, 비어있는 방향으로부터 판자를 바라보도록 회전값 저장
			SetBiteRotation(dir);
			return true;
		}
	}
	return false;
}

void ASharkAI::SetBiteRotation(const FVector& dir)
{
	if (dir == FVector(0, 1, 0)) 
	{
		// UE_LOG(LogTemp, Display, TEXT("동쪽이 비었음."));
		BiteRotation = FRotator(30, -90, 0);
	}
	else if (dir == FVector(-1, 0, 0))
	{
		// UE_LOG(LogTemp, Display, TEXT("남쪽이 비었음."));
		BiteRotation = FRotator(30, 0, 0);
	}
	else if (dir == FVector(1, 0, 0))
	{
		// UE_LOG(LogTemp, Display, TEXT("북쪽이 비었음."));
		BiteRotation = FRotator(30, 180, 0);
	}
	else if (dir == FVector(0, -1, 0))
	{
		// UE_LOG(LogTemp, Display, TEXT("서쪽이 비었음."));
		BiteRotation = FRotator(30, 90, 0);
	}
}

void ASharkAI::Died()
{
	UE_LOG(LogTemp,Display,TEXT("상어 죽었썩"));
	CurrentState = ESharkState::Dead;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	TargetLocation = NewRunawayLocation(GetActorLocation(),MaxDist,MinDist);
	SetActorLocation(TargetLocation);
	GetWorld()->GetTimerManager().SetTimer(SharkTimerHandle,this, &ASharkAI::Respawn,20.0f,false);
}

void ASharkAI::Respawn()
{
	UE_LOG(LogTemp,Display,TEXT("상어 리스폰"));
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	TargetLocation = NewIdleLocation();
	NextState = ESharkState::Idle;
	CurrentState = ESharkState::Turning;
}

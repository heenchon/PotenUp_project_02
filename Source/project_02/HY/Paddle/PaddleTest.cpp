#include "PaddleTest.h"
#include "../RaftGameState.h"
#include "../Raft/Raft.h"
#include "Kismet/GameplayStatics.h"


APaddleTest::APaddleTest()
{
}

void APaddleTest::BeginPlay()
{
	Super::BeginPlay();
	if (!Player)
	{
		Player = UGameplayStatics::GetPlayerPawn(GetWorld(),0);
		// UE_LOG(LogTemp,Warning,TEXT("플레이어는? %s"),*Player->GetName());
	}
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();
	if (RaftGameState)
	{
		WindOriginDir = RaftGameState->WindDirection;
	}
	
}

void APaddleTest::PaddlingStart()
{
	if (!bIsPaddling)
	{
		if (!GetPlayerRaft()) return;
	
		UE_LOG(LogTemp, Warning, TEXT("Paddling..."));
	
		//Yaw값만을 반영하는 플레이어 정면
		FRotator PlayerRotator = Player->GetActorRotation();
		FVector PlayerForwardVector = FRotator(0.0f, PlayerRotator.Yaw,0.0f).Vector();
	
		WindOriginDir = RaftGameState->WindDirection; //원본 바람 방향 백업
		WindOriginStrength = RaftGameState->WindStrength; //원본 바람 힘 백업
		Raft->WindDirection = PlayerForwardVector + WindOriginDir; //기존 바람 방향에 노 방향을 더한 새 방향
		Raft->WindStrength *= PaddleForce;
	
		// UE_LOG(LogTemp, Warning, TEXT("플레이어 방향: %s"), *PlayerForwardVector.ToString());
		// UE_LOG(LogTemp, Warning, TEXT("최종 이동 방향: %s"),*(Raft->WindDirection .ToString()));
	
		bIsPaddling= true;
	}
}

void APaddleTest::PaddlingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("paddling stop"));
	Raft->WindDirection = WindOriginDir; //바람 방향 되돌리기
	Raft->WindStrength = WindOriginStrength; //바람 힘 되돌리기

	bIsPaddling= false;
}

bool APaddleTest::GetPlayerRaft()
{
	FVector start = Player->GetActorLocation();
	FVector end = start - FVector(0.f,0.f,300.0f);

	FHitResult hit;
	FCollisionQueryParams params;
	params.AddIgnoredActor(Player);

	bool bHit = GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_Visibility, params);
	DrawDebugLine(GetWorld(), start, end, bHit ? FColor::Red:FColor::Green, false,0,0,2.0f);
	
	if (bHit)
	{
		FVector hitLoc = hit.ImpactPoint;
		DrawDebugPoint(GetWorld(), hitLoc, 20, FColor::Red, false, 0.0f, 2.0f);

		ARaft* hitRaft = Cast<ARaft>(hit.GetActor());
		if (hitRaft)
		{
			Raft = hitRaft;
			// UE_LOG(LogTemp, Warning, TEXT("배를 타고 있네요."));
			return true;
		}
	}
	UE_LOG(LogTemp,Warning,TEXT("배를 안 타고 있어요. 노는 작동하지 않아요."));
	return false;
}





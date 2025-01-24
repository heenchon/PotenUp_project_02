#include "PaddleTest.h"
#include "../RaftGameState.h"
#include "../Raft/Raft.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"


APaddleTest::APaddleTest()
{
	PaddlingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Paddling Timeline Component"));
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

	DivingCallback.BindDynamic(this, &ThisClass::OnPaddlingPlayCallback);
	DivingFinish.BindDynamic(this, &ThisClass::OnPaddlingFinish);

	PaddlingTimeline->SetLooping(true);
	PaddlingTimeline->AddInterpFloat(PaddlingTimingCurve, DivingCallback);
	PaddlingTimeline->SetTimelineFinishedFunc(DivingFinish);
}

void APaddleTest::PaddlingStart()
{
	if (!bIsPaddling)
	{
		if (!GetPlayerRaft()) return;
	
		UE_LOG(LogTemp, Display, TEXT("Paddling..."));
	
		//Yaw값만을 반영하는 플레이어 정면
		const FRotator PlayerRotator = Player->GetActorRotation();
		PaddleVelocity = FRotator(0.0f, PlayerRotator.Yaw,0.0f).Vector();
		
		PaddlingTimeline->PlayFromStart();
		bIsPaddling= true;
	}
}

void APaddleTest::OnPaddlingPlayCallback(float Output)
{
	if (!GetPlayerRaft()) return;
	Raft->AddActorLocalOffset(PaddleVelocity * Output);
}

void APaddleTest::PaddlingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("paddling stop"));
	bIsPaddling= false;
	PaddlingTimeline->Stop();
}

void APaddleTest::OnPaddlingFinish()
{
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
	// UE_LOG(LogTemp,Warning,TEXT("배를 안 타고 있어요. 노는 작동하지 않아요."));
	return false;
}





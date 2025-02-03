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
	}
	RaftGameState = GetWorld()->GetGameState<ARaftGameState>();

	DivingCallback.BindDynamic(this, &ThisClass::OnPaddlingPlayCallback);

	PaddlingTimeline->SetLooping(true);
	PaddlingTimeline->AddInterpFloat(PaddlingTimingCurve, DivingCallback);
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

		if (!TimerToSail.IsValid())
		{
			GetWorld()->GetTimerManager().SetTimer(TimerToSail,
				FTimerDelegate::CreateUObject(
					this, &ThisClass::OnPaddleingInTime), SoundCooldown, true, 0);
		} else
		{
		GetWorld()->GetTimerManager().UnPauseTimer(TimerToSail);
		}
		PaddlingTimeline->PlayFromStart();
		bIsPaddling= true;
	}
}

void APaddleTest::OnPaddleingInTime()
{
	UGameplayStatics::PlaySoundAtLocation(
		GetWorld(), PaddlingSound, GetActorLocation(), GetActorRotation());
}


void APaddleTest::OnPaddlingPlayCallback(float Output)
{
	if (!GetPlayerRaft()) return;
	Raft->AddActorLocalOffset(PaddleVelocity * Output);
}

void APaddleTest::PaddlingEnd()
{
	bIsPaddling= false;
	GetWorld()->GetTimerManager().PauseTimer(TimerToSail);
	PaddlingTimeline->Stop();
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





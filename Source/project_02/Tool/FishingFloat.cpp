

#include "FishingFloat.h"

#include "FishingRod.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


AFishingFloat::AFishingFloat()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
	Collision = CreateDefaultSubobject<UBoxComponent>("Collision");
	Collision->SetupAttachment(StaticMesh);
}

void AFishingFloat::BeginPlay()
{
	Super::BeginPlay();
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlap);
}

void AFishingFloat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsThrowing)
	{
		Throwing(DeltaTime);
		return;
	}
	if (bIsWaiting)
	{
		Waiting(DeltaTime);
		return;
	}
	if (bIsFish)
	{
		Fishing(DeltaTime);
	}
}

void AFishingFloat::StartWaiting()
{
	WaitTime = FMath::RandRange(4.0f, 8.0f);
	bIsWaiting = true;
}

void AFishingFloat::Waiting(float deltaTime)
{
	CurTime += deltaTime;
	if (CurTime > WaitTime)
	{
		CurTime = 0.0f;
		//낚싯대->물고기 낚였다는 정보 전달.
		FishingRod->StartFishing();
		bIsWaiting = false;
		bIsFish = true;
	}
}

void AFishingFloat::Fishing(float deltaTime)
{
	// UE_LOG(LogTemp, Display, TEXT("물고기와 겨루는 중..."));
	CurTime += deltaTime;
	if (CurTime > FishingTime)
	{
		UE_LOG(LogTemp, Display, TEXT("물고기 놓쳤썩"));
		FishingRod->FishingFail();
		Destroy();
	}
}

void AFishingFloat::StartThrow(const FVector& startLoc, float& power)
{
	power = FMath::Clamp(power, 100.0f, 700.0f);
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	StartLocation = startLoc;
	Velocity = Player->GetActorForwardVector()*power;
	bIsThrowing = true;
}

void AFishingFloat::Throwing(float deltaTime)
{
	CurTime += deltaTime;
	FVector NewLocation = StartLocation + (Velocity * CurTime) 
	   + (0.5f * FVector(0, 0, Gravity) * CurTime * CurTime);
	
	SetActorLocation(NewLocation);
}

void AFishingFloat::SetFishingRod(AFishingRod* fishingRod)
{
	FishingRod = fishingRod;
}

void AFishingFloat::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsThrowing)
	{
		bIsThrowing = false;
		
		StartLocation = Player->GetActorLocation();
		FVector EndLocation = StartLocation;
		EndLocation.Z -= 5000;

		TArray<AActor*> IgnoreActorList;
		IgnoreActorList.Add(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		FHitResult HitResult;
		// 사용한 플레이어 기준으로 아래를 바라보고 아래의 액터를 기준으로 위치를 고정하는 로직을 실행한다.
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			StartLocation,
			EndLocation,
			TraceTypeQuery1,
			false,
			IgnoreActorList,
			EDrawDebugTrace::ForOneFrame,
			HitResult,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			10
		))
		{
			AttachToActor(HitResult.GetActor(), FAttachmentTransformRules::KeepWorldTransform);
		}
		StartWaiting();
	}
}


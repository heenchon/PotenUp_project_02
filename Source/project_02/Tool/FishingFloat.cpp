

#include "FishingFloat.h"

#include "VectorTypes.h"


AFishingFloat::AFishingFloat()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Root);
}

void AFishingFloat::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFishingFloat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsThrowing)
	{
		Throwing(DeltaTime);
		return;
	}
	if (bIsFishing)
	{
		Fishing(DeltaTime);
	}
}

void AFishingFloat::Throwing(float deltaTime)
{
	CurTime += deltaTime;
	
	float t = FMath::Clamp(CurTime / ThrowDuration, 0.0f, 1.0f);
	
	FVector curLoc = FMath::Lerp(StartLocation, FishingLocation, t); 
	curLoc.Z = FMath::Lerp(StartLocation.Z, FishingLocation.Z, t) 
						+ MaxThrowHeight * (1.0f - FMath::Pow(2.0f * t - 1.0f, 2.0f));
	
	SetActorLocation(curLoc);
	DrawDebugDirectionalArrow(GetWorld(), curLoc, curLoc, 100.0f, FColor::Yellow, true, 1.0f, 0, 2.0f);
	if (t >= 1.0f)
	{
		CurTime = 0.0f;
		bIsThrowing = false;
		bIsFishing = true;
	}
}

void AFishingFloat::StartFishing()
{
}

void AFishingFloat::Fishing(float deltaTime)
{
}

void AFishingFloat::GetFish()
{
}

void AFishingFloat::StartThrow(const FVector& startLoc, const FVector& endLoc)
{
	UE_LOG(LogTemp, Warning, TEXT("FishingFloat::InitThrow"));
	StartLocation = startLoc;
	FishingLocation = endLoc;
	bIsThrowing = true;
}


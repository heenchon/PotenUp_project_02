

#include "FishingFloat.h"
#include "../Player/BasePlayerState.h"
#include "../Helper/ItemHelper.h"
#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/InventoryComponent.h"


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
	PS = UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetPlayerState<ABasePlayerState>();
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
	DrawDebugDirectionalArrow(GetWorld(), curLoc, curLoc, 100.0f, FColor::Magenta, true, 1.0f, 0, 2.0f);
	
	if (t >= 1.0f)
	{
		CurTime = 0.0f;
		bIsThrowing = false;
		StartFishing();
	}
}

void AFishingFloat::StartFishing()
{
	FishingTime = FMath::RandRange(4.0f, 8.0f);
	bIsFishing = true;
}

void AFishingFloat::Fishing(float deltaTime)
{
	CurTime += deltaTime;
	if (CurTime > FishingTime)
	{
		CurTime = 0.0f;
		bIsFishing = false;
		GetFish();
	}
}

void AFishingFloat::GetFish()
{
	FItemMetaInfo NewFish = FItemHelper::GetInitialItemMetaDataById(GetWorld(), 8);
	PS->AddItem(NewFish);
	Destroy();
}

void AFishingFloat::StartThrow(const FVector& startLoc, float& power)
{
	UE_LOG(LogTemp, Warning, TEXT("%f"),power);
	power = FMath::Clamp(power, 60.0f, 500.0f);
	Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	FishingLocation = startLoc + Player->GetActorForwardVector()*power;
	FishingLocation.Z = 0.0f; 
	// DrawDebugSphere(GetWorld(), FishingLocation, 10.0f, 12, FColor::Red,false,2.0f,0,0.0f);
	
	StartLocation = startLoc;
	bIsThrowing = true;
}


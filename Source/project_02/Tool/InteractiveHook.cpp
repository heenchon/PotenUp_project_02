#include "InteractiveHook.h"
#include "WaterBodyOceanActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AInteractiveHook::AInteractiveHook()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	HookMesh->SetupAttachment(RootComponent);
	
	GrabOverlapBox = CreateDefaultSubobject<USphereComponent>("Grab Overlap Box");
	GrabOverlapBox->SetupAttachment(HookMesh);
}

// Called when the game starts or when spawned
void AInteractiveHook::BeginPlay()
{
	Super::BeginPlay();
	GrabOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapHookGrab);
}

void AInteractiveHook::StartLaunch(const FVector& MoveToVector, const uint8 NewPower)
{
	HookStatus = EHookStatus::Launched;
	Power = NewPower;
	MovetoPos = MoveToVector;
}

// Called every frame
void AInteractiveHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HookStatus == EHookStatus::Launched)
	{
		const FVector Position = GetActorLocation() + 
			(MovetoPos * DeltaTime * Power * PowerPercent) + 
			(FVector::UpVector * (-1.f * GravityScale * DeltaTime));

		SetActorLocation(Position);
	}
}

void AInteractiveHook::OnOverlapHookGrab(
		UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex,
		bool bFromSweep, 
		const FHitResult &SweepResult)
{
	if (OtherActor == this) return;
	
	UE_LOG(LogTemp, Display, TEXT("테스트 Overlap 실행 확인"));

	HookStatus = EHookStatus::Idle;
	MovetoPos = FVector::ZeroVector;
	
	if (Cast<AWaterBodyOcean>(OtherActor))
	{
		HookStatus = EHookStatus::Idle;
		MovetoPos = FVector::ZeroVector;
	}
	
}

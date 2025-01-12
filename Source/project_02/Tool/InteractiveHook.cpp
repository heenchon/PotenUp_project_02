#include "InteractiveHook.h"
#include "WaterBodyOceanActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/HY/Trash/Trash.h"

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

double AInteractiveHook::GetDistanceBetweenMoveToAndCurrentLocation()
{
	return UKismetMathLibrary::Distance2D({GetActorLocation().X, GetActorLocation().Y}, {MoveToPos.X, MoveToPos.Y});
}


bool AInteractiveHook::PullHook(const FVector& TargetLocation)
{
	MoveToPos = TargetLocation;
	HookStatus = EHookStatus::Pulled;

	return GetDistanceBetweenMoveToAndCurrentLocation() < HookCalcRadius;
}

void AInteractiveHook::StartLaunch(const FVector& MoveToVector, const uint8 NewPower)
{
	HookStatus = EHookStatus::Launched;
	Power = NewPower;
	MoveToPos = MoveToVector;
}

// Called every frame
void AInteractiveHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HookStatus == EHookStatus::Launched)
	{
		AddActorWorldOffset(MoveToPos * DeltaTime * Power * PowerPercent + 
                            			FVector::UpVector * (-1.f * GravityScale * DeltaTime));
	}
	
	if (HookStatus == EHookStatus::Pulled)
	{
		if (GetDistanceBetweenMoveToAndCurrentLocation() < HookCalcRadius)
		{
			Destroy();
		}

		FVector MoveTo = (MoveToPos - GetActorLocation())
			.GetSafeNormal();
		MoveTo.Z = 0;
		
		AddActorWorldOffset(MoveTo * DeltaTime * HookPullSpeed);
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

	HookStatus = EHookStatus::Fixed;
	MoveToPos = FVector::ZeroVector;

	UE_LOG(LogTemp, Display, TEXT("%s"), *OtherActor->GetName())
	if (ATrash* NewTrash = Cast<ATrash>(OtherActor))
	{
		NewTrash->StaticMesh->SetSimulatePhysics(false);
		NewTrash->SetActorLocation(GetActorLocation());
		NewTrash->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
		// TODO: 폐품의 바람 이동을 막아야함.
	}
}

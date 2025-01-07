#include "InteractiveHook.h"
#include "BuoyancyComponent.h"
#include "WaterBodyOceanActor.h"
#include "Components/SphereComponent.h"

// Sets default values
AInteractiveHook::AInteractiveHook()
{
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	SetRootComponent(RootComponent);
	
	GrabOverlapBox = CreateDefaultSubobject<USphereComponent>("Grab Overlap Box");
	GrabOverlapBox->SetupAttachment(HookMesh);
	
	Buoyancy = CreateDefaultSubobject<UBuoyancyComponent>(TEXT("Buoyancy"));
	Buoyancy->AddCustomPontoon(5, "Hook Center");
}

// Called when the game starts or when spawned
void AInteractiveHook::BeginPlay()
{
	Super::BeginPlay();
	GrabOverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapHookGrab);
}

// Called every frame
void AInteractiveHook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

	if (Cast<AWaterBodyOcean>(OtherActor))
	{
		UE_LOG(LogTemp, Display, TEXT("하이하이하이"));
	}
	// TODO: 만약 부력을 없앤 상태로 처리할 거라면 이 코드를 살려야함.
	// if (Cast<AWaterBodyOcean>(OtherActor))
	// {
	// 	HookMesh->SetSimulatePhysics(false);
	// 	HookMesh->SetRelativeRotation({0, 0, 90});
	// }
}

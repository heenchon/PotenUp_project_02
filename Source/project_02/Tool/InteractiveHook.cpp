#include "InteractiveHook.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/HY/Trash/Trash.h"
#include "project_02/Player/BasePlayerState.h"

// Sets default values
AInteractiveHook::AInteractiveHook()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	HookMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AInteractiveHook::BeginPlay()
{
	Super::BeginPlay();
	// 현재 HookMesh는 Object Channel이 WorldDynamic이고
	// 갈고리에 연결할 쓰레기도 WorldDynamic이다.
	// 그리고 서로 간의 WorldDynamic에 Overlap 속성을 줌으로써 서로 Overlap시
	// 끌어당기게 처리해뒀다.
	// TODO: 추후 이 규칙에 대해 정리하기 (알파 ~ 베타 중에서 정리하기)
	HookMesh->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapHookGrab);
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

	switch (HookStatus)
	{
		case EHookStatus::Launched:
		{
			AddActorWorldOffset(MoveToPos * DeltaTime * Power * PowerPercent + 
										FVector::UpVector * (-1.f * GravityScale * DeltaTime));
			break;
		}
		case EHookStatus::Pulled:
		{
			if (GetDistanceBetweenMoveToAndCurrentLocation() < HookCalcRadius)
			{
				for (ATrash* TrashItem : AttachTrashList)
				{
					// TODO: Delegate로 이전하는 방향성을 고려해보기
					if (ABasePlayerState* PS = Cast<ABasePlayerState>(UGameplayStatics::GetPlayerState(GetWorld(), 0)))
					{
						PS->AddItem(TrashItem->GetItemMetaInfo());
						TrashItem->Destroy();
					}
				}
			
				Destroy();
			}

			FVector MoveTo = (MoveToPos - GetActorLocation())
				.GetSafeNormal();
			MoveTo.Z = 0;
		
			AddActorWorldOffset(MoveTo * DeltaTime * HookPullSpeed);
			
			break;
		}
		default:
		{
			break;
		}
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
	UE_LOG(LogTemp, Display, TEXT("Test: %s"), *OtherActor->GetName())
	if (OtherActor == this) return;

	HookStatus = EHookStatus::Fixed;
	MoveToPos = FVector::ZeroVector;

	if (ATrash* NewTrash = Cast<ATrash>(OtherActor))
	{
		NewTrash->StaticMesh->SetSimulatePhysics(false);
		NewTrash->StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		NewTrash->SetActorLocation(GetActorLocation());
		NewTrash->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
		
		AttachTrashList.Add(NewTrash);
	}
}

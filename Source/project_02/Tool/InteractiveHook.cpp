#include "InteractiveHook.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "project_02/HY/Trash/Trash.h"
#include "project_02/HY/Trash/TrashSpawner.h"
#include "project_02/Player/BasePlayerState.h"

// Sets default values
AInteractiveHook::AInteractiveHook()
{
	PrimaryActorTick.bCanEverTick = true;
	
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultRoot);
	
	HookMesh = CreateDefaultSubobject<UStaticMeshComponent>("Hook Mesh");
	HookMesh->SetupAttachment(RootComponent);

	BoxCollision = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	BoxCollision->SetupAttachment(HookMesh);
}

// Called when the game starts or when spawned
void AInteractiveHook::BeginPlay()
{
	Super::BeginPlay();
	// 현재 HookMesh는 Object Channel이 WorldDynamic이고
	// 갈고리에 연결할 쓰레기도 WorldDynamic이다.
	// 그리고 서로 간의 WorldDynamic에 Overlap 속성을 줌으로써 서로 Overlap시
	// 끌어당기게 처리해뒀다.
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapHookGrab);
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
						ATrashSpawner* TS = Cast<ATrashSpawner>(UGameplayStatics::GetActorOfClass(GetWorld(), ATrashSpawner::StaticClass()));
						TS->RespawnTrashAt(TrashItem);
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
	// 공격용은 아니기 때문에 Pawn 타입이면 그냥 전부 무시한다.
	if (OtherActor == this || OtherActor->IsA(APawn::StaticClass())) return;

	if (!OtherActor->IsA(ATrash::StaticClass())
		&& HookStatus == EHookStatus::Launched)
	{
		HookStatus = EHookStatus::Fixed;
		MoveToPos = FVector::ZeroVector;
		
		const FVector StartLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
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
	}

	if (ATrash* NewTrash = Cast<ATrash>(OtherActor))
	{
		NewTrash->StaticMesh->SetSimulatePhysics(false);
		NewTrash->StaticMesh->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		NewTrash->SetActorLocation(GetActorLocation());

		// 있는 만큼 Yaw 값을 돌려서 여러개가 있음을 표시
		FRotator NewRotator = FRotator::ZeroRotator;
		// TODO: 임시 로직으로 많은 것을 부착할 때 더 고도화 해보기
		NewRotator.Yaw += AttachTrashList.Num() * 10;
		NewTrash->SetActorRotation(NewRotator);
		
		NewTrash->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform, "");
		// TODO: 갈고리가 중간에 없어지는 경우에 대한 예외처리가 없음
		// 갈고리 중간에 없어지면 IsStop을 false로 다시 변경해두기
		NewTrash->IsStop = true;
		AttachTrashList.Add(NewTrash);
	}
}

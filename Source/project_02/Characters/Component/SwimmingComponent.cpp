#include "SwimmingComponent.h"

#include "WaterBodyOceanActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "project_02/Characters/PlayerCharacter.h"

USwimmingComponent::USwimmingComponent()
{
	DivingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Diving Timeline Component"));
}

void USwimmingComponent::BeginPlay()
{
	Super::BeginPlay();

	// 여기서 실패하면 어떤 것도 할 수 없다.
	if (GetOwner()->IsA(APlayerCharacter::StaticClass()))
	{
		Owner = static_cast<APlayerCharacter*>(GetOwner());
		
		Owner->GetChestBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCheckOverlapInWater);
		Owner->GetChestBox()->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCheckOverlapOutWater);

		DivingCallback.BindDynamic(this, &ThisClass::OnDivingPlayCallback);
		DivingFinish.BindDynamic(this, &ThisClass::OnDivingFinish);
		
		DivingTimeline->SetLooping(false);
		DivingTimeline->AddInterpFloat(DivingTimingCurve, DivingCallback);
		DivingTimeline->SetTimelineFinishedFunc(DivingFinish);
	}
}

void USwimmingComponent::OnCheckOverlapInWater(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AWaterBodyOcean::StaticClass()))
	{
		IsSwimMode = true;
		WaterLevel = OtherActor->GetActorLocation().Z;

		if (!IsDiving) {
			Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
			Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		}
		
	}
}

void USwimmingComponent::OnCheckOverlapOutWater(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AWaterBodyOcean::StaticClass()))
	{
		IsSwimMode = false;
		Owner->GetCharacterMovement()->bOrientRotationToMovement = false;
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	}
}

bool USwimmingComponent::IsOwnerNearWaterLevel(const FVector& MoveToVector) const
{
	// 올라가는 방향의 Z값이 0보다 커야한다. (올라가기 때문)
	// 그리고 현재 Actor의 Z값 좌표에 움직이는 좌표 값이 더해진 값이
	// WaterLevel (수면 Z 좌표값)에 캐릭터의 Capsule 높이의 비율 값 만큼
	// 빠진 높이보다 크거나 같은 경우가 수면 근처에 있는 상태라고 정의된다.
	return MoveToVector.Z > 0 && Owner->GetActorLocation().Z + MoveToVector.Z >=
			GetWaterLevel() - Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() /
				GetFloatingValueInWaterPercent();
}

void USwimmingComponent::OnDivingPlayCallback(float Output)
{
	const FVector MoveTo = Owner->GetActorForwardVector() * DivingMoveForwardPower
		+ FVector(0, 0, Output * DivingJumpPower);
	Owner->GetCharacterMovement()->Launch(MoveTo);
}

void USwimmingComponent::OnDivingFinish()
{
	Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	IsDiving = false;
}

void USwimmingComponent::PlayDiving() const
{
	DivingTimeline->PlayFromStart();
}


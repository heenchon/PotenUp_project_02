#include "SwimmingComponent.h"

#include "WaterBodyOceanActor.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "project_02/Characters/PlayerCharacter.h"

USwimmingComponent::USwimmingComponent()
{
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
		Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
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

bool USwimmingComponent::CanMoveToUpInSwimming(const FVector MoveToVector) const
{
	const FVector StartPos = Owner->GetActorLocation();
	FVector EndPos = Owner->GetActorLocation();
	EndPos.X += 200;
	FHitResult HitResult;
	TArray<AActor*> IgnoreActor;
	IgnoreActor.Add(Owner);
	
	const bool HasObjectOnFront = UKismetSystemLibrary::LineTraceSingle(
		GetWorld(), StartPos, EndPos,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true, IgnoreActor, EDrawDebugTrace::ForOneFrame,
		HitResult, true);

	if (HasObjectOnFront) return true;
	
	return !(MoveToVector.Z > 0 && Owner->GetActorLocation().Z + MoveToVector.Z >=
			GetWaterLevel() - Owner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() /
				GetFloatingValueInWaterPercent());
}

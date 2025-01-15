#include "SwimmingComponent.h"

#include "WaterBodyOceanActor.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
		
		Owner->GetCheckSwimOverlapBox()->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnCheckOverlapInWater);
		Owner->GetCheckSwimOverlapBox()->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnCheckOverlapOutWater);
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
		Owner->GetCharacterMovement()->bOrientRotationToMovement = true;
		Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		UE_LOG(LogTemp, Display, TEXT("Check Capsule Overlap Start: %s"), *OtherActor->GetName());
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
		UE_LOG(LogTemp, Display, TEXT("Check Capsule Overlap End: %s"), *OtherActor->GetName());
	}
}
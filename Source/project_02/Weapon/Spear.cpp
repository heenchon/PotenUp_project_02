#include "Spear.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/SwimmingComponent.h"
#include "project_02/DataTable/EntityAnimationInfo.h"


ASpear::ASpear()
{
}

void ASpear::BeginPlay()
{
	Super::BeginPlay();
}

void ASpear::Attack()
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		// 수영중일 때는 행동 불가능
		if (Player->GetSwimmingComponent()->GetIsSwimMode())
		{
			return;
		}
		Player->GetCharacterMovement()->DisableMovement();
		Player->PlayAnimMontage(
			Player->GetAnimationData()->AttackAnimation);
	}
}


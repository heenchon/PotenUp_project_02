#include "Spear.h"

#include "project_02/Characters/PlayerCharacter.h"
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
		Player->PlayAnimMontage(
			Player->GetAnimationData()->AttackAnimation);
	}
}


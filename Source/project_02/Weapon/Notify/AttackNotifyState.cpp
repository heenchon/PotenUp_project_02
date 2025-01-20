// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackNotifyState.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Weapon/WeaponBase.h"

void UAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp,
                                     UAnimSequenceBase* Animation, float TotalDuration,
                                     const FAnimNotifyEventReference& EventReference)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(Player->GetMainHandTool()))
		{
			Weapon->ToggleWeaponAttack(true);
		}
	}
}

void UAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner()))
	{
		if (AWeaponBase* Weapon = Cast<AWeaponBase>(Player->GetMainHandTool()))
		{
			Weapon->ToggleWeaponAttack(false);
		}
		Player->GetCharacterMovement()->SetDefaultMovementMode();
	}
}


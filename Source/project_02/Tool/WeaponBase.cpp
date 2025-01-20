#include "WeaponBase.h"

AWeaponBase::AWeaponBase()
{
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Body");
	WeaponBody->SetupAttachment(GetRootComponent());
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AWeaponBase::Attack()
{
	
}


#include "WeaponBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

AWeaponBase::AWeaponBase()
{
	DefaultRoot = CreateDefaultSubobject<USceneComponent>("Default Root");
	SetRootComponent(DefaultRoot);
	
	WeaponBody = CreateDefaultSubobject<UStaticMeshComponent>("Weapon Body");
	WeaponBody->SetupAttachment(GetRootComponent());
	
	AttackCollision = CreateDefaultSubobject<USphereComponent>("Attack Collision");
	AttackCollision->SetupAttachment(WeaponBody);
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	AttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapAttackCollision);
}

void AWeaponBase::Attack()
{
	
}

void AWeaponBase::ToggleWeaponAttack(const bool IsAttack)
{
	AttackCollision->SetCollisionEnabled(IsAttack ?
		ECollisionEnabled::Type::QueryAndPhysics
		: ECollisionEnabled::Type::NoCollision);
}


void AWeaponBase::OnOverlapAttackCollision(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner() || OtherActor == this)
	{
		return;
	}
	// UGameplayStatics::ApplyDamage(OtherActor, Damage,
	// 	nullptr, GetOwner(),
	// 	UDamageType::StaticClass());
}


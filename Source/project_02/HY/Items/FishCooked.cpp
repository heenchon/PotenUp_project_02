// Fill out your copyright notice in the Description page of Project Settings.


#include "FishCooked.h"

#include "Kismet/GameplayStatics.h"
#include "project_02/Characters/Component/SurvivalComponent.h"
#include "project_02/Characters/PlayerCharacter.h"
#include "project_02/Characters/Component/InventoryComponent.h"
#include "project_02/Player/BasePlayerState.h"


// Sets default values
AFishCooked::AFishCooked()
{
}

// Called when the game starts or when spawned
void AFishCooked::BeginPlay()
{
	Super::BeginPlay();
	if (const APlayerCharacter* Player =
		Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		SurvivalComponent = Player->GetSurvivalComponent();
		InventoryComponent = Player->GetInventoryComponent();
		PS = Player->GetPlayerState<ABasePlayerState>();
	}
}

void AFishCooked::Use()
{
	Super::Use();
	PS->DropItem(InventoryComponent->GetSelectedHotSlotIndex(),1);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(),
		UsedSound, GetActorLocation(), GetActorRotation());
	SurvivalComponent->IncreaseHunger(10);
	this->Destroy();
}

FString AFishCooked::GetDisplayText() const
{
	return FoodInteractiveText;
}


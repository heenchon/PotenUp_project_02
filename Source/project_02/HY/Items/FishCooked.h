// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Usable_Item.h"
#include "FishCooked.generated.h"

class APlayerCharacter;
class USurvivalComponent;
class UInventoryComponent;
class ABasePlayerState;

UCLASS()
class PROJECT_02_API AFishCooked : public AUsable_Item
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFishCooked();

	UPROPERTY(EditAnywhere)
	USurvivalComponent* SurvivalComponent;
	UPROPERTY(EditAnywhere)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditAnywhere)
	ABasePlayerState* PS;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Use() override;

	virtual FString GetDisplayText() const override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Options|Text", meta = (AllowPrivateAccess = true))
	FString FoodInteractiveText = TEXT("Get Fish");
};

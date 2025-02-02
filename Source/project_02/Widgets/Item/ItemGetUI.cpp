// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemGetUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "project_02/Widgets/HUD/ItemUI.h"

void UItemGetUI::NativeConstruct()
{
	Super::NativeConstruct();
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &UItemGetUI::DestroyItemGetUI, 3.0f,false);
}

void UItemGetUI::DestroyItemGetUI()
{
	UE_LOG(LogTemp, Display, TEXT("디스트로이 item get UI"));
	MainItemUI->RemoveItemGetUI(this);
	RemoveFromParent();
}

void UItemGetUI::InitItemUI(UItemUI* mainUI,uint8 count,FString displayName,TObjectPtr<UTexture2D> texture)
{
	MainItemUI = mainUI;
	ItemCount->SetText(FText::AsNumber(count));
	ItemThumbnail->SetBrushFromTexture(texture);
	ItemName->SetText(FText::FromString(displayName));
}

void UItemGetUI::SetItemCount(uint8 count)
{
}

void UItemGetUI::SetItemName(FString displayName)
{
}

void UItemGetUI::SetItemThumbnail(TSoftObjectPtr<UTexture2D> texture)
{
}

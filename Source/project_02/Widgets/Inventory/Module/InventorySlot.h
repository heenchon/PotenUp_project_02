// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlot.generated.h"

class UOverlay;
class UBorder;
struct FItemMetaInfo;
class UTextBlock;
class UImage;


UCLASS()
class PROJECT_02_API UInventorySlot : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetThumbnail(const TSoftObjectPtr<UTexture2D>& Thumbnail) const;
	
	void SetItemCount(const uint16 NewCount) const;
	
	void SetItemCountText(const FText Text) const;
	
	void SetIsDragged(const bool bIsDragged);
	
	void SetSlotInfo(const FItemMetaInfo& ItemMetaInfo);

	void SetSelected(const bool bIsSelected) const;

	void RemoveDragDropSlot() const;

	FORCEINLINE void SetIndex(const uint16 NewIndex) { Index = NewIndex; }
	FORCEINLINE void SetSlotItemId(const uint16 NewSlotItemId) { SlotItemId = NewSlotItemId; }

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

private:
	bool IsDragUi;
	bool IsExecuteDrop = true;

	uint16 Index;
	// 해당 슬롯이 표현하는 아이템의 아이디
	uint16 SlotItemId;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UInventorySlot> DragItemWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UInventorySlot> DragItemWidget;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = true))
	TSubclassOf<UDragDropOperation> DragDropOptionsClass;
	
	FVector2d DragStartOffset;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UOverlay> ItemOverlay;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemThumbnail;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemCount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> SelectedBorder;
};
 
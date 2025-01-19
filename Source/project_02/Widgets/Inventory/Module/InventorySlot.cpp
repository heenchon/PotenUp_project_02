#include "InventorySlot.h"

#include "InventorySlotDragDropOperation.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/KismetInputLibrary.h"
#include "project_02/DataTable/ItemInfoData.h"
#include "project_02/Game/BaseGameInstance.h"
#include "project_02/Player/BasePlayerController.h"
#include "project_02/Player/BasePlayerState.h"

FReply UInventorySlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsDragUi)
	{
		Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	}
	
	DragStartOffset = USlateBlueprintLibrary::AbsoluteToLocal(InGeometry,
		UKismetInputLibrary::PointerEvent_GetScreenSpacePosition(InMouseEvent));
	
	return UWidgetBlueprintLibrary::DetectDragIfPressed(
		InMouseEvent,
		this,
		EKeys::LeftMouseButton
	).NativeReply;
}

void UInventorySlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (IsDragUi) return;
	
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	
	// 드래그를 위한 위젯 설정
	DragItemWidget = CreateWidget<UInventorySlot>(this, DragItemWidgetClass);
	DragItemWidget->SetThumbnail(ItemThumbnail->GetBrush().GetResourceObject());
	DragItemWidget->SetItemCountText(ItemCount->GetText());
	DragItemWidget->SetIsDragged(true);

	// 드래그 드롭 관련 오퍼레이션 설정
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOptionsClass);
	OutOperation->Pivot = EDragPivot::MouseDown;
	OutOperation->DefaultDragVisual = DragItemWidget;
	
	if (OutOperation->IsA(UInventorySlotDragDropOperation::StaticClass()))
	{
		static_cast<UInventorySlotDragDropOperation*>(OutOperation)->SetOriginWidget(this);
		static_cast<UInventorySlotDragDropOperation*>(OutOperation)->SetIndex(Index);
	}

	// 현재 내가 선택중인 Drag Drop 관련 Slot 정보 넣어두기
	if (ABasePlayerController* PC = Cast<ABasePlayerController>(GetOwningPlayer()))
	{
		PC->SetSelectedSlot(this);
	}
}

bool UInventorySlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (IsDragUi) return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	// 드래그 필요한 UI인 경우 여기에 접근하는 순간부터 우선은 true로 교체해준다.
	IsExecuteDrop = true;
	
	// 이 경우가 애초부터 인벤토리 칸과 인벤토리 칸과의 교차임을 의미한다.
	// 근데 이후 불안한 경우 내부에서 Operation의 Widget이 InvenSlot인지 확인
	if (!InOperation->IsA(UInventorySlotDragDropOperation::StaticClass()))
	{
		return false;
	}
	
	// TODO: 이 if문과 static cast를 이후 공통화 해서 유틸함수를 만들어보기
	if (!GetOwningPlayerPawn()->GetPlayerState()->IsA(ABasePlayerState::StaticClass()))
	{
		return false;
	}
	
	UInventorySlotDragDropOperation* DropOperation
		= static_cast<UInventorySlotDragDropOperation*>(InOperation);
	
	if (!DropOperation->GetOriginWidget().IsA(UInventorySlot::StaticClass()))
	{
		return false;
	}

	// TODO: 여기서 부터 아래 내용들은 전부 Delegate로 처리하게 해야한다.
	// 실제 로직은 PC에서 AddDynamic으로 연동 처리
	// UI 자체에 비지니스 로직이 들어가면 안된다.
	UInventorySlot* PrevInvSlot = static_cast<UInventorySlot*>(DropOperation->GetOriginWidget());
	ABasePlayerState* PS = static_cast<ABasePlayerState*>(GetOwningPlayerPawn()->GetPlayerState());
	
	SetSlotInfo(PS->GetPlayerInventoryList()[DropOperation->GetIndex()]);
	PrevInvSlot->SetSlotInfo(PS->GetPlayerInventoryList()[Index]);
	
	PS->SwapItemInInventory(
		DropOperation->GetIndex(),
		Index
	);

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UInventorySlot::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	if (IsDragUi) return;
	Super::NativeOnDragCancelled(InDragDropEvent, InOperation);
	if (IsExecuteDrop)
	{
		UE_LOG(LogTemp, Display, TEXT("드래그에서 떠나다...%d"), Index)
	}
	IsExecuteDrop = false;
}

void UInventorySlot::SetIsDragged(const bool bIsDragged)
{
	IsDragUi = bIsDragged;
	if (bIsDragged)
	{
		ItemOverlay->SetRenderOpacity(0.5);
	}
}

void UInventorySlot::SetSelected(const bool bIsSelected) const
{
	SelectedBorder->SetVisibility(bIsSelected ?
		ESlateVisibility::Visible : ESlateVisibility::Hidden);
}


void UInventorySlot::SetSlotInfo(const FItemMetaInfo& ItemMetaInfo)
{
	const UBaseGameInstance* GameInstance = static_cast<UBaseGameInstance*>(GetGameInstance());

	SetSlotItemId(ItemMetaInfo.GetId());
	SetThumbnail(GameInstance->GetItemInfoList()[ItemMetaInfo.GetId()].GetThumbnail());
	SetItemCount(ItemMetaInfo.GetCurrentCount());
}


void UInventorySlot::SetThumbnail(const TSoftObjectPtr<UTexture2D> Thumbnail) const
{
	ItemThumbnail.Get()->SetBrushFromTexture(Thumbnail.Get());
}


void UInventorySlot::SetItemCount(const uint16 NewCount) const
{
	if (NewCount > 1)
	{
		ItemCount->SetText(FText::AsNumber(NewCount));
	} else
	{
		ItemCount->SetText(FText::GetEmpty());
	}
}

// 외부에서 해당 ItemCount 에 직접 접근해 세팅하려는 Setter 함수
void UInventorySlot::SetItemCountText(const FText Text) const
{
	ItemCount->SetText(Text);
}

void UInventorySlot::RemoveDragDropSlot() const
{
	UWidgetBlueprintLibrary::CancelDragDrop();
}

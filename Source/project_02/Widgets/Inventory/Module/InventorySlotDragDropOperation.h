#pragma once

#include "CoreMinimal.h"
#include "project_02/Widgets/BaseDragDropOperation.h"
#include "InventorySlotDragDropOperation.generated.h"


UCLASS()
class PROJECT_02_API UInventorySlotDragDropOperation : public UBaseDragDropOperation
{
	GENERATED_BODY()

public:
	FORCEINLINE uint16 GetIndex() const { return Index; }
	FORCEINLINE void SetIndex(const uint16 NewIndex) { Index = NewIndex; }
	
private:
	uint16 Index;
};

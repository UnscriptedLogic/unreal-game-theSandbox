#pragma once

#include "InventoryOperationStruct.generated.h"

UENUM(BlueprintType)
enum class EInventoryOperationType : uint8
{
	AppendingToStack UMETA(displayName = "Appending to Stack"),
	AppendingNewSlot UMETA(displayName = "Appending New Slot")
};

USTRUCT(BlueprintType)
struct FInventoryOperationStruct
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Operation")
	EInventoryOperationType OperationType;
};

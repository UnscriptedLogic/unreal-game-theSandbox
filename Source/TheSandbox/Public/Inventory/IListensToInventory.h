#pragma once

#include "CoreMinimal.h"
#include "InventoryBase.h"
#include "UObject/Interface.h"
#include "InventoryStruct.h"
#include "InventoryOperationStruct.h"
#include "IListensToInventory.generated.h"

class UItemBase;

UINTERFACE(Blueprintable)
class THESANDBOX_API UListensToInventory : public UInterface
{
	GENERATED_BODY()
};

class THESANDBOX_API IListensToInventory
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Listener")
	void OnItemAdded(
		UItemBase* Item,
		UInventoryBase* InventoryContext,
		FInventoryOperationStruct InventoryOperation
	);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Listener")
	void OnItemRemoved(
		UItemBase* Item,
		UInventoryBase* InventoryContext,
		FInventoryOperationStruct InventoryOperation
	);
};

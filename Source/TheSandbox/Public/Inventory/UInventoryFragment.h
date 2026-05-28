#pragma once

#include "CoreMinimal.h"
#include "InventoryStruct.h"
#include "InventoryBase.h"
#include "InventoryOperationStruct.h"
#include "Response.h"
#include "UInventoryFragment.generated.h"

class UItemBase;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class THESANDBOX_API UInventoryFragment : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Fragment")
	FString ID;	
	
	UPROPERTY(BlueprintReadWrite, Category = "Inventory Fragment")
	UInventoryBase* inventoryBaseContext;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Fragment")
	void OnFragmentAttached(
		UInventoryBase* InventoryContext
	);

	virtual void OnFragmentAttached_Implementation(
		UInventoryBase* InventoryContext
	);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Fragment")
	void OnFragmentDettached(
		UInventoryBase* InventoryContext
	);

	virtual void OnFragmentDettached_Implementation(
		UInventoryBase* InventoryContext
	);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Fragment")
	FResponse Pass_NewItemToBeAdded(
		UItemBase* item,
		UInventoryBase* inventory,
		FInventoryOperationStruct inventoryOperation
	);

	virtual FResponse Pass_NewItemToBeAdded_Implementation(
		UItemBase* item,
		UInventoryBase* inventory,
		FInventoryOperationStruct inventoryOperation
	);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Inventory Fragment")
	FResponse Pass_ItemToBeRemoved(
		UItemBase* item,
		UInventoryBase* inventory,
		FInventoryOperationStruct inventoryOperation
	);

	virtual FResponse Pass_ItemToBeRemoved_Implementation(
		UItemBase* item,
		UInventoryBase* inventory,
		FInventoryOperationStruct inventoryOperation
	);
};

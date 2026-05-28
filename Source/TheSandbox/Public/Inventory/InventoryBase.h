#pragma once

#include "CoreMinimal.h"
#include "InventoryStruct.h"
#include "InventoryBase.generated.h"

class UItemBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItem_Delegate, UItemBase*, Item, UInventoryBase*, Inventory);

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class THESANDBOX_API UInventoryBase : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FInventoryStruct data;
	
	UPROPERTY(BlueprintReadWrite)
	AActor* actorOwner;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnItem_Delegate OnItemAdded;
	
	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnItem_Delegate OnItemRemoved;
};

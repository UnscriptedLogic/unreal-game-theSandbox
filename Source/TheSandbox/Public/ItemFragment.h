// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Response.h"
#include "ItemFragment.generated.h"

class UInventoryBase;
class UItemBase;

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class THESANDBOX_API UItemFragment : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Fragment")
	FString ID;
	
	UPROPERTY(BlueprintReadWrite, Category = "Item Fragment")
	UItemBase* itemContext;	
	
	UPROPERTY(BlueprintReadWrite, Category = "Item Fragment")
	UInventoryBase* inventoryContext;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Item Fragment")
	void Initialize(UItemBase* itemBase, UInventoryBase* inventoryBase);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item Fragment")
	FResponse Pass_ItemToBeRemovedFromInventory(UInventoryBase* inventoryBase, UItemBase* item);
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item Fragment")
	FResponse Pass_ItemToBeAddedToInventory(UInventoryBase* inventoryBase, bool HasExistingSlot, UItemBase* existingSlot, UItemBase*& overrideSlot);
};

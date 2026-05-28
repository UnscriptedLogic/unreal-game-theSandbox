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

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Item Fragment")
	FResponse Pass_ItemToBeAddedToInventory(UInventoryBase* InventoryContext, bool HasExistingSlot, UItemBase* existingSlot, UItemBase*& overrideSlot);
};

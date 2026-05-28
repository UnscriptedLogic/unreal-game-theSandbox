// Fill out your copyright notice in the Description page of Project Settings.

#include "Inventory/UInventoryFragment.h"

void UInventoryFragment::OnFragmentAttached_Implementation(
	UInventoryBase* InventoryContext
)
{
	this->inventoryBaseContext = InventoryContext;
}

void UInventoryFragment::OnFragmentDettached_Implementation(
	UInventoryBase* InventoryContext
)
{
	this->inventoryBaseContext = nullptr;
}

FResponse UInventoryFragment::Pass_NewItemToBeAdded_Implementation(
	UItemBase* item,
	UInventoryBase* inventory,
	FInventoryOperationStruct inventoryOperation
)
{
	FResponse Response;
	Response.Code = 200;
	Response.Message = TEXT("All Okay");

	return Response;
}

FResponse UInventoryFragment::Pass_ItemToBeRemoved_Implementation(
	UItemBase* item,
	UInventoryBase* inventory,
	FInventoryOperationStruct inventoryOperation
)
{
	FResponse Response;
	Response.Code = 200;
	Response.Message = TEXT("All Okay");

	return Response;
}

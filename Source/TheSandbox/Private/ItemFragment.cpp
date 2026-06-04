// Fill out your copyright notice in the Description page of Project Settings.

#include "ItemFragment.h"

void UItemFragment::Initialize_Implementation(UItemBase* itemBase, UInventoryBase* inventoryBase)
{
	this->itemContext = itemBase;
	this->inventoryContext = inventoryBase;
}

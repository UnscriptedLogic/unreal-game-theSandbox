// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

void UItemBase::SetQuantity(int32 NewQuantity)
{
	quantity = FMath::Max(0, NewQuantity);
}

void UItemBase::ModifyQuantity(int32 Amount)
{
	SetQuantity(quantity + Amount);
}
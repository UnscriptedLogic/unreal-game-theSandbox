// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "InventoryStruct.generated.h"

class UItemBase;
class UInventoryFragment;

USTRUCT(BlueprintType)
struct FInventoryStruct
{
	GENERATED_BODY();

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TArray<UItemBase*> items;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Inventory")
	TArray<TObjectPtr<UInventoryFragment>> fragments;
};

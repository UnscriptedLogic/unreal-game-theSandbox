// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemStruct.h"
#include "CoreMinimal.h"
#include "ItemBase.generated.h"

UCLASS(Blueprintable, EditInlineNew, DefaultToInstanced)
class THESANDBOX_API UItemBase : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FItemStruct ItemData;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite, Category = "Inventory")
	TArray<TObjectPtr<UItemFragment>> ItemFragments;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 quantity;

	UFUNCTION(BlueprintCallable, Category = "Item")
	void SetQuantity(int32 NewQuantity);

	UFUNCTION(BlueprintCallable, Category = "Item")
	void ModifyQuantity(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Item")
	bool HasFragment(TSubclassOf<UItemFragment> FragmentClass) const
	{
		if (!*FragmentClass)
		{
			return false;
		}

		for (const TObjectPtr<UItemFragment>& Fragment : ItemFragments)
		{
			if (IsValid(Fragment) && Fragment->IsA(FragmentClass))
			{
				return true;
			}
		}

		return false;
	}
};

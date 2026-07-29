#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "FurnitureAttributeSet.generated.h"

UCLASS()
class THESANDBOX_API UFurnitureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UFurnitureAttributeSet();

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Weight)
	FGameplayAttributeData Weight;
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(UFurnitureAttributeSet, Weight)
	GAMEPLAYATTRIBUTE_VALUE_GETTER(Weight)
	GAMEPLAYATTRIBUTE_VALUE_SETTER(Weight)
	GAMEPLAYATTRIBUTE_VALUE_INITTER(Weight)

	UFUNCTION()
	void OnRep_Weight(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UFurnitureAttributeSet, Weight, OldValue);
	}

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};

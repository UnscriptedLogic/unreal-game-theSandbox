#include "FurnitureAttributeSet.h"

#include "Net/UnrealNetwork.h"

UFurnitureAttributeSet::UFurnitureAttributeSet()
{
	Weight = 0.f;
}

void UFurnitureAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UFurnitureAttributeSet, Weight, COND_None, REPNOTIFY_Always);
}

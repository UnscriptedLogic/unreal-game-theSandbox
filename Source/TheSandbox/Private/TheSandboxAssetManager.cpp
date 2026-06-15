#include "TheSandboxAssetManager.h"

#include <iostream>
#include <ostream>

#include "AbilitySystemGlobals.h"

void UTheSandboxAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();

	UAbilitySystemGlobals::Get().InitGlobalData();
	std::cout << "Global Data Initialized" << std::endl;
}

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TheSandboxAssetManager.generated.h"

UCLASS()
class THESANDBOX_API UTheSandboxAssetManager : public UAssetManager
{
	GENERATED_BODY()

protected:
	virtual void StartInitialLoading() override;
};

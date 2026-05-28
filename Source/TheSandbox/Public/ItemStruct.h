#pragma once

#include "CoreMinimal.h"
#include "Engine/Texture2D.h"
#include "Engine/StaticMesh.h"
#include "ItemFragment.h"
#include "ItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FItemStruct
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> Mesh;
};

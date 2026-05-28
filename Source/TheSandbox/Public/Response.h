#pragma once

#include "CoreMinimal.h"
#include "Response.generated.h"

USTRUCT(BlueprintType)
struct FResponse
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response")
	int32 Code = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Response")
	FString Message = TEXT("OK");
};

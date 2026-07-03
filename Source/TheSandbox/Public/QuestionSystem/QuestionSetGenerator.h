#pragma once

#include "CoreMinimal.h"
#include "QuestionSystem/QuestionSystemTypes.h"
#include "UObject/Interface.h"
#include "QuestionSetGenerator.generated.h"

UINTERFACE(Blueprintable)
class THESANDBOX_API UQuestionSetGenerator : public UInterface
{
	GENERATED_BODY()
};

class THESANDBOX_API IQuestionSetGenerator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Question|Runtime")
	bool GenerateQuestions(const FQuestionSetGenerationOptions& Options, UObject* ContextObject, TArray<FQuestion>& OutQuestions, FName& OutStartQuestionId);
};

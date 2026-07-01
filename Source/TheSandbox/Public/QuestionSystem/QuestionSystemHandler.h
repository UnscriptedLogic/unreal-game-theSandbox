#pragma once

#include "CoreMinimal.h"
#include "QuestionSystem/QuestionSystemTypes.h"
#include "UObject/Interface.h"
#include "QuestionSystemHandler.generated.h"

UINTERFACE(Blueprintable)
class THESANDBOX_API UQuestionSystemHandler : public UInterface
{
	GENERATED_BODY()
};

class THESANDBOX_API IQuestionSystemHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Question")
	bool EvaluateQuestionCondition(const FQuestionCondition& Condition, UObject* ContextObject);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Question")
	void ApplyQuestionEffect(const FQuestionEffect& Effect, UObject* ContextObject, const FQuestionAnswerResult& AnswerResult);
};

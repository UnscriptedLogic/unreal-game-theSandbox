#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestionSystem/QuestionSystemTypes.h"
#include "QuestionSet.generated.h"

UCLASS(BlueprintType)
class THESANDBOX_API UQuestionSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Question")
	FName StartQuestionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Question")
	TArray<FQuestion> Questions;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	bool FindQuestion(FName QuestionId, FQuestion& OutQuestion) const;
};

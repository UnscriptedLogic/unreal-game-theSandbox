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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName StartQuestionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestion> Questions;

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime", meta = (WorldContext = "Outer"))
	static UQuestionSet* CreateRuntimeQuestionSet(UObject* Outer, const TArray<FQuestion>& InQuestions, FName InStartQuestionId = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime", meta = (WorldContext = "Outer"))
	static UQuestionSet* GenerateRandomQuestionSet(UObject* Outer, const UQuestionSet* SourceQuestionSet, const FQuestionSetGenerationOptions& Options);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime", meta = (WorldContext = "Outer"))
	static UQuestionSet* GenerateRuntimeQuestionSet(UObject* Outer, UObject* GeneratorObject, UObject* ContextObject, const FQuestionSetGenerationOptions& Options);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime")
	void SetQuestions(const TArray<FQuestion>& InQuestions, FName InStartQuestionId = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime")
	void AddQuestion(const FQuestion& Question, bool bSetAsStartQuestion = false);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime")
	void AppendQuestions(const TArray<FQuestion>& InQuestions);

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime")
	void LinkQuestionsAsSequence();

	UFUNCTION(BlueprintCallable, Category = "Question|Runtime")
	void ShuffleAnswerOptions(int32 RandomSeed = 0, bool bUseRandomSeed = true);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	bool FindQuestion(FName QuestionId, FQuestion& OutQuestion) const;

private:
	static void EnsureQuestionIds(TArray<FQuestion>& InOutQuestions);
	static void LinkQuestionsSequentially(TArray<FQuestion>& InOutQuestions);
	static void RandomizeAnswerOptions(TArray<FQuestion>& InOutQuestions, FRandomStream& RandomStream);
};

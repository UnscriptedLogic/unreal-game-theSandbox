#pragma once

#include "CoreMinimal.h"
#include "QuestionSystem/QuestionSet.h"
#include "QuestionSession.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionChanged, const FQuestion&, Question);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestionAnswered, const FQuestionAnswerResult&, Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestionSessionEnded);

UCLASS(BlueprintType)
class THESANDBOX_API UQuestionSession : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "Question")
	FOnQuestionChanged OnQuestionChanged;

	UPROPERTY(BlueprintAssignable, Category = "Question")
	FOnQuestionAnswered OnQuestionAnswered;

	UPROPERTY(BlueprintAssignable, Category = "Question")
	FOnQuestionSessionEnded OnQuestionSessionEnded;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	TObjectPtr<UQuestionSet> QuestionSet;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	TObjectPtr<UObject> ContextObject;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	TObjectPtr<UObject> HandlerObject;

	UFUNCTION(BlueprintCallable, Category = "Question")
	void Initialize(UQuestionSet* InQuestionSet, UObject* InContextObject, UObject* InHandlerObject);

	UFUNCTION(BlueprintCallable, Category = "Question")
	bool StartSession(FName StartQuestionId = NAME_None);

	UFUNCTION(BlueprintCallable, Category = "Question")
	void EndSession();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	bool IsSessionActive() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	FName GetCurrentQuestionId() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	bool GetCurrentQuestion(FQuestion& OutQuestion) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Question")
	TArray<FQuestionAnswerOption> GetAvailableAnswerOptions() const;

	UFUNCTION(BlueprintCallable, Category = "Question")
	bool ChooseAnswer(FName AnswerOptionId, FQuestionAnswerResult& OutResult);

	UFUNCTION(BlueprintCallable, Category = "Question")
	bool SubmitTextAnswer(const FString& TextAnswer, FQuestionAnswerResult& OutResult);

	UFUNCTION(BlueprintCallable, Category = "Question")
	bool SubmitNumberAnswer(float NumberAnswer, FQuestionAnswerResult& OutResult);

	UFUNCTION(BlueprintCallable, Category = "Question")
	bool SubmitAnswer(const FQuestionSubmittedAnswer& SubmittedAnswer, FQuestionAnswerResult& OutResult);

private:
	UPROPERTY()
	FName CurrentQuestionId = NAME_None;

	UPROPERTY()
	bool bSessionActive = false;

	bool CanUseQuestion(const FQuestion& Question) const;
	bool CanUseAnswerOption(const FQuestionAnswerOption& AnswerOption) const;
	bool EvaluateConditions(const TArray<FQuestionCondition>& Conditions) const;
	bool DoesRuleMatch(const FQuestionAnswerRule& Rule, const FQuestionSubmittedAnswer& SubmittedAnswer) const;
	FString GetGivenAnswerText(const FQuestion& Question, const FQuestionSubmittedAnswer& SubmittedAnswer) const;
	void ApplyEffects(const TArray<FQuestionEffect>& Effects, const FQuestionAnswerResult& Result) const;
	void AdvanceFromResult(const FQuestionAnswerResult& Result);
};

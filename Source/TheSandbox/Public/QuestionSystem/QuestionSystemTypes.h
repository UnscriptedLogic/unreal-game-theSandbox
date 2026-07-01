#pragma once

#include "CoreMinimal.h"
#include "QuestionSystemTypes.generated.h"

UENUM(BlueprintType)
enum class EQuestionAnswerKind : uint8
{
	Choice,
	Text,
	Number
};

UENUM(BlueprintType)
enum class EQuestionRuleMatchType : uint8
{
	Always,
	ExactText,
	CaseInsensitiveText,
	NumberEquals,
	NumberRange,
	NameEquals
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionEffect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName EffectType = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName Target = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float FloatValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString StringValue;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionCondition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName ConditionType = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName Target = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	int32 IntValue = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float FloatValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString StringValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	bool bInvertResult = false;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionAnswerOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName Id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FText Text;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName PayloadName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString PayloadString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float PayloadNumber = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionCondition> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName NextQuestionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	bool bEndSession = false;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionAnswerRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName Id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	EQuestionRuleMatchType MatchType = EQuestionRuleMatchType::Always;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString TextValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName NameValue = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float NumberValue = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float NumberMin = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float NumberMax = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question", meta = (ClampMin = "0.0"))
	float NumberTolerance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionEffect> Effects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName NextQuestionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	bool bEndSession = false;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestion
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName Id = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FText Prompt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	EQuestionAnswerKind AnswerKind = EQuestionAnswerKind::Choice;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionCondition> Conditions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionAnswerOption> AnswerOptions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionAnswerRule> AnswerRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	TArray<FQuestionEffect> DefaultEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName DefaultNextQuestionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	bool bEndSessionOnDefault = false;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionSubmittedAnswer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString GivenAnswer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName AnswerOptionId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FString TextAnswer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	float NumberAnswer = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Question")
	FName NameAnswer = NAME_None;
};

USTRUCT(BlueprintType)
struct THESANDBOX_API FQuestionAnswerResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	FName QuestionId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	FQuestionSubmittedAnswer SubmittedAnswer;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	FName MatchedRuleId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	FName ChosenAnswerOptionId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	TArray<FQuestionEffect> Effects;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	FName NextQuestionId = NAME_None;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	bool bMatched = false;

	UPROPERTY(BlueprintReadOnly, Category = "Question")
	bool bEndedSession = false;
};

#include "QuestionSystem/QuestionSet.h"

#include "QuestionSystem/QuestionSetGenerator.h"

UQuestionSet* UQuestionSet::CreateRuntimeQuestionSet(UObject* Outer, const TArray<FQuestion>& InQuestions, FName InStartQuestionId)
{
	UQuestionSet* QuestionSet = NewObject<UQuestionSet>(Outer ? Outer : GetTransientPackage());
	QuestionSet->SetQuestions(InQuestions, InStartQuestionId);
	return QuestionSet;
}

UQuestionSet* UQuestionSet::GenerateRandomQuestionSet(UObject* Outer, const UQuestionSet* SourceQuestionSet, const FQuestionSetGenerationOptions& Options)
{
	if (!SourceQuestionSet)
	{
		return nullptr;
	}

	TArray<int32> QuestionIndexes;
	QuestionIndexes.Reserve(SourceQuestionSet->Questions.Num());
	for (int32 QuestionIndex = 0; QuestionIndex < SourceQuestionSet->Questions.Num(); ++QuestionIndex)
	{
		QuestionIndexes.Add(QuestionIndex);
	}

	const int32 Seed = Options.bUseRandomSeed ? Options.RandomSeed : FMath::Rand();
	FRandomStream RandomStream(Seed);

	for (int32 Index = QuestionIndexes.Num() - 1; Index > 0; --Index)
	{
		const int32 SwapIndex = RandomStream.RandRange(0, Index);
		QuestionIndexes.Swap(Index, SwapIndex);
	}

	const int32 RequestedQuestionCount = Options.QuestionCount <= 0 ? QuestionIndexes.Num() : Options.QuestionCount;
	const int32 QuestionCount = FMath::Min(RequestedQuestionCount, QuestionIndexes.Num());

	TArray<FQuestion> GeneratedQuestions;
	GeneratedQuestions.Reserve(QuestionCount);
	for (int32 Index = 0; Index < QuestionCount; ++Index)
	{
		GeneratedQuestions.Add(SourceQuestionSet->Questions[QuestionIndexes[Index]]);
	}

	if (Options.bRandomizeAnswerOptions)
	{
		RandomizeAnswerOptions(GeneratedQuestions, RandomStream);
	}

	EnsureQuestionIds(GeneratedQuestions);

	if (Options.bLinkQuestionsSequentially)
	{
		LinkQuestionsSequentially(GeneratedQuestions);
	}

	const FName StartQuestionId = GeneratedQuestions.IsEmpty() ? NAME_None : GeneratedQuestions[0].Id;
	return CreateRuntimeQuestionSet(Outer, GeneratedQuestions, StartQuestionId);
}

UQuestionSet* UQuestionSet::GenerateRuntimeQuestionSet(UObject* Outer, UObject* GeneratorObject, UObject* ContextObject, const FQuestionSetGenerationOptions& Options)
{
	if (!GeneratorObject || !GeneratorObject->GetClass()->ImplementsInterface(UQuestionSetGenerator::StaticClass()))
	{
		return nullptr;
	}

	TArray<FQuestion> GeneratedQuestions;
	FName GeneratedStartQuestionId = NAME_None;
	if (!IQuestionSetGenerator::Execute_GenerateQuestions(GeneratorObject, Options, ContextObject, GeneratedQuestions, GeneratedStartQuestionId))
	{
		return nullptr;
	}

	const int32 Seed = Options.bUseRandomSeed ? Options.RandomSeed : FMath::Rand();
	FRandomStream RandomStream(Seed);

	if (Options.bRandomizeAnswerOptions)
	{
		RandomizeAnswerOptions(GeneratedQuestions, RandomStream);
	}

	EnsureQuestionIds(GeneratedQuestions);

	if (Options.bLinkQuestionsSequentially)
	{
		LinkQuestionsSequentially(GeneratedQuestions);
	}

	return CreateRuntimeQuestionSet(Outer, GeneratedQuestions, GeneratedStartQuestionId);
}

void UQuestionSet::SetQuestions(const TArray<FQuestion>& InQuestions, FName InStartQuestionId)
{
	Questions = InQuestions;
	EnsureQuestionIds(Questions);
	StartQuestionId = InStartQuestionId;

	if (StartQuestionId.IsNone() && !Questions.IsEmpty())
	{
		StartQuestionId = Questions[0].Id;
	}
}

void UQuestionSet::AddQuestion(const FQuestion& Question, bool bSetAsStartQuestion)
{
	Questions.Add(Question);
	EnsureQuestionIds(Questions);

	if (bSetAsStartQuestion || StartQuestionId.IsNone())
	{
		StartQuestionId = Questions.Last().Id;
	}
}

void UQuestionSet::AppendQuestions(const TArray<FQuestion>& InQuestions)
{
	const bool bShouldSetStartQuestion = StartQuestionId.IsNone() && !InQuestions.IsEmpty();
	Questions.Append(InQuestions);
	EnsureQuestionIds(Questions);

	if (bShouldSetStartQuestion)
	{
		StartQuestionId = Questions[Questions.Num() - InQuestions.Num()].Id;
	}
}

void UQuestionSet::LinkQuestionsAsSequence()
{
	LinkQuestionsSequentially(Questions);
}

void UQuestionSet::ShuffleAnswerOptions(int32 RandomSeed, bool bUseRandomSeed)
{
	const int32 Seed = bUseRandomSeed ? RandomSeed : FMath::Rand();
	FRandomStream RandomStream(Seed);
	RandomizeAnswerOptions(Questions, RandomStream);
}

bool UQuestionSet::FindQuestion(FName QuestionId, FQuestion& OutQuestion) const
{
	for (const FQuestion& Question : Questions)
	{
		if (Question.Id == QuestionId)
		{
			OutQuestion = Question;
			return true;
		}
	}

	return false;
}

void UQuestionSet::EnsureQuestionIds(TArray<FQuestion>& InOutQuestions)
{
	TSet<FName> UsedQuestionIds;
	for (int32 QuestionIndex = 0; QuestionIndex < InOutQuestions.Num(); ++QuestionIndex)
	{
		FQuestion& Question = InOutQuestions[QuestionIndex];
		if (!Question.Id.IsNone() && !UsedQuestionIds.Contains(Question.Id))
		{
			UsedQuestionIds.Add(Question.Id);
			continue;
		}

		FName GeneratedQuestionId;
		int32 AttemptIndex = QuestionIndex;
		do
		{
			GeneratedQuestionId = FName(*FString::Printf(TEXT("RuntimeQuestion_%d"), AttemptIndex));
			++AttemptIndex;
		}
		while (UsedQuestionIds.Contains(GeneratedQuestionId));

		Question.Id = GeneratedQuestionId;
		UsedQuestionIds.Add(Question.Id);
	}
}

void UQuestionSet::LinkQuestionsSequentially(TArray<FQuestion>& InOutQuestions)
{
	for (int32 QuestionIndex = 0; QuestionIndex < InOutQuestions.Num(); ++QuestionIndex)
	{
		FQuestion& Question = InOutQuestions[QuestionIndex];
		const bool bLastQuestion = QuestionIndex == InOutQuestions.Num() - 1;
		const FName NextQuestionId = bLastQuestion ? NAME_None : InOutQuestions[QuestionIndex + 1].Id;

		for (FQuestionAnswerOption& AnswerOption : Question.AnswerOptions)
		{
			AnswerOption.NextQuestionId = NextQuestionId;
			AnswerOption.bEndSession = bLastQuestion;
		}

		for (FQuestionAnswerRule& AnswerRule : Question.AnswerRules)
		{
			AnswerRule.NextQuestionId = NextQuestionId;
			AnswerRule.bEndSession = bLastQuestion;
		}

		Question.DefaultNextQuestionId = NextQuestionId;
		Question.bEndSessionOnDefault = bLastQuestion;
	}
}

void UQuestionSet::RandomizeAnswerOptions(TArray<FQuestion>& InOutQuestions, FRandomStream& RandomStream)
{
	for (FQuestion& Question : InOutQuestions)
	{
		for (int32 Index = Question.AnswerOptions.Num() - 1; Index > 0; --Index)
		{
			const int32 SwapIndex = RandomStream.RandRange(0, Index);
			Question.AnswerOptions.Swap(Index, SwapIndex);
		}
	}
}

#include "QuestionSystem/QuestionSession.h"

#include "QuestionSystem/QuestionSystemHandler.h"

void UQuestionSession::Initialize(UQuestionSet* InQuestionSet, UObject* InContextObject, UObject* InHandlerObject)
{
	QuestionSet = InQuestionSet;
	ContextObject = InContextObject;
	HandlerObject = InHandlerObject;
	CurrentQuestionId = NAME_None;
	bSessionActive = false;
}

bool UQuestionSession::StartSession(FName StartQuestionId)
{
	if (!QuestionSet)
	{
		return false;
	}

	const FName QuestionId = StartQuestionId.IsNone() ? QuestionSet->StartQuestionId : StartQuestionId;
	if (QuestionId.IsNone())
	{
		return false;
	}

	FQuestion Question;
	if (!QuestionSet->FindQuestion(QuestionId, Question) || !CanUseQuestion(Question))
	{
		return false;
	}

	CurrentQuestionId = QuestionId;
	bSessionActive = true;
	OnQuestionChanged.Broadcast(Question);
	return true;
}

void UQuestionSession::EndSession()
{
	if (!bSessionActive)
	{
		return;
	}
	
	bSessionActive = false;
	CurrentQuestionId = NAME_None;
	OnQuestionSessionEnded.Broadcast();
}

bool UQuestionSession::IsSessionActive() const
{
	return bSessionActive;
}

FName UQuestionSession::GetCurrentQuestionId() const
{
	return CurrentQuestionId;
}

bool UQuestionSession::GetCurrentQuestion(FQuestion& OutQuestion) const
{
	return bSessionActive && QuestionSet && QuestionSet->FindQuestion(CurrentQuestionId, OutQuestion);
}

TArray<FQuestionAnswerOption> UQuestionSession::GetAvailableAnswerOptions() const
{
	TArray<FQuestionAnswerOption> AvailableAnswers;

	FQuestion CurrentQuestion;
	if (!GetCurrentQuestion(CurrentQuestion))
	{
		return AvailableAnswers;
	}

	for (const FQuestionAnswerOption& AnswerOption : CurrentQuestion.AnswerOptions)
	{
		if (CanUseAnswerOption(AnswerOption))
		{
			AvailableAnswers.Add(AnswerOption);
		}
	}

	return AvailableAnswers;
}

bool UQuestionSession::ChooseAnswer(FName AnswerOptionId, FQuestionAnswerResult& OutResult)
{
	FQuestionSubmittedAnswer SubmittedAnswer;
	SubmittedAnswer.AnswerOptionId = AnswerOptionId;
	SubmittedAnswer.GivenAnswer = AnswerOptionId.ToString();
	return SubmitAnswer(SubmittedAnswer, OutResult);
}

bool UQuestionSession::SubmitTextAnswer(const FString& TextAnswer, FQuestionAnswerResult& OutResult)
{
	FQuestionSubmittedAnswer SubmittedAnswer;
	SubmittedAnswer.TextAnswer = TextAnswer;
	SubmittedAnswer.GivenAnswer = TextAnswer;
	return SubmitAnswer(SubmittedAnswer, OutResult);
}

bool UQuestionSession::SubmitNumberAnswer(float NumberAnswer, FQuestionAnswerResult& OutResult)
{
	FQuestionSubmittedAnswer SubmittedAnswer;
	SubmittedAnswer.NumberAnswer = NumberAnswer;
	SubmittedAnswer.GivenAnswer = LexToString(NumberAnswer);
	return SubmitAnswer(SubmittedAnswer, OutResult);
}

bool UQuestionSession::SubmitAnswer(const FQuestionSubmittedAnswer& SubmittedAnswer, FQuestionAnswerResult& OutResult)
{
	OutResult = FQuestionAnswerResult();

	FQuestion CurrentQuestion;
	if (!GetCurrentQuestion(CurrentQuestion))
	{
		return false;
	}

	OutResult.QuestionId = CurrentQuestion.Id;
	OutResult.SubmittedAnswer = SubmittedAnswer;
	OutResult.SubmittedAnswer.GivenAnswer = GetGivenAnswerText(CurrentQuestion, SubmittedAnswer);

	if (CurrentQuestion.AnswerKind == EQuestionAnswerKind::Choice)
	{
		for (const FQuestionAnswerOption& AnswerOption : GetAvailableAnswerOptions())
		{
			if (AnswerOption.Id == SubmittedAnswer.AnswerOptionId)
			{
				OutResult.ChosenAnswerOptionId = AnswerOption.Id;
				OutResult.Effects = AnswerOption.Effects;
				OutResult.NextQuestionId = AnswerOption.NextQuestionId;
				OutResult.bMatched = true;
				OutResult.bEndedSession = AnswerOption.bEndSession;
				break;
			}
		}

		if (!OutResult.bMatched)
		{
			return false;
		}
	}
	else
	{
		for (const FQuestionAnswerRule& Rule : CurrentQuestion.AnswerRules)
		{
			if (DoesRuleMatch(Rule, SubmittedAnswer))
			{
				OutResult.MatchedRuleId = Rule.Id;
				OutResult.Effects = Rule.Effects;
				OutResult.NextQuestionId = Rule.NextQuestionId;
				OutResult.bMatched = true;
				OutResult.bEndedSession = Rule.bEndSession;
				break;
			}
		}

		if (!OutResult.bMatched)
		{
			OutResult.Effects = CurrentQuestion.DefaultEffects;
			OutResult.NextQuestionId = CurrentQuestion.DefaultNextQuestionId;
			OutResult.bEndedSession = CurrentQuestion.bEndSessionOnDefault;
		}
	}

	ApplyEffects(OutResult.Effects, OutResult);
	OnQuestionAnswered.Broadcast(OutResult);
	AdvanceFromResult(OutResult);
	return true;
}

bool UQuestionSession::CanUseQuestion(const FQuestion& Question) const
{
	return EvaluateConditions(Question.Conditions);
}

bool UQuestionSession::CanUseAnswerOption(const FQuestionAnswerOption& AnswerOption) const
{
	return EvaluateConditions(AnswerOption.Conditions);
}

bool UQuestionSession::EvaluateConditions(const TArray<FQuestionCondition>& Conditions) const
{
	if (Conditions.IsEmpty())
	{
		return true;
	}

	if (!HandlerObject || !HandlerObject->GetClass()->ImplementsInterface(UQuestionSystemHandler::StaticClass()))
	{
		return false;
	}

	for (const FQuestionCondition& Condition : Conditions)
	{
		bool bPassed = IQuestionSystemHandler::Execute_EvaluateQuestionCondition(HandlerObject, Condition, ContextObject);
		if (Condition.bInvertResult)
		{
			bPassed = !bPassed;
		}

		if (!bPassed)
		{
			return false;
		}
	}

	return true;
}

bool UQuestionSession::DoesRuleMatch(const FQuestionAnswerRule& Rule, const FQuestionSubmittedAnswer& SubmittedAnswer) const
{
	switch (Rule.MatchType)
	{
	case EQuestionRuleMatchType::Always:
		return true;
	case EQuestionRuleMatchType::ExactText:
		return SubmittedAnswer.TextAnswer.Equals(Rule.TextValue, ESearchCase::CaseSensitive);
	case EQuestionRuleMatchType::CaseInsensitiveText:
		return SubmittedAnswer.TextAnswer.Equals(Rule.TextValue, ESearchCase::IgnoreCase);
	case EQuestionRuleMatchType::NumberEquals:
		return FMath::IsNearlyEqual(SubmittedAnswer.NumberAnswer, Rule.NumberValue, Rule.NumberTolerance);
	case EQuestionRuleMatchType::NumberRange:
		return SubmittedAnswer.NumberAnswer >= Rule.NumberMin && SubmittedAnswer.NumberAnswer <= Rule.NumberMax;
	case EQuestionRuleMatchType::NameEquals:
		return SubmittedAnswer.NameAnswer == Rule.NameValue;
	default:
		return false;
	}
}

FString UQuestionSession::GetGivenAnswerText(const FQuestion& Question, const FQuestionSubmittedAnswer& SubmittedAnswer) const
{
	if (!SubmittedAnswer.GivenAnswer.IsEmpty())
	{
		return SubmittedAnswer.GivenAnswer;
	}

	switch (Question.AnswerKind)
	{
	case EQuestionAnswerKind::Choice:
		return SubmittedAnswer.AnswerOptionId.ToString();
	case EQuestionAnswerKind::Text:
		return SubmittedAnswer.TextAnswer;
	case EQuestionAnswerKind::Number:
		return LexToString(SubmittedAnswer.NumberAnswer);
	default:
		return SubmittedAnswer.NameAnswer.ToString();
	}
}

void UQuestionSession::ApplyEffects(const TArray<FQuestionEffect>& Effects, const FQuestionAnswerResult& Result) const
{
	if (!HandlerObject || !HandlerObject->GetClass()->ImplementsInterface(UQuestionSystemHandler::StaticClass()))
	{
		return;
	}

	for (const FQuestionEffect& Effect : Effects)
	{
		IQuestionSystemHandler::Execute_ApplyQuestionEffect(HandlerObject, Effect, ContextObject, Result);
	}
}

void UQuestionSession::AdvanceFromResult(const FQuestionAnswerResult& Result)
{
	if (Result.bEndedSession || Result.NextQuestionId.IsNone())
	{
		EndSession();
		return;
	}

	FQuestion NextQuestion;
	if (!QuestionSet || !QuestionSet->FindQuestion(Result.NextQuestionId, NextQuestion) || !CanUseQuestion(NextQuestion))
	{
		EndSession();
		return;
	}

	CurrentQuestionId = Result.NextQuestionId;
	OnQuestionChanged.Broadcast(NextQuestion);
}

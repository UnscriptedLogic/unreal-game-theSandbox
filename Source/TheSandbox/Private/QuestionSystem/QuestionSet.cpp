#include "QuestionSystem/QuestionSet.h"

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

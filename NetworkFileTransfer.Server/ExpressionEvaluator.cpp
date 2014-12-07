#include "ExpressionEvaluator.h"

ExpressionEvaluator::ExpressionEvaluator()
{
}

ExpressionEvaluator::~ExpressionEvaluator()
{
}

char* ExpressionEvaluator::evaluate(const char* expression, const char* operation)
{
	char* result = new char[255];

	vector<string> tokens;
	vector<double> numbers;

	tokenize(expression, tokens, numbers);

	// if TRUE then we are working with numbers.
	if (tokens.size() == numbers.size())
	{
		evaluate(numbers, operation, result);
	}
	else
	{
		evaluate(tokens, operation, result);
	}

	return result;
}

void ExpressionEvaluator::tokenize(const char* exp, vector<string>& tokens, vector<double>& numbers)
{
	char token[255];
	token[0] = '\0';

	int tokenIndex = 0;
	int i = -1;

	do
	{
		++i;

		if (exp[i] == ' ' || exp[i] == '\0')
		{
			token[tokenIndex] = '\0';

			char* p;
			double number = strtod(token, &p);
			if (*p == '\0')
			{
				numbers.push_back(number);
			}

			string t(token);
			tokens.push_back(t);

			tokenIndex = 0;
			continue;
		}

		token[tokenIndex] = exp[i];
		++tokenIndex;

	} while (exp[i] != '\0');
}

void ExpressionEvaluator::evaluate(vector<string>& tokens, const char* operation, char* result)
{
	// Add
	if (isAddition(operation))
	{
		string r;

		for (int i = 0; i < tokens.size(); ++i)
		{
			r.append(tokens.at(i));
		}

		sprintf(result, "%s", r.c_str());
		return;
	}

	// Multiply
	if (isMultiplication(operation))
	{
		sprintf(result, "%s", "Can't perform multiplication for strings");
		return;
	}
}

void ExpressionEvaluator::evaluate(vector<double>& numbers, const char* operation, char* result)
{
	// Add
	if (isAddition(operation))
	{
		double r = 0;

		for (int i = 0; i < numbers.size(); ++i)
		{
			r += numbers.at(i);
		}

		sprintf(result, "%f", r);
		return;
	}

	// Multiply
	if (isMultiplication(operation))
	{
		double r = 1;

		for (int i = 0; i < numbers.size(); ++i)
		{
			r *= numbers.at(i);
		}

		sprintf(result, "%f", r);
		return;
	}
}

bool ExpressionEvaluator::isAddition(const char* operation)
{
	return strcmp(operation, "add") == 0;
}

bool ExpressionEvaluator::isMultiplication(const char* operation)
{
	return strcmp(operation, "mul") == 0;
}
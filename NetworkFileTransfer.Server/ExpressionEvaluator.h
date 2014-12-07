#pragma once
#include <vector>
#include <cstring>

using namespace std;

class ExpressionEvaluator
{
public:
	ExpressionEvaluator();
	~ExpressionEvaluator();
	char* evaluate(const char* expression, const char* operation);

private:
	void evaluate(vector<string>& tokens, const char* operation, char* result);
	void evaluate(vector<double>& numbers, const char* operation, char* result);
	void tokenize(const char* exp, vector<string>& tokens, vector<double>& numbers);
	bool isAddition(const char* operation);
	bool isMultiplication(const char* operation);
};
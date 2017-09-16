#pragma once
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stack>
#include <string>
#include <fstream>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <stdlib.h>
#include "jsonshot.h"
#include "utils/parser.h"

static bool is_number(char c)
{
	return (c >= '0' && c <= '9') || c == '.' || c == '-';
}

//return (low, high)
inline static string parse_deeper_vector(std::string cvar, VectorString v) {
	string returnVal = "9000";

	if (string_starts_with(cvar, "x"))
	{
		returnVal = to_string(get_safe_float(v.x));
	}
	else if (string_starts_with(cvar, "y"))
	{
		returnVal = to_string(get_safe_float(v.y));
	}
	else if (string_starts_with(cvar, "z"))
	{
		returnVal = to_string(get_safe_float(v.z));
	}
	return returnVal;
}
inline static string parse_deeper(std::string cvar, VectorString loc, VectorString vel) {
	string returnVal = "-9000";

	if (string_starts_with(cvar, "velocity"))
	{
		string newString = cvar.substr(9);
		returnVal = parse_deeper_vector(newString, vel);
	}
	else if (string_starts_with(cvar, "location"))
	{
		string newString = cvar.substr(9);
		returnVal = parse_deeper_vector(newString, loc);
	}
	return returnVal;
}

//ball|player_velocity|location_x|y|z
inline static string get_cvar_value(std::string cvar, player p, ball b, speed s) {
	std::transform(cvar.begin(), cvar.end(), cvar.begin(), ::tolower);
	string returnVal = "0";
	if (string_starts_with(cvar, "player")) {
		string newString = cvar.substr(7);
		returnVal = parse_deeper(newString, p.location, p.velocity);
	}
	else if (string_starts_with(cvar, "ball")) {
		string newString = cvar.substr(5);
		returnVal = parse_deeper(newString, b.location, b.velocity);
	}
	else if (string_starts_with(cvar, "speed")) {

	}
	return returnVal + "|";
}

// get weight of operators as per precedence
// higher weight given to operators with higher precedence
// for non operators, return 0 
inline static int getWeight(char ch) {
	switch (ch) {
	case '/':
	case '*': return 2;
	case '+':
	case '!': return 1;
	default: return 0;
	}
}

// convert infix expression to postfix using a stack
inline static string infix2postfix(std::string infix) {
	std::string postfix = "                                                                                                               ";
	int size = infix.size();
	stack<char> s;
	int weight;
	int i = 0;
	int k = 0;
	char ch;
	// iterate over the infix expression   
	while (i < size) {
		ch = infix[i];
		if (ch == '(') {
			// simply push the opening parenthesis
			s.push(ch);
			i++;
			continue;
		}
		if (ch == ')') {
			// if we see a closing parenthesis,
			// pop of all the elements and append it to
			// the postfix expression till we encounter
			// a opening parenthesis
			while (!s.empty() && s.top() != '(') {
				postfix[k++] = s.top();
				s.pop();

			}
			// pop off the opening parenthesis also
			if (!s.empty()) {
				s.pop();
			}
			i++;
			continue;
		}
		weight = getWeight(ch);
		if (weight == 0) {
			// we saw an operand
			// simply append it to postfix expression
			postfix[k++] = ch;

		}
		else {
			// we saw an operator
			if (s.empty()) {
				// simply push the operator onto stack if
				// stack is empty
				s.push(ch);
			}
			else {
				// pop of all the operators from the stack and
				// append it to the postfix expression till we
				// see an operator with a lower precedence that
				// the current operator
				while (!s.empty() && s.top() != '(' &&
					weight <= getWeight(s.top())) {
					postfix[k++] = s.top();
					s.pop();

				}
				// push the current operator onto stack
				s.push(ch);
			}
		}
		i++;
	}
	// pop of the remaining operators present in the stack
	// and append it to postfix expression 
	while (!s.empty()) {
		postfix[k++] = s.top();
		s.pop();
	}
	postfix[k] = 0; // null terminate the postfix expression 
	return postfix;
}

enum oper {
	MUL, DIV, ADD, SUB
};

inline static float calculate(oper op, float n1, float n2) {
	float res = 0;
	switch (op) {
	case MUL:
		res = n1 * n2;
		break;
	case DIV:
		res = n1 / n2;
		break;
	case ADD:
		res = n1 + n2;
		break;
	case SUB:
		res = n1 - n2;
		break;
	}
	return res;
}


inline static float eval(std::string expr) {
	expr = infix2postfix(expr);
	std::stack<float> stk;
	std::string currentExpr;
	while (expr.length() > 0) {
		char ch = expr[0];
		expr.erase(0, 1);
		if (ch == '+') {
			float n1 = stk.top();
			stk.pop();
			float n2 = stk.top();
			stk.pop();
			stk.push(calculate(ADD, n2, n1));
		}
		else if (ch == '!') {
			float n1 = stk.top();
			stk.pop();
			float n2 = stk.top();
			stk.pop();
			stk.push(calculate(SUB, n2, n1));
		}
		else if (ch == '*') {
			float n1 = stk.top();
			stk.pop();
			float n2 = stk.top();
			stk.pop();
			stk.push(calculate(MUL, n2, n1));
		}
		else if (ch == '/') {
			float n1 = stk.top();
			stk.pop();
			float n2 = stk.top();
			stk.pop();
			stk.push(calculate(DIV, n2, n1));
		}
		else if (ch == '|') {
			stk.push(stof(currentExpr));
			currentExpr.clear();
		}
		else {
			currentExpr += ch;
		}
	}
	return stk.top();
}

inline static string parse(std::string input, player p, ball b, speed s) {
	if (input.empty())
		return "0";
	std::string currentString;
	std::string lowNo = "";
	std::string highNo = "";
	std::string cvarName = "";
	std::string currentWrite = "";
	bool isLow = true;
	bool parsingCvar = false;
	bool parsingNumber = false;
	for (size_t i = 0; i<input.length(); i++) {
		char c = input[i];
		if (c == ' ')
		{

		}
		/*if (c == '(' && i == 0)
		{

		}
		else if (c == ')' && i == input.length() - 1) {

		}*/
		else if (c == ',') {
			if (parsingNumber) {
				currentWrite += "|";
				parsingNumber = false;
			}
			lowNo = currentWrite;
			currentWrite.clear();
		}
		else if (c == '{') {
			parsingCvar = true;
		}
		else if (c == '}') {
			parsingCvar = false;
			currentWrite += get_cvar_value(cvarName, p, b, s);

		}
		else
		{
			if (parsingCvar) {
				cvarName += c;
			}
			else {
				if (is_number(c)) {
					if (!parsingNumber) {
						parsingNumber = true;

					}
				}
				else {
					if (parsingNumber) {
						currentWrite += "|";
					}
					parsingNumber = false;
				}
				currentWrite += c;
			}
		}
	}
	if (parsingNumber) {
		currentWrite += "|";
		parsingNumber = false;
	}
	highNo = currentWrite;
	if (lowNo.empty())
		lowNo = highNo;
	return "(" + to_string(eval(lowNo)) + ", " + to_string(eval(highNo)) + ")";
}
#pragma once
#include "Stack.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
class TFormula {
	Stack<double> nums;
	Stack<string> operators;
	string Postfix;
	string Infix;
	vector<string> splitExpression;
	vector<string> postfix;
public:
	void getPostfix() {
		for (size_t i = 0; i < postfix.size(); i++) {
			cout << postfix[i];
		}
		cout << endl;
	}
	int getPrecedence(string op) {
		if (op == "(") return 1;
		if (op == ")") return 2;
		if (op == "+" || op == "-") {
			return 3;
		}
		else if (op == "*" || op == "/") {
			return 4;
		}
		else return 0;
	}
	TFormula() {
		cout << "Введите выражение в инфиксной форме" << endl;
		cin >> Infix;
		string tmp = "";
		// Создать словарь для хранения значений букв
		map<char, int> values;

		// Запросить значения букв у пользователя
		for (char c : Infix) {
			if (isalpha(c)) {
				cout << "Введите значение для буквы " << c << ": ";
				int value;
				cin >> value;
				values[c] = value;
			}
		}

		// Преобразовать выражение в цифры
		string result;
		for (char c : Infix) {
			if (isalpha(c)) {
				result += to_string(values[c]);
			}
			else {
				result += c;
			}
		}
		Infix = result;
		for (char c : Infix) {
			if (isdigit(c)) {
				tmp += c;
			}
			else {
				if(tmp!="")splitExpression.push_back(tmp);
				tmp = "";
				tmp += c;
				splitExpression.push_back(tmp);
				tmp = "";
			}
		}
		splitExpression.push_back(tmp);
	}
	TFormula(const TFormula& a) : Postfix(a.Postfix), Infix(a.Infix) {

	}
	bool FormulaChecker() {
		int index = 1;
		int errors = 0;
		for (char c : Infix) {
			if (c == '(') {
				nums.Push(index++);
			}
			if (c == ')') {
				if (nums.isEmpty()) {
					cout << 0 << ' ' << index++ << endl;
					errors++;
					continue;
				}
				cout << nums.Top() << ' ' << index++ << endl;
				nums.Pop();
			}
		}
		if (!nums.isEmpty()) {
			while (!nums.isEmpty()) {
				cout << nums.Top() << ' ' << 0 << endl;
				nums.Pop();
				errors++;
			}
		}
		cout << "errors: " << errors << endl;
		return errors == 0 ? true : false;
	}
	void FormulaConverter() {
		if (!FormulaChecker()) throw "incorrect Postfix";
		for (string& token : splitExpression) {
			if (getPrecedence(token) == 0) {
				postfix.push_back(token); continue;
			}
			if (getPrecedence(token) == 1) {
				operators.Push(token); continue;
			}
			if (operators.isEmpty()) {
				operators.Push(token); continue;
			}
			if (getPrecedence(token) > getPrecedence(operators.Top())) {
				operators.Push(token); continue;
			}
			if (getPrecedence(token) == 2) {
				while (getPrecedence(operators.Top()) != 1) {
					postfix.push_back(operators.Top());
					operators.Pop();
				}
				operators.Pop();
				continue;
			}
			else {
				while (getPrecedence(operators.Top()) >= getPrecedence(token)) {
					postfix.push_back(operators.Top());
					operators.Pop();
					if (operators.isEmpty()) break;
				}
				operators.Push(token);
				continue;
			}
		}
		while (!operators.isEmpty()) {
			postfix.push_back(operators.Top());
			operators.Pop();
		}
	}
	double FormulaCalculator() {
		double res = double();
		double tmp;
		for (string& token : postfix) {
			double op1 = double();
			double op2 = double();
			if (getPrecedence(token) == 0) {
				tmp = stoi(token);
				nums.Push(tmp);
				continue;
			}
			if (token == "+") {
				op2 = nums.Top();
				nums.Pop();
				op1 = nums.Top();
				nums.Pop();
				tmp = op1 + op2;
				nums.Push(tmp);
				continue;
			}
			if (token == "-") {
				op2 = nums.Top();
				nums.Pop();
				op1 = nums.Top();
				nums.Pop();
				tmp = op1 - op2;
				nums.Push(tmp);
				continue;
			}
			if (token == "*") {
				op2 = nums.Top();
				nums.Pop();
				op1 = nums.Top();
				nums.Pop();
				tmp = op1 * op2;
				nums.Push(tmp);
				continue;
			}
			if (token == "/") {
				op2 = nums.Top();
				nums.Pop();
				op1 = nums.Top();
				nums.Pop();
				tmp = op1 / op2;
				nums.Push(tmp);
				continue;
			}
		}
			res = nums.Top(); nums.Pop();
			return res;
	}
};
		
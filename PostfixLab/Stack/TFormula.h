#pragma once
#include "Stack.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
class TFormula {
	Stack<double> nums;
	Stack<char> operators;
	string Postfix;
	string Infix;
public:
	string getPostfix() {
		return Postfix;
	}
	TFormula() {
		cout << "¬ведите выражение в инфиксной форме" << endl;
		cin >> Infix;
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
					cout << 0 << ' ' << index++<<endl;
					errors++;
					continue;
				}
				cout << nums.Top() << ' ' << index++<<endl;
				nums.Pop();
			}
		}
		if (!nums.isEmpty()) {
			while (!nums.isEmpty()) {
				cout << nums.Top() << ' ' << 0<<endl;
				nums.Pop();
				errors++;
			}
		}
		cout << "errors: " << errors<<endl;
		return errors == 0 ? true : false;
	}
	string FormulaConverter() {
		if (!FormulaChecker()) throw "incorrect Postfix";
		map<char, int> myMap;
		myMap.emplace('(', 0);
		myMap.emplace(')', 1);
		myMap.emplace('+', 2);
		myMap.emplace('-', 2);
		myMap.emplace('*', 3);
		myMap.emplace('/', 3);
		for (char c : Infix) {
			auto it = myMap.find(c);
			if (it == myMap.end()) {
				Postfix += c; continue;
			}
			if (operators.isEmpty()) {
				operators.Push(c); continue;
			}
			if (myMap[c] == 1) {
				while (myMap[operators.Top()] != 0) {
					Postfix += operators.Top();
					operators.Pop();
				}
				operators.Pop(); continue;
			}
			if (myMap[c] == 0) {
				operators.Push(c); continue;
			}
			if (myMap[c] > myMap[operators.Top()]) {
				operators.Push(c); continue;
			}
			else {
					while (myMap[operators.Top()] >= myMap[c]) {
						Postfix += operators.Top();
						operators.Pop();
						if (operators.isEmpty()) break;
					}
				operators.Push(c); continue;
			}
		}
		while (!operators.isEmpty()) {
			Postfix += operators.Top();
			operators.Pop();
		}
		return Postfix;
	}
	double FormulaCalculator() {
		double res = double();
		for (char c : Postfix) {
			double op1 = double();
			double op2 = double();
			switch (c) {
			default:
				if ((int)c < 48 || (int)c>57) throw "its not a number";
				nums.Push(c - '0'); continue;
			case '+' :
				op2 = nums.Top(); nums.Pop();
				op1 = nums.Top(); nums.Pop();
				nums.Push(op1 + op2); continue;
			case '-':
				op2 = nums.Top(); nums.Pop();
				op1 = nums.Top(); nums.Pop();
				nums.Push(op1 - op2); continue;
			case '*':
				op2 = nums.Top(); nums.Pop();
				op1 = nums.Top(); nums.Pop();
				nums.Push(op1 * op2); continue;
			case '/':
				op2 = nums.Top(); nums.Pop();
				op1 = nums.Top(); nums.Pop();
				nums.Push(op1 / op2); continue;
			}
		}
		res = nums.Top(); nums.Pop();
		return res;
	}
};
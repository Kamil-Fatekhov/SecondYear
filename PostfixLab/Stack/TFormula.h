#pragma once
#include "TStack.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
class TFormula {
	Stack<double> nums;
	Stack<string> operators;
	string Postfix;
	string Infix;
	Stack<char> lexems;
	vector<string> splitExpression;
	vector<string> postfix;
public:
	void getSplit(){
		for (size_t i = 0; i < splitExpression.size(); i++) {
			cout << splitExpression[i]<<i<<endl;
		}
		cout << endl;
	}
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
		cout << "Input infix" << endl;
		getline(cin, Infix);
		string space;
		int count = 0;
		for(char c : Infix){
			if(c=='('){ count = 1;
			space+=c;
			continue;
			}
			else if(c!=')') count = 0;
			if (c == ')' && count == 1) throw "error";
			if(c!= ' '){
				space+=c;
				continue;
			}
		}
		Infix = space;
		//cout<<Infix;
		string tmp = "";
		// Ñîçäàòü ñëîâàðü äëÿ õðàíåíèÿ çíà÷åíèé áóêâ
		map<char, int> values;

		// Çàïðîñèòü çíà÷åíèÿ áóêâ ó ïîëüçîâàòåëÿ
		for (char c : Infix) {
			if (isalpha(c)) {
				cout << "input value of " << c << ": ";
				 float value;
				cin >> value;
				values[c] = value;
			}
		}
		// Ïðåîáðàçîâàòü âûðàæåíèå â öèôðû
		string result;
		for (char c : Infix) {
			if (isalpha(c)) {
				cout<<c;
				result += to_string(values[c]);
				cout<<result;
			}
			else {
				result += c;
			}
		}
		Infix = result;
		cout<<Infix;
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
		if(splitExpression.back()=="") splitExpression.pop_back();
	}
	TFormula(const TFormula& a) : Postfix(a.Postfix), Infix(a.Infix) {

	}
	bool FormulaChecker() {
		size_t count = 0;
		for(char c : Infix){
			if(c == '+' || c == '-' || c=='/'||c=='*'){
				if(count == 0){
				count++;
				cout<<count;
				continue;
				}
				else throw "incorrect infix";
			}
			else if(count!=0) count--;
		}
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
				else {cout << nums.Top() << ' ' << index++ << endl;
				nums.Pop();
				}
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

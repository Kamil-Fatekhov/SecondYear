#pragma once
#include "Stack.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unordered_map>
class TFormula {
	string marks;
	Stack<double> nums;
	Stack<string> operators;
	string Postfix;
	string Infix;
	Stack<char> lexems;
	vector<string> splitExpression;
	vector<string> postfix;
public:
	void getMarks() {
		if (!isCorrect()) {
			cout << Infix<<endl;
			cout << marks<<endl;
		}
		else {
			cout << Infix << endl;
		}
	}
	bool isBrace(string op) {
		if (op == "(" || op == ")") return true;
		return false;
	}
	bool isBrace(char op) {
		if (op == '(' || op == ')') return true;
		return false;
	}
	bool isOperator(char op) {
		if (op == '*' || op == '/' || op ==  '+' || op == '-') return true;
		return false;
	}
	bool isOperator(string op) {
		if (op == "*" || op == "/" || op == "+" || op == "-") return true;
		return false;
	}
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
		bool lastWasOperator = true;
		string space;

		int count = 0;
		int flag = 0;
		for(char c : Infix){
			if (!isOperator(c) && c != ' ' && flag == 0 || isdigit(c) || isBrace(c)) {
				flag = 1;
				space += c;
				continue;
			}
			else if(isOperator(c)){
				flag = 0;
				space += c;
				continue;
			}
			if(c!= ' ' && flag == 0){
				space+=c;
				count++;
				continue;
			}
			else if(c != ' ' && flag != 0 && !isdigit(c)) {
				space += ' ';
				space += c;
				continue;
			}
		}
		Infix = space;
		string tmp = "";

		/*for (char c : Infix) {
			if (isalpha(c)) {
				cout << "input value of " << c << ": ";
				 char value;
				cin >> value;
				if (!isdigit(value)) throw "its not a digit";
				values[c] = value;
			}
		}
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
		Infix = result;*/
		for (size_t i = 0; i < Infix.size(); i++) {
			char c = Infix[i];
			if (c == '(') {
				splitExpression.push_back(tmp);
				tmp = "";
				splitExpression.push_back("("); continue;
			}
			if (isdigit(c)) {
				tmp += c;
			}
			else {
				if (c == '-' ) {
					if (i == 0) {
						tmp = c; continue;
					}
					else {
						if (splitExpression[i - 1] == "(") tmp = c; continue;
					}
				}
				if (tmp != "")splitExpression.push_back(tmp);
				tmp = "";
				tmp += c;
				splitExpression.push_back(tmp);
				tmp = "";
			}
		}
		splitExpression.push_back(tmp);
		if (splitExpression.back() == "") splitExpression.pop_back();
		/*for (char c : Infix) {
			if (c == '(') int count = 1;
			if (isdigit(c)) {
				tmp += c;
			}
			else {
				if (c == '-' && count == 1) {
					tmp += c;
				}
				if(tmp!="")splitExpression.push_back(tmp);
				tmp = "";
				tmp += c;
				splitExpression.push_back(tmp);
				tmp = "";
			}
		}
		splitExpression.push_back(tmp);
		if(splitExpression.back()=="") splitExpression.pop_back();*/
	}
	TFormula(const TFormula& a) : Postfix(a.Postfix), Infix(a.Infix) {
		
	}
	bool FormulaCheckerBraces() {
		marks.resize(Infix.size(),' ');
		size_t count = 0;
		size_t negative = 0;
		Stack<int> ind;
		for (size_t i = 0; i < Infix.size(); i++) {
			if (Infix[i] == '(') {
				if (i == Infix.size() - 1) {
					marks[i] = '^'; break;
				}
				if (Infix[i + 1] == ')') marks[i] = '^';
				count++;
				ind.Push(i);
			}
			if (Infix[i] == ')') {
				if (count <= 0) {
					ind.Push(i);
					count--;
				}
				else {
					count--;
					ind.Pop();
				}
			}
		}
			if (!ind.isEmpty()) {
				while (!ind.isEmpty()) {
					marks[ind.Top()] = '^';
					ind.Pop();
				}
			}
		return count == 0 ? true : false;
	}
	bool isCorrect() {
		return (FormulaCheckerBraces() && FormulaCheckerOperators() && FormulaCheckerOperands());
	}
	bool FormulaCheckerOperators() {
		size_t flag = 0;
		for (size_t i = 0; i < Infix.size(); i++) {
			if (Infix[i] == ' ') {
				marks[i] = '^';
				flag = 1;
				continue; 
			}
			if (isOperator(Infix[i]) && isOperator(splitExpression[i]) && i==0) {
				marks[0] = '^'; flag = 1; continue;
			}
			if (isOperator(Infix[i])) {
				if (i == Infix.size() - 1) {
					marks[i] = '^'; flag = 1; continue;
				}
				else {
					if (isOperator(Infix[i + 1])) {
						marks[i + 1] = '^'; flag = 1; continue;
					}
				}
			}
		}
		return flag == 0 ? true : false;
	}
	bool FormulaCheckerOperands() {
			string tmp = "";
		size_t flag = 0;
		unordered_map<char, int> values;
		for (char c : Infix) {
			if (isalpha(c)) {
					char value;
				if (values.find(c) == values.end()) {
					cout << "input value of " << c << ": ";
					cin >> value;
					if (!isdigit(value)) throw "its not a digit";
					values[c] = value - '0';
				}
			}
		}
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
		splitExpression.resize(0);
		for (size_t i = 0; i < Infix.size(); i++) {
			char c = Infix[i];
			if (c == '(') {
				tmp = "";
				splitExpression.push_back("("); continue;
			}
			if (isdigit(c)) {
				tmp += c;
			}
			else {
				if (c == '-') {
					if (i == 0) {
						tmp = c; continue;
					}
					else {
						if (splitExpression[i - 1] == "(") tmp = c; continue;
					}
				}
				if (tmp != "")splitExpression.push_back(tmp);
				tmp = "";
				tmp += c;
				splitExpression.push_back(tmp);
				tmp = "";
			}
		}
		splitExpression.push_back(tmp);
		if (splitExpression.back() == "") splitExpression.pop_back();
		return flag == 0 ? true : false;
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
		if (!isCorrect()) throw "incorrect Postfix";
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

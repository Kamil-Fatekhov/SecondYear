#pragma once
#include "Stack.h"
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
using namespace std;

class TFormula {
    Stack<double> nums;
    Stack<string> operators;
    string Postfix;
    string Infix;
    vector<string> splitExpression;
    vector<string> postfix;

    // Функция для вывода строки с подчеркиванием ошибок
    void printError(const string& expression, const vector<size_t>& errorPositions) {
        cout << expression << endl;
        for (size_t i = 0; i < expression.length(); ++i) {
            if (find(errorPositions.begin(), errorPositions.end(), i) != errorPositions.end()) {
                cout << '^';
            }
            else {
                cout << ' ';
            }
        }
        cout << endl;
    }

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
        cout << "Enter the expression in infix form: " << endl;
        cin >> Infix;
        string tmp = "";
        // Создать словарь для хранения значений букв
        map<char, int> values;

        // Запросить значения букв у пользователя
        for (char c : Infix) {
            if (isalpha(c)) {
                cout << "Enter value for letter " << c << ": ";
                string valueStr;
                cin >> valueStr;

                // Проверка на отрицательное значение
                try {
                    int value = stoi(valueStr); // Преобразуем строку в число
                    values[c] = value;
                }
                catch (const invalid_argument& e) {
                    cerr << "Invalid input for " << c << ". Please enter a valid integer." << endl;
                    values[c] = 0; // По умолчанию 0, если ввод некорректен
                }
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
            if (isdigit(c) || c == '-') { // Разрешаем минус в числах
                tmp += c;
            }
            else {
                if (tmp != "") splitExpression.push_back(tmp);
                tmp = "";
                tmp += c;
                splitExpression.push_back(tmp);
                tmp = "";
            }
        }
        splitExpression.push_back(tmp);
    }

    TFormula(const TFormula& a) : Postfix(a.Postfix), Infix(a.Infix) {}

    bool FormulaChecker() {
        vector<size_t> errorPositions;
        Stack<size_t> stack;
        for (size_t i = 0; i < Infix.length(); ++i) {
            if (Infix[i] == '(') {
                stack.Push(i);
            }
            else if (Infix[i] == ')') {
                if (stack.isEmpty()) {
                    errorPositions.push_back(i); // Отмечаем позицию ошибки
                }
                else {
                    stack.Pop();
                }
            }
        }
        while (!stack.isEmpty()) {
            errorPositions.push_back(stack.Pop()); // Отмечаем позиции оставшихся '('
        }

        if (!errorPositions.empty()) {
            cout << "Error in expression: " << endl;
            printError(Infix, errorPositions);
            return false;
        }
        return true;
    }

    void FormulaConverter() {
        if (!FormulaChecker()) throw "Incorrect expression";
        for (string& token : splitExpression) {
            if (getPrecedence(token) == 0) {
                postfix.push_back(token);
                continue;
            }
            if (getPrecedence(token) == 1) {
                operators.Push(token);
                continue;
            }
            if (operators.isEmpty()) {
                operators.Push(token);
                continue;
            }
            if (getPrecedence(token) > getPrecedence(operators.Top())) {
                operators.Push(token);
                continue;
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
                tmp = stod(token); // Используем stod для поддержки отрицательных чисел
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
                if (op2 == 0) throw "Division by zero";
                tmp = op1 / op2;
                nums.Push(tmp);
                continue;
            }
        }
        res = nums.Top();
        nums.Pop();
        return res;
    }
};

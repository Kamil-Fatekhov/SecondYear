#include <iostream>
#include <string>
#include <cctype>
#include <sstream>
#include <unordered_map>
#include <algorithm> // Для std::unique
#include <cmath> //Для pow()
#include <vector>

using namespace std;

// Реализация стека на основе массива фиксированного размера
template <typename T>
class MyStack {
private:
    T* data; // Указатель на массив
    int capacity; // Максимальный размер стека
    int topIndex; // Индекс верхнего элемента ( -1, если стек пуст)

public:
    // Конструктор
    MyStack(int size) : capacity(size), topIndex(-1) {
        data = new T[capacity]; // Выделяем память под массив
    }

    // Деструктор (важно освободить память!)
    ~MyStack() {
        delete[] data; // Освобождаем выделенную память
    }

    void push(T value) {
        if (isFull()) {
            throw runtime_error("Stack overflow");
        }
        topIndex++;
        data[topIndex] = value;
    }

    T pop() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        T topValue = data[topIndex];
        topIndex--;
        return topValue;
    }

    T top() {
        if (isEmpty()) {
            throw runtime_error("Stack is empty");
        }
        return data[topIndex];
    }

    bool isEmpty() {
        return topIndex == -1;
    }

    bool isFull() {
        return topIndex == capacity - 1;
    }

    int size() {
        return topIndex + 1; // Возвращает количество элементов в стеке
    }
};

// Функция для определения приоритета оператора
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3; //Оператор возведения в степень
    return 0; // Для скобок и других символов
}

// Функция для проверки корректности выражения
bool isValidExpression(const string& expression, string& errorHighlights, const unordered_map<char, double>& variables) {
    bool isValid = true; // Изначально считаем, что выражение валидно
    errorHighlights = string(expression.length(), ' ');

    MyStack<char> parentheses(expression.length());  // Максимальный размер стека для скобок

    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (c == '(') {
            try {
                parentheses.push(c);
            }
            catch (const runtime_error& e) {
                cout << "Ошибка: Слишком много открывающих скобок." << endl;
                isValid = false;
                break;
            }
        }
        else if (c == ')') {
            if (parentheses.isEmpty()) {
                errorHighlights[i] = '^';
                isValid = false; // Лишняя закрывающая скобка
            }
            else {
                try {
                    parentheses.pop();
                }
                catch (const runtime_error& e) {
                    // This should never happen, but we keep it for safety
                    cout << "Ошибка: Что-то пошло не так со стеком скобок." << endl;
                    isValid = false;
                    break;
                }
            }
        }
        else if (!isdigit(c) && !isalpha(c) && c != '+' && c != '-' && c != '*' && c != '/' && c != '^' && c != ' ' && c != '(' && c != ')') { //Добавили ^
            errorHighlights[i] = '^';
            isValid = false; // Недопустимый символ
        }
        else if ((c == '+' || c == '*' || c == '/' || c == '^') && (i == 0 || i == expression.length() - 1)) { //Добавили ^
            errorHighlights[i] = '^';
            isValid = false; // Оператор в начале или конце выражения
        }
        else if ((c == '+' || c == '*' || c == '/' || c == '^') && (i > 0 && (expression[i - 1] == '+' || expression[i - 1] == '-' || expression[i - 1] == '*' || expression[i - 1] == '/' || expression[i - 1] == '^'))) { //Добавили ^
            errorHighlights[i] = '^';
            isValid = false; // Два оператора подряд
        }
        else if (isalpha(c)) {
            if (variables.find(c) == variables.end()) {
                errorHighlights[i] = '^';
                isValid = false; // Переменная не определена
            }
            if (i > 0 && (isdigit(expression[i - 1]) || isalpha(expression[i - 1]))) {
                errorHighlights[i] = '^';
                isValid = false; // Буквы или цифры рядом без знака
            }
        }
        else if (isdigit(c) && i > 0 && isalpha(expression[i - 1])) {
            errorHighlights[i] = '^';
            isValid = false; // Буквы или цифры рядом без знака
        }
    }

    if (!parentheses.isEmpty()) {
        isValid = false;
        //Отмечаем все незакрытые скобки.
        for (int i = 0; i < expression.length(); i++)
        {
            if (expression[i] == '(')
            {
                errorHighlights[i] = '^';
            }
        }
    }

    return isValid; // Возвращаем true, только если ошибок не было
}

// Функция для вычисления выражения и получения RPN
double evaluateExpression(const string& expression, const unordered_map<char, double>& variables, vector<string>& rpnOutput) {
    MyStack<double> numbers(expression.length()); //Стек чисел.
    MyStack<char> operators(expression.length()); //Стек операторов.
    string token;
    bool lastWasOperator = true; // Флаг для отслеживания унарного минуса

    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];

        if (isdigit(c) || (c == '-' && lastWasOperator && (token.empty() || !isalpha(expression[i - 1])))) {
            // Если это цифра, '-' (для отрицательного числа) или буква (переменная)
            token += c;
            lastWasOperator = false; // Теперь ждем не оператор
        }
        else {
            if (!token.empty()) {
                // Если токен накопился, преобразуем его в число и помещаем в стек чисел
                double numValue;
                if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
                    numValue = stod(token); // Преобразуем в double
                }
                else {
                    numValue = variables.at(token[0]); // Получаем значение переменной
                }
                numbers.push(numValue);
                rpnOutput.push_back(token); //Добавляем число/переменную в RPN
                token = "";
            }
            if (c == ' ') continue; // Пропускаем пробелы

            if (c == '(') {
                operators.push(c); // Открывающую скобку помещаем в стек операторов
                lastWasOperator = true; // После скобки ждем операнд
            }
            else if (c == ')') {
                // Закрывающая скобка: выталкиваем операторы из стека до открывающей скобки
                while (!operators.isEmpty() && operators.top() != '(') {
                    char op = operators.pop();
                    double operand2 = numbers.pop();
                    double operand1 = numbers.pop();

                    rpnOutput.push_back(string(1, op)); //Добавляем оператор в RPN

                    double result = 0;
                    switch (op) {
                    case '+': result = operand1 + operand2; break;
                    case '-': result = operand1 - operand2; break;
                    case '*': result = operand1 * operand2; break;
                    case '/':
                        if (operand2 == 0) {
                            cout << "Division by zero." << endl;
                            return 0; // Возвращаем 0 в случае ошибки
                        }
                        result = operand1 / operand2;
                        break;
                    case '^': result = pow(operand1, operand2); break; //Возведение в степень
                    }
                    numbers.push(result);

                }
                if (!operators.isEmpty())
                {
                    operators.pop(); // Удаляем открывающую скобку
                }
                else {
                    cout << "Ошибка в выражении: Лишняя закрывающая скобка." << endl;
                    return 0;

                }
                lastWasOperator = false;
            }
            else {
                // Обрабатываем операторы
                while (!operators.isEmpty() && precedence(c) <= precedence(operators.top())) {
                    char op = operators.pop();
                    double operand2 = numbers.pop();
                    double operand1 = numbers.pop();

                    rpnOutput.push_back(string(1, op)); //Добавляем оператор в RPN

                    double result = 0;
                    switch (op) {
                    case '+': result = operand1 + operand2; break;
                    case '-': result = operand1 - operand2; break;
                    case '*': result = operand1 * operand2; break;
                    case '/':
                        if (operand2 == 0) {
                            cout << "Ошибка: Деление на ноль." << endl;
                            return 0; // Возвращаем 0 в случае ошибки
                        }
                        result = operand1 / operand2;
                        break;
                    case '^': result = pow(operand1, operand2); break; //Возведение в степень
                    }
                    numbers.push(result);
                }
                operators.push(c); // Помещаем текущий оператор в стек
                lastWasOperator = true; // После оператора ждем операнд
            }
        }
    }

    if (!token.empty()) {
        // Если после обработки выражения остался токен, преобразуем его в число
        double numValue;
        if (isdigit(token[0]) || (token[0] == '-' && token.length() > 1 && isdigit(token[1]))) {
            numValue = stod(token);
        }
        else {
            numValue = variables.at(token[0]);
        }
        numbers.push(numValue);
        rpnOutput.push_back(token); //Добавляем число/переменную в RPN
    }

    // Выталкиваем все оставшиеся операторы из стека
    while (!operators.isEmpty()) {
        char op = operators.pop();
        double operand2 = numbers.pop();
        double operand1 = numbers.pop();

        rpnOutput.push_back(string(1, op)); //Добавляем оператор в RPN

        double result = 0;
        switch (op) {
        case '+': result = operand1 + operand2; break;
        case '-': result = operand1 - operand2; break;
        case '*': result = operand1 * operand2; break;
        case '/':
            if (operand2 == 0) {
                cout << "Ошибка: Деление на ноль." << endl;
                return 0; // Возвращаем 0 в случае ошибки
            }
            result = operand1 / operand2;
            break;
        case '^': result = pow(operand1, operand2); break; //Возведение в степень
        }
        numbers.push(result);
    }

    if (numbers.size() != 1)
    {
        cout << "Ошибка в выражении: Неправильное количество операндов." << endl;
        return 0;
    }

    return numbers.top(); // Результат вычисления находится на вершине стека
}

int main() {
    string expression;
    string errorHighlights;
    unordered_map<char, double> variables;

    cout << "Enter expression: ";
    getline(cin, expression);

    // Находим уникальные переменные
    string uniqueVars;
    for (char c : expression) {
        if (isalpha(c)) {
            uniqueVars += c;
        }
    }
    sort(uniqueVars.begin(), uniqueVars.end());
    uniqueVars.erase(unique(uniqueVars.begin(), uniqueVars.end()), uniqueVars.end());

    // Запрашиваем значения переменных
    for (char c : uniqueVars) {
        double value;
        cout << "Enter " << c << ": ";
        cin >> value;
        variables[c] = value;
        cin.ignore(); // Очистить буфер после ввода числа
    }

    bool isValid = isValidExpression(expression, errorHighlights, variables);

    if (isValid) {
        vector<string> rpnOutput;
        try {
            double result = evaluateExpression(expression, variables, rpnOutput);

            cout << "Postfix: ";
            for (const string& token : rpnOutput) {
                cout << token << " ";
            }
            cout << endl;
            cout << "Result: " << result << endl;
        }
        catch (const std::exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }
    else {
        cout << "Error:" << endl;
        cout << expression << endl;
        cout << errorHighlights << endl;
    }

    return 0;
}

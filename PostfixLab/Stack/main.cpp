#include "TFormula.h";
int main() {
    setlocale(LC_ALL, "ru");
    Stack<int> myStack;

    myStack.Push(10);
    myStack.Push(20);
    myStack.Push(30);

    std::cout << "Top element: " << myStack.Top() << std::endl;

    myStack.Pop();

    std::cout << "Top element: " << myStack.Top() << std::endl;

    std::cout << "Stack size: " << myStack.size() << std::endl;

    TFormula a;
    //a.FormulaChecker();
    a.FormulaConverter();
    cout << a.getPostfix() << endl;
    cout<<a.FormulaCalculator();
    return 0;
}
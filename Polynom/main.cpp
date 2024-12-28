#include <iostream>
#include <vector>
#include "Monom.h"

int main() {
    // Пример использования Manom
    cout << "Demonstration of Manom class:\n";

    // Создание мономов
    int powersA[] = { 2, 3 };
    Manom monomA(2.0, 2, powersA);
    cout << "Monom A = ";
    monomA.print();

    int powersB[] = { 1, 2 };
    Manom monomB(3.0, 2, powersB);
    cout << endl << "Monom B = ";
    monomB.print();

    int powersC[] = { 2, 3 };
    Manom monomC(1.0, 2, powersC);
    cout << endl << "Monom C = ";
    monomC.print();

    int powersD[] = { 1, 2 };
    Manom monomD(2.0, 2, powersD);
    cout << endl << "Monom D = ";
    monomD.print();


    // Операции с мономами
    try {
        Manom sumMonom = monomA + monomC;
        cout << endl << "A + C = ";
        sumMonom.print();
        Manom subMonom = monomA - monomC;
        cout << endl << "A - C = ";
        subMonom.print();
    }
    catch (const char* ex) {
        cout << ex << endl;
    }


    try {
        Manom sumMonom = monomA + monomB;
        cout << "A + B = ";
        sumMonom.print();
    }
    catch (const char* ex) {
        cout << "A + B = ";
        cout << ex << endl;
    }

    Manom productMonom = monomA * monomB;
    cout << endl << "A * B = ";
    productMonom.print();
    Manom divisionMonom = monomA / monomB;
    cout << endl << "A / B = ";
    divisionMonom.print();

    // Вычисление значения монома
    std::vector<double> values = { 2.0, 3.0 };
    double resultA = monomA.evaluate(values);
    cout << endl << "A(2,3) = " << resultA << endl;

    // Пример использования inputFromConsole
    cout << "\nDemonstration of Polynom class:\n";
    Polynom p3 = Polynom::inputFromConsole();
    cout << "You entered: ";
    p3.print();

    // Остальной код для проверки
    int powers1[] = { 2, 3 };
    int powers2[] = { 1, 2 };
    int powers3[] = { 2, 3 };
    int powers4[] = { 4, 1 };
    Manom m1(2.0, 2, powers1);
    Manom m2(3.0, 2, powers2);
    Manom m3(1.0, 2, powers3);
    Manom m4(4.0, 2, powers4);

    // Создание полиномов
    Polynom p1;
    p1.insertEnd(m1);
    p1.insertEnd(m2);

    Polynom p2;
    p2.insertEnd(m3);
    p2.insertEnd(m4);

    cout << "p1 = ";
    p1.print();
    cout << "p2 = ";
    p2.print();

    // Сложение полиномов
    Polynom sum = p1 + p2;
    cout << "p1 + p2 = ";
    sum.print();

    // Вычитание полиномов
    Polynom diff = p1 - p2;
    cout << "p1 - p2 = ";
    diff.print();

    // Умножение полиномов
    Polynom product = p1 * p2;
    cout << "p1 * p2 = ";
    product.print();

    // Копирование полинома
    Polynom p4 = p1;
    cout << "p4 = ";
    p4.print();

    // Вычисление значения полинома
    double result = p1.evaluate(values);
    cout << "p1(2,3) = " << result << endl;

    std::vector<double> values2 = { 2.0, 3.0 };
    double result2 = p2.evaluate(values2);
    cout << "p2(2,3) = " << result2 << endl;

    // Удаление полинома 
    p1 = Polynom();
    cout << "p1 after delete = ";
    p1.print();

    return 0;
}
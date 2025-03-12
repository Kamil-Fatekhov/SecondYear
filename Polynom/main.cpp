#include <iostream>
#include <string>
#include <cmath>

// Узел односвязного списка
template <typename T>
class Node {
public:
    T data;
    Node* next;

    Node(T data) : data(data), next(nullptr) {}
};

// Моном
class Monom {
public:
    double coefficient; // Коэффициент
    int x_power;        // Степень переменной x
    int y_power;        // Степень переменной y
    int z_power;        // Степень переменной z

    Monom(double coeff = 0.0, int x = 0, int y = 0, int z = 0)
        : coefficient(coeff), x_power(x), y_power(y), z_power(z) {}

    // Умножение мономов
    Monom operator*(const Monom& other) const {
        return Monom(
            coefficient * other.coefficient,
            x_power + other.x_power,
            y_power + other.y_power,
            z_power + other.z_power
        );
    }

    // Деление мономов
    Monom operator/(const Monom& other) const {
        return Monom(
            coefficient / other.coefficient,
            x_power - other.x_power,
            y_power - other.y_power,
            z_power - other.z_power
        );
    }

    // Возведение монома в степень
    Monom pow(int power) const {
        return Monom(
            std::pow(coefficient, power),
            x_power * power,
            y_power * power,
            z_power * power
        );
    }

    // Проверка на равенство мономов (для упрощения полиномов)
    bool operator==(const Monom& other) const {
        return x_power == other.x_power &&
               y_power == other.y_power &&
               z_power == other.z_power;
    }

    // Вывод монома
    void print() const {
        std::cout << coefficient;
        if (x_power > 0) std::cout << "x^" << x_power;
        if (y_power > 0) std::cout << "y^" << y_power;
        if (z_power > 0) std::cout << "z^" << z_power;
    }
};

// Полином
class Polynomial {
private:
    Node<Monom>* head;

    // Упрощение полинома (сложение одинаковых мономов)
    void simplify() {
        Node<Monom>* current = head;
        while (current) {
            Node<Monom>* runner = current->next;
            Node<Monom>* prev = current;
            while (runner) {
                if (current->data == runner->data) {
                    current->data.coefficient += runner->data.coefficient;
                    prev->next = runner->next;
                    delete runner;
                    runner = prev->next;
                } else {
                    prev = runner;
                    runner = runner->next;
                }
            }
            current = current->next;
        }
    }

public:
    Polynomial() : head(nullptr) {}

    ~Polynomial() {
        while (head) {
            Node<Monom>* temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Добавление монома в полином
    void addMonom(const Monom& monom) {
        Node<Monom>* newNode = new Node<Monom>(monom);
        if (!head) {
            head = newNode;
        } else {
            Node<Monom>* current = head;
            while (current->next) {
                current = current->next;
            }
            current->next = newNode;
        }
        simplify(); // Упрощаем полином после добавления
    }

    // Сложение полиномов
    Polynomial operator+(const Polynomial& other) const {
        Polynomial result;
        Node<Monom>* current = head;
        while (current) {
            result.addMonom(current->data);
            current = current->next;
        }
        current = other.head;
        while (current) {
            result.addMonom(current->data);
            current = current->next;
        }
        return result;
    }

    // Вычитание полиномов
    Polynomial operator-(const Polynomial& other) const {
        Polynomial result;
        Node<Monom>* current = head;
        while (current) {
            result.addMonom(current->data);
            current = current->next;
        }
        current = other.head;
        while (current) {
            Monom temp = current->data;
            temp.coefficient *= -1;
            result.addMonom(temp);
            current = current->next;
        }
        return result;
    }

    // Умножение полиномов
    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        Node<Monom>* current1 = head;
        while (current1) {
            Node<Monom>* current2 = other.head;
            while (current2) {
                result.addMonom(current1->data * current2->data);
                current2 = current2->next;
            }
            current1 = current1->next;
        }
        return result;
    }

    // Деление полиномов (упрощенное, без остатка)
    Polynomial operator/(const Polynomial& other) const {
        Polynomial result;
        Node<Monom>* current1 = head;
        while (current1) {
            Node<Monom>* current2 = other.head;
            while (current2) {
                result.addMonom(current1->data / current2->data);
                current2 = current2->next;
            }
            current1 = current1->next;
        }
        return result;
    }

    // Возведение полинома в степень
    Polynomial pow(int power) const {
        Polynomial result;
        Node<Monom>* current = head;
        while (current) {
            result.addMonom(current->data.pow(power));
            current = current->next;
        }
        return result;
    }

    // Вывод полинома
    void print() const {
        Node<Monom>* current = head;
        while (current) {
            current->data.print();
            if (current->next) {
                std::cout << " + ";
            }
            current = current->next;
        }
        std::cout << std::endl;
    }
};

int main() {
    Monom m1(5, 2, 3, 1); // 5x^2y^3z^1
    Monom m2(3, 1, 2, 0);  // 3x^1y^2

    Polynomial p1;
    p1.addMonom(m1);
    p1.addMonom(m2);

    Polynomial p2;
    p2.addMonom(m1);

    std::cout << "Polynomial 1: ";
    p1.print();
    std::cout << "Polynomial 2: ";
    p2.print();

    Polynomial p3 = p1 + p2;
    std::cout << "p1 + p2: ";
    p3.print();

    Polynomial p4 = p1 - p2;
    std::cout << "p1 - p2: ";
    p4.print();

    Polynomial p5 = p1 * p2;
    std::cout << "p1 * p2: ";
    p5.print();

    Polynomial p6 = p1 / p2;
    std::cout << "p1 / p2: ";
    p6.print();

    Polynomial p7 = p1.pow(2);
    std::cout << "p1^2: ";
    p7.print();

    return 0;
}

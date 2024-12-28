
#pragma once
#include <iostream>
#include <vector>
#include <sstream>
#include "List.h"

using namespace std;

class Manom {
private:
    double _coef;
    size_t _size;
    int* _powers;

public:
    Manom* Next = nullptr;

    Manom() : _coef(0), _size(1), _powers(new int[_size]) {
    }

    Manom(double coef, int count, int* powers) {
        _coef = coef;
        _size = count;
        _powers = new int[_size];
        for (int i = 0; i < _size; i++) {
            _powers[i] = powers[i];
        }
    }

    Manom(const Manom& tmp) {
        _coef = tmp._coef;
        _size = tmp._size;
        _powers = new int[_size];
        for (int i = 0; i < _size; i++) {
            _powers[i] = tmp._powers[i];
        }
    }

    ~Manom() {
        delete[] _powers;
    }

    Manom& operator=(const Manom& tmp) {
        if (&tmp == this) return *this;
        _coef = tmp._coef;
        _size = tmp._size;
        delete[] _powers;
        _powers = new int[_size];
        for (int i = 0; i < _size; i++) {
            _powers[i] = tmp._powers[i];
        }
        return *this;
    }

    Manom operator+(const Manom& tmp) const {
        if (_size != tmp._size) throw "Not equal size";
        for (int i = 0; i < _size; i++) {
            if (_powers[i] != tmp._powers[i]) throw "Not equal powers";
        }
        return Manom(_coef + tmp._coef, _size, _powers);
    }

    Manom operator-(const Manom& tmp) const {
        if (_size != tmp._size) throw "Not equal size";
        for (int i = 0; i < _size; i++) {
            if (_powers[i] != tmp._powers[i]) throw "Not equal powers";
        }
        return Manom(_coef - tmp._coef, _size, _powers);
    }
    Manom operator*(const Manom& tmp) const {
        size_t max_size = std::max(_size, tmp._size);
        size_t min_size = std::min(_size, tmp._size);
        int* new_powers = new int[max_size];
        int* tmp2 = max_size == _size ? _powers : tmp._powers;
        for (int i = 0; i < max_size; i++) {
            if (i < min_size) {
                new_powers[i] = _powers[i] + tmp._powers[i];
            }
            else {
                new_powers[i] = tmp2[i];
            }
        }
        return Manom(_coef * tmp._coef, max_size, new_powers);
    }

    Manom operator/(const Manom& tmp) const {
        size_t max_size = std::max(_size, tmp._size);
        size_t min_size = std::min(_size, tmp._size);
        int* new_powers = new int[max_size];
        int* tmp2 = max_size == _size ? _powers : tmp._powers;
        for (int i = 0; i < max_size; i++) {
            if (i < min_size) {
                new_powers[i] = _powers[i] - tmp._powers[i];
            }
            else {
                new_powers[i] = tmp2[i];
            }
        }
        return Manom(_coef / tmp._coef, max_size, new_powers);
    }
    bool operator==(const Manom& tmp) const {
        if (_size != tmp._size) return false;
        for (int i = 0; i < _size; i++) {
            if (_powers[i] != tmp._powers[i]) return false;
        }
        return (_coef == tmp._coef);
    }

    bool IsPowersEqual(const Manom& monom) const {
        if (_size != monom._size) return false;
        for (int i = 0; i < _size; i++) {
            if (_powers[i] != monom._powers[i]) return false;
        }
        return true;
    }
    double getCoef() const { return _coef; }
    size_t getSize() const { return _size; }
    int* getPowers() const { return _powers; }

    double evaluate(const std::vector<double>& values) const {
        double result = _coef;
        for (size_t i = 0; i < _size; ++i) {
            if (values.size() > i) {
                result *= pow(values[i], _powers[i]);
            }
            else {
                throw "Not enough values";
            }

        }
        return result;
    }

    void print() const {
        if (_coef == 0) {
            cout << 0 << endl;
        }
        else {
            cout << _coef << "*";
            for (int i = 0; i < _size; i++) {
                if (i != _size - 1)
                    cout << "x" << (i + 1) << "^" << _powers[i] << " ";
                else
                    cout << "x" << (i + 1) << "^" << _powers[i];
            }
        }
    }
};

class Polynom : public List<Manom> {
public:
    Polynom() : List<Manom>() {}
    Polynom(const Polynom& other) : List<Manom>() {
        for (auto it = other.begin(); it != other.end(); ++it) {
            insertEnd(*it.get());
        }
    }

    Polynom& operator=(const Polynom& other) {
        if (this == &other) return *this;

        while (!isEmpty()) {
            removeFront();
        }
        for (auto it = other.begin(); it != other.end(); ++it) {
            insertEnd(*it.get());
        }
        return *this;
    }


    Polynom operator+(const Polynom& other) const {
        Polynom result;
        for (auto it = this->begin(); it != this->end(); ++it) {
            result.insertEnd(*it.get());
        }
        for (auto it = other.begin(); it != other.end(); ++it) {
            Manom* currentResult = nullptr;
            for (auto it2 = result.begin(); it2 != result.end(); ++it2) {
                if (it2.get()->IsPowersEqual(*it.get())) {
                    *it2.get() = *it2.get() + *it.get();
                    currentResult = it2.get();
                    break;
                }
            }
            if (!currentResult) {
                result.insertEnd(*it.get());
            }
        }

        return result;
    }

    Polynom operator-(const Polynom& other) const {
        Polynom result;
        for (auto it = this->begin(); it != this->end(); ++it) {
            result.insertEnd(*it.get());
        }
        for (auto it = other.begin(); it != other.end(); ++it) {
            Manom* currentResult = nullptr;
            for (auto it2 = result.begin(); it2 != result.end(); ++it2) {
                if (it2.get()->IsPowersEqual(*it.get())) {
                    *it2.get() = *it2.get() - *it.get();
                    currentResult = it2.get();
                    break;
                }
            }
            if (!currentResult) {
                Manom temp = *it.get();
                temp = Manom(temp.getCoef() * -1, temp.getSize(), temp.getPowers());
                result.insertEnd(temp);
            }
        }

        return result;
    }
    Polynom operator*(const Polynom& other) const {
        Polynom result;
        for (auto it1 = this->begin(); it1 != this->end(); ++it1) {
            for (auto it2 = other.begin(); it2 != other.end(); ++it2) {
                Manom temp = *it1.get() * *it2.get();
                result.insertEnd(temp);
            }
        }
        return result;
    }
    double evaluate(const std::vector<double>& values) const {
        double result = 0.0;
        for (auto it = begin(); it != end(); ++it) {
            result = result + it.get()->evaluate(values);
        }
        return result;
    }

    void print() const {
        if (isEmpty()) {
            cout << 0;
            return;
        }
        for (auto it = begin(); it != end(); ++it) {
            it.get()->print();
            if (it.getNext() != end()) {
                cout << " + ";
            }
        }
        cout << endl;
    }
    static Polynom inputFromConsole() {
        Polynom result;
        string input;
        cout << "Enter the polynomial (e.g., 2*x1^2 x2^3 + 3*x1^1 x2^2):" << endl;
        getline(cin, input);

        stringstream ss(input);
        string term;

        while (getline(ss, term, '+')) {
            stringstream termStream(term);
            double coef;
            char xChar;
            string part;

            termStream >> coef;


            Manom currentManom;
            if (termStream >> xChar && xChar == '*') {
                vector<int> powers;

                while (termStream >> xChar && xChar == 'x') {
                    int index;
                    char powChar;
                    int power;
                    if (termStream >> index && termStream >> powChar && powChar == '^' && termStream >> power)
                    {
                        powers.push_back(power);
                    }
                }

                int* powerArr = new int[powers.size()];
                for (size_t i = 0; i < powers.size(); i++) {
                    powerArr[i] = powers[i];
                }
                currentManom = Manom(coef, powers.size(), powerArr);
                delete[] powerArr;
            }
            else {
                currentManom = Manom(coef, 1, new int[1] {0});
                delete[] currentManom.getPowers();
            }

            result.insertEnd(currentManom);
        }
        return result;
    }
};

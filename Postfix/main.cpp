#include "TFormula.h"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "ru");

    TFormula a;
    try {
        a.FormulaConverter();
        a.getPostfix();
        cout << "Result: " << a.FormulaCalculator() << endl;
    }
    catch (const char* msg) {
        cerr << msg << endl;
    }

    return 0;
}

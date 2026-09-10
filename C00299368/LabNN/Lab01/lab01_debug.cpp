// Lab 01 - Debugging fundamentals
// Created on: 02 SEP 2026 - OA
//
// Student Name  :
// Student ID:
//
// Four intentional bugs are marked with //<BUG_x>. Fix them one at a time.
// Leave the marker in place; annotate with the fix, e.g.
//     //<BUG_1 fixed: swapped '-' for '+'>
//
// Build:  g++ -std=c++17 -Wall -Wextra lab01_debug.cpp -o lab01_debug
// Run  :  ./lab01_debug

#include <iostream>
using namespace std;

class Arithmetic1 {
public:
    int add(int a, int b) {
        return a + b;   //<BUG_1 fixed: swapped "-" for "+">
    }

    int subtract(int a, int b) {
        return a + b;   //<BUG_2>
    }
};

class Arithmetic2 {
public:
    int multiply(int a, int b) {
        return a / b;   //<BUG_3>
    }

    double divide(int a, int b) {
        return a / b;   //<BUG_4>
    }
};

int main() {
    Arithmetic1 ar1;
    Arithmetic2 ar2;

    int x = 10, y = 5;
    cout << "Addition       (" << x << " + " << y << ") = " << ar1.add(x, y)      << endl;
    cout << "Subtraction    (" << x << " - " << y << ") = " << ar1.subtract(x, y) << endl;
    cout << "Multiplication (" << x << " * " << y << ") = " << ar2.multiply(x, y) << endl;
    cout << "Division       (" << x << " / " << y << ") = " << ar2.divide(x, y)   << endl;

    return 0;
}


#include <iostream>
#include <string>


// 函数模版
template <typename T> T fuc(T a, T b) {
    return a + b;
}

// 函数模版2
template <typename T>
T func(T a, T b) {
    return a + b;
}


int main() {

    int a = 10, b = 20;

    // a > b ? a : b是左值，可以被赋值，并且不会报错
    a > b ? a : b = 100;
}
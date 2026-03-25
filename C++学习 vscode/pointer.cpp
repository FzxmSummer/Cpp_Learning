#include <iostream>
#include <string>

using std::string;

int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}

int divide(int a, int b) {
    if (b == 0) {
        throw std::invalid_argument("除数不能为零");
    }
    return a / b;
}

void calculate(int a, int b, int(*func)(int, int)) {
    std::cout << "结果：" << func(a, b) << std::endl;
}

int main() {

    calculate(10, 5, add);
    calculate(10, 5, subtract);
    calculate(10, 5, multiply);
    try {
        calculate(10, 0, divide);
    } catch (const std::invalid_argument& e) {
        std::cout << "错误：" << e.what() << std::endl;
    }

    return 0;
}
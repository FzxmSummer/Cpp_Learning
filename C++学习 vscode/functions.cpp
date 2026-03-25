#include <iostream>

void func(int* const p) {
    std::cout << "形参P地址" << &p << std::endl;

    // p = nullptr; // 错误：不能修改const指针
// p = new int(20); // 错误：不能修改const指针
    *p = 10;

    // 内部修改指针p的值，并不会影响实参px的值，因为p是px的副本

}


int main() {
    int x = 5;
    int* px = &x;
    std::cout << "实参Px地址" << &px << std::endl;
    func(px);
    std::cout << x << std::endl; // 输出10：外部变量被修改
    std::cout << (px == nullptr) << std::endl; // 输出0：p_x未被修改

    return 0;
}
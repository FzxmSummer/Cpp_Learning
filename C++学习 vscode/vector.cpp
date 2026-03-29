#include <cstddef>      // 提供std::size_t等类型定义
#include <iostream>
#include <stdexcept>    // 提供std::out_of_range异常
#include <utility>      // 提供std::move等工具函数

template <typename T>
class Vector {
private:
    T* data_;               // 指向动态分配的数组
    std::size_t size_;      // 当前元素数量
    std::size_t capacity_;  // 当前分配的内存容量

public:
    /// 无参构造函数
    Vector() :data_(nullptr), size_(0), capacity_(0) {}

    /// @brief 有参构造函数
    /// @param count 创建的元素数量
    /// @param value 创建的元素值
    Vector(std::size_t count, const T& value = T())
        :data_(count == 0 ? nullptr : new T[count]), size_(count), capacity_(count) {
        for (std::size_t i = 0; i < size_; ++i) {
            data_[i] = value;
        }
    }

    /// @brief 拷贝构造
    /// @param other 对象元素
    Vector(const Vector& other)
        :data_(other.capacity_ == 0 ? nullptr : new T[other.capacity_]),
        size_(other.size_),
        capacity_(other.capacity_) {
        for (std::size_t i = 0; i < size_; i++) {
            data_[i] = other.data_[i];
        }
    }

    /// @brief 重载 = 运算符，实现深拷贝
    /// @param other 对象元素
    /// @return 引用
    Vector& operator= (const Vector& other) {
        if (this != &other) {
            // 创建内存空间
            T* new_data = other.capacity_ == 0 ? nullptr : new T[other.capacity_];
            for (std::size_t i = 0; i < other.size_; i++) {
                new_data[i] = other.data_[i];
            }

            // 释放原有资源
            delete[] data_;
            data_ = new_data;
            size_ = other.size_;
            capacity_ = other.capacity_;
        }

        return *this;
    }

    // 析构
    ~Vector() {
        delete[] data_;
    }

    /// @brief 元素插入末尾
    /// @param value 插入的元素值
    void push_back(const T& value) {
        if (size_ == capacity_) {
            // 动态扩容
            reserve(capacity_ == 0 ? 1 : capacity_ * 2);
        }

        data_[size_++] = value;
    }

    /// 弹出末尾元素
    void pop_back() {
        if (empty()) {
            throw std::out_of_range("pop_back on empty vector");
        }

        // 直接移动标记
        --size_;
    }

    /// @brief 扩容数组
    /// @param new_capacity 新的内存分配容量
    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }

        // 先完成新资源构造，把旧资源当做备份
        T* new_data = new T[new_capacity];
        for (std::size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }

        delete[] data_;
        data_ = new_data;       //最后替换旧资源，这样可以避免异常时候新资源没有完成，旧资源也出现错误
        capacity_ = new_capacity;
    }

    /// @brief 获取vector当前容量
    /// @return vector当前容量
    std::size_t size() const {
        return size_;
    }

    /// @brief 获取vector最大容量
    /// @return vector最大容量
    std::size_t capacity() const {
        return capacity_;
    }

    /// @brief 判断vector是否为空
    /// @return 结果
    bool empty() const {
        return size_ == 0;
    }

    /// @brief 清空vector
    void clear() {
        size_ = 0;
    }

    /// @brief 重载bool()运算符
    explicit operator bool() const noexcept {
        return size_ != 0;
    }

    /// 重载[]
    T& operator[] (std::size_t index) {
        return data_[index];
    }

    /// @brief 重载[] const使用
    /// @param index 索引下标 -- 下标不做判断，为了实现快速访问，更接近原生数组效果
    /// @return 数组值
    const T& operator[] (std::size_t index) const { // 后置const为了让const常量调用，前置const减少拷贝操作且不允许修改
        return data_[index];
    }

    /// @brief 返回下标处的值
    /// @param index 索引下标
    /// @return 数组值
    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    /// @brief 返回下标处的值 const使用
    /// @param index 索引下标
    /// @return 不允许修改的数组值
    const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index out of range");
        }
        return data_[index];
    }

    // 本身设计就不好，不应该存在输入输出流，并非无法实现，而是类型并不适合
    // 重载 << -- 此处声明并定义，正常定义需要定义在.h文件中
    friend std::ostream& operator<<(std::ostream& out, const Vector& vec) {
        out << "[";
        for (std::size_t i = 0; i < vec.size_; ++i) {
            out << vec.data_[i];
            if (i + 1 != vec.size_) {
                out << ",";
            }
        }
        out << "]";
        return out;
    }
};


// 测试
int main() {
    Vector<int> numbers;
    numbers.push_back(10);
    numbers.push_back(20);
    numbers.push_back(30);

    std::cout << "numbers = " << numbers << '\n';
    std::cout << "size = " << numbers.size() << ", capacity = " << numbers.capacity() << '\n';

    numbers[1] = 99;
    std::cout << "修改下标 1 后: " << numbers << '\n';

    Vector<int> copied;
    copied = numbers;
    copied.push_back(123);
    std::cout << "copied = " << copied << '\n';
    std::cout << "numbers = " << numbers << '\n';
    return 0;
}
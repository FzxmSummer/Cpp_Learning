#include <iostream>
#include <utility>


// 定义删除器模版 -- 主模版
template<typename T>
struct DefaultDelete {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

// 定义对象数组类型删除器模版 -- 模版特化
template<typename T>
struct DefaultDelete<T[]> {
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};


template<typename T, typename Deleter = DefaultDelete<T>>
class UniquePtr {
private:
    T* ptr;          // 内部指针
    Deleter deleter; // 删除器

public:
    // 1. 构造函数-- 禁止隐式转换，承诺不抛出异常
    explicit UniquePtr(T* p = nullptr) noexcept
        : ptr(p), deleter() {
    }

    // 多参数无隐式转换风险
    UniquePtr(T* p = nullptr) noexcept
        : ptr(p), deleter() {
    }

    // 2. 禁止拷贝构造函数
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // 3. 移动构造(右值引用)
    UniquePtr(UniquePtr&& other) noexcept
        :ptr(other.ptr), deleter(std::move(other.deleter)) { // std::move(other.deleter)转让所有权，掠夺资源
        other.ptr = nullptr;
    }

    // 4. 移动赋值
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset();            // 释放当前资源
            ptr = other.ptr;    // 获取新的内存资源
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
        }
        return *this;
    }

    // 5. 析构函数
    ~UniquePtr() {
        if (ptr) {
            delete(ptr);
        }
    }

    // 6.1 解引用,实现类似指针 (*p).xxx 的功能
    T& operator*() const {  // ptr可能为空指针，解引用会报错，                           
        return *ptr;        //同时也不确定具体的T类型是否重写了解引用方法，所以无法承诺不抛出异常
    }

    // 6.2 解引用,实现类似指针 p->xxx 的功能
    T* operator->() const noexcept {
        return ptr;
    }

    // 7. 获取原始指针
    T* get() const noexcept {
        return ptr;
    }

    // 8. 类型转换操作符重载(本身可以直接作为bool值一样来使用，判断是否为空,但是禁止隐式类型转换)
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    // 9. 释放所有权(手动管理该指针所指内存空间)
    T* release() noexcept {
        T* old = ptr;
        ptr = nullptr; // 指针变量置空
        return old;
    }

    // 10. 交换数据
    void swap(UniquePtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(deleter, other.deleter);
    }

    // 11. 释放当前资源(只是释放了指针)
    void reset(T* p = nullptr) noexcept {
        T* old = ptr;
        ptr = p;
        if (old) {
            deleter(old);
        }
    }
};



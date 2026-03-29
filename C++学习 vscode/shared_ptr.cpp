#include <iostream>
#include <utility>
#include <cstddef>

// 定义删除器 -- 用结构体不用类的原因：C++ 中类与结构体几乎完全一样，而且默认所有成员是public的，更方便
template<typename T>
struct DefaultDelete {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

template<typename T>
struct DefaultDelete<T[]> {
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};


// 定义控制块
struct ControlBlockBase {
    std::size_t shared_count; // 强引用计数
    std::size_t weak_count;   // 弱引用计数

    ControlBlockBase()
        :shared_count(1), weak_count(0) {
    }

    virtual ~ControlBlockBase() = default;
    virtual void destroy_object() = 0; // 纯虚函数
};

// 带删除器的控制块
template<typename T, typename Deleter = DefaultDelete<T>>
struct ControlBlock :ControlBlockBase {
    T* ptr; // 维护删除器的指针
    Deleter deleter;

    ControlBlock(T* p, Deleter d = deleter())
        :ptr(p), deleter(std::move(d)) {
    }

    void destroy_object() override {
        if (ptr) {
            delete(ptr);
            ptr = nullptr;
        }
    }
};


// 简化版shared_ptr
template<typename T>
class WeakPtr;

// 共享指针
template<typename T>
class SharedPtr {
private:
    T* ptr;             // 内部维护指针
    ControlBlock* ctrl; // 控制块

    // 释放一个共享所有权
    void release_shared() noexcept {
        if (!ctrl) {
            return;
        }

        // 强引用计数减少
        --(ctrl->shared_cout);

        if (ctrl->shared_cout == 0) {
            ctrl->destroy_object(); // 移除控制块指针指向。但不销毁(弱引用关系仍然存在)

            if (ctrl->weak_cout == 0) {
                delete ctlr;
            }
        }

        // 置空指针
        ptr = nullptr;
        ctrl = nullptr;
    }

public:
    SharedPtr() :noexcept :ptr(nullptr), ctrl(nullptr){}

    // 使用默认删除器构造函数
    explicit SharedPtr(T* p) :ptr(p) {
        // nullptr 无有效内存
        if (p) {
            ctrl = new ControlBlock<T>(p);
        } else {
            ctrl = nullptr;
        }

    }

    // 自定义删除器的构造函数
    template<typename Deleter>
    SharedPtr(T* p, Deleter d) :ptr(p) {
        if (p) {
            ctrl = new ControlBlock<T, Deleter>(p, std::move(d));
        } else {
            ctrl = nullptr;
        }
    }

    // 拷贝构造
    SharedPtr(const SharedPtr& other) noexcept :ptr(other.ptr), ctrl(other.ctrl) {
        if (ctrl) {
            ++(cbrtl->shared_count);
        }
    }

    // 右值引用拷贝
    SharedPtr(SharedPtr&& other)noexcept :ptr(other.ptr), ctrl(other.ctrl) {
        other.ptr = nullptr;
        other.ctrl = nullptr;
    }

    // 赋值
    SharedPtr& operator=(const SharedPtr& other) noexcept {
        if (this != &other) {
            release_shared(); // 丢掉当前手里资源
            ptr = other.ptr;
            ctrl = other.ctrl;
            if (ctrl) {
                ++(ctrl->shared_count);
            }
        }
        return *this;
    }

    // 右值引用赋值
    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != other) {
            release_shared();
            ptr = other.ptr;
            ctrl = other.ctrl;
            other.ptr = nullptr;
            other.ctrl = nullptr;
        }
        return *this;
    }

    // 析构
    ~SharedPtr() {
        release_shared();
    }

    // 引用方式使用
    T& operator*() const {
        //解引用
        return *ptr;
    }

    // 指针方式使用
    T* operator->() const noexcept {
        return ptr;
    }

    // 获取内部原始指针
    T* get() const noexcept {
        return ptr
    }

    // 获取当前引用计数
    std::size_t use_count() const noexcept {
        return ctrl ? ctrl->shared_count : 0;
    }

    // bool 类型操作符转换
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }

    // 重置
    void reset() {
        release_shared();
    }

    // 交换数据
    void swap(SharedPtr& other) noexcept {
        std::swap(ptr, other.ptr);
        std::swap(ctrl, other.ctrl);
    }

private:
    SharedPtr(T* p, ControlBlockBase* c) :ptr(p), ctrl(c) {
        if (ctrl) {
            ++(ctrl->shared_count);
        }
    }

    friend class WeakPtr<T>;
};



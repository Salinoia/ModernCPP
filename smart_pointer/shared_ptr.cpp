#include <memory>
#include <iostream>

struct MyStruct {
    MyStruct() { std::cout << "MyStruct Constructor\n"; }
    ~MyStruct() { std::cout << "MyStruct Destructor\n"; }
};

int main() {
    // std::shared_ptr<int> shared(new int(100));// 不推荐的初始化方式

    // int* pi = new int(100);
    // std::shared_ptr<int> shared_3(pi);// 禁止使用裸指针初始化

    // 智能指针对于数组的支持并不好
    std::shared_ptr<int> shared(new int[100]());
    std::cout << shared.get()[10] << "\n";

    std::shared_ptr<MyStruct> ptr1 = std::make_shared<MyStruct>();

    std::cout << ptr1.unique() << "\n"; // 输出1，此时是独占的

    std::shared_ptr<MyStruct> ptr2 = ptr1; // 引用计数增加到2
    std::cout << "Before reset, use_count: " << ptr1.use_count() << "\n"; // 输出 2
    std::cout << ptr2.unique() << "\n"; // 输出0，此时不是独占的
    ptr1.reset(); // ptr1 释放所有权，引用计数减少到1


    std::cout << "After reset, ptr1 use_count: " << ptr1.use_count() << "\n"; // 输出 0
    ptr2.reset();// 当 ptr2 释放所有权，引用计数降为0，MyStruct 被销毁
    if(ptr2.get() == nullptr) std::cout << "ptr2 is nullptr" << "\n"; // 输出ptr2 is nullptr
    std::cout << "After reset, ptr2 use_count: " << ptr2.use_count() << "\n"; // 输出 0

    

    return 0;
}
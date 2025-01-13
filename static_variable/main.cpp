#include <iostream>

unsigned test(){
    static unsigned callCount = 0;
    return ++callCount;
}

int main(){
    test();
    test();
    test();
    unsigned testFuncCallCount = test();
    std::cout << testFuncCallCount <<std::endl;

    int i = 20;
    int* pi = &i;
    *pi = 10;// 直接修改值
    std::cout << pi << std::endl;
    ++pi;
    std::cout << pi << std::endl;

    // 以下两种写法，引用和常量指针在表达效果上可视为“等价”
    int& refI = i;
    int* const ptr_i = &i;

    return 0;
}


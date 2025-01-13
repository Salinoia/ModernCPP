#include<iostream>
#include<functional>

void test(int i){
    std::cout << i << "\n";
    std::cout << "Hello world.\n" << std::endl; 
}

class Test{
public:
    void operator()(int i){
        std::cout << i << "\n";
        std::cout <<  "Here is a functor.\n" << std::endl;
    }
};

using fp_type = void(*)(int); // 函数指针
using func_type = std::function<void(int)>;

void myFunc0(fp_type fp, int i){
    fp(i);
}

void myFunc1(func_type func, int i){
    func(i);
}

int main(){
    // 可调用对象
    // 1.最主要用法：为另一个函数传递参数
    myFunc0(test, 200);

    // 2.仿函数
    Test t;
    t(100);

    // 3.Lambda表达式
    int i = 10;
    int j = 20;

    // 基础用法
    auto lambda = [&i](int elem) -> int{// 捕获引用，修改变量本身
        i = 30;
        std::cout << elem << "\n";
        std::cout << "Here is a basic lambda expression.\n" << std::endl;
        return elem + i;
    };

    std::cout << lambda(200) << std::endl;

    // 常见用法：作为函数参数传递
    myFunc0([](int elem){
        std::cout << elem << std::endl;
        // std::cout << j << std::endl;// 错误，函数指针强制要求捕获列表为空
        std::cout << "Function Pointer Lambda. \n" << std::endl;
    }, 200);

    myFunc1([i](int elem){
        std::cout << elem << std::endl;
        std::cout << i << std::endl; // 引入C++11的std::function，能够灵活地接受任何符合签名的可调用对象
    }, 200);
}

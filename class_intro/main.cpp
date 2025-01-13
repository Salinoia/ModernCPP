#include<iostream>

struct TestStruct
{
    /*
        C语言的特点是：
        结构体对象声明的是位于栈区和堆区的
        而函数的声明则是严格位于代码区  
        单从汇编的角度来看是无法解决的
    */
    int i;
    int j[20];
    int *k;
};

void initialize(TestStruct test){
    test.k = (int*)malloc(test.i * sizeof(int));
}

class TestClass{
public:
    //默认构造函数
    TestClass(){};
    // 普通构造函数
    TestClass(int i_, int j_, int k_):i(i_), j(j_), pi(new int(k_)){}
    int get_integer_i(){
        return this->i;
    }
    // 拷贝构造函数
    TestClass(const TestClass& CppTest): i(CppTest.i), j(CppTest.j), pi(new int(*CppTest.pi)){}

    // 移动构造函数

    /*
        暂略
    */

   // 动态申请内存的情况必须显式析构
   ~TestClass(){
    delete this->pi;
   }

private:
    int i;
    int j;
    int *pi;
};

int main(){
    TestStruct CTest;
    TestClass CppTest0;
    TestClass CppTest1(1, 2, 3);
    TestClass CppTest2(CppTest1);


    std::cout << CTest.i << "\n";// 此时调用实际上是未定义行为
    std::cout << CppTest0.get_integer_i() << std::endl;
    std::cout << CppTest1.get_integer_i() << std::endl;
    std::cout << CppTest2.get_integer_i() << std::endl;
    return 0;
}
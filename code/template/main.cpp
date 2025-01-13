// main.cpp
#include <iostream>
#include <vector>
#include <functional>

namespace mystd{
    using void_int_func_type = std::function<void(int&)>;// 默认模板参数
    template<typename iter_type, typename func_type = void_int_func_type>// 函数模板
    void for_each(iter_type first, iter_type last, func_type func = [](int& elem){
        ++elem;
    })
    {
        for(auto iter = first; iter != last; ++iter){
            func(*iter);
        }
    }

    template<typename T, typename allocator_type = std::allocator<T>>
    class MyVector{
    public:
        template<typename T2>
        void outPut(const T2& elem);
    };
    template<typename T, typename allocator>
    template<typename T2>
    void MyVector<T, allocator>::outPut(const T2& elem){// 成员函数模板
        std::cout << elem << "\n";
    }
};


int main() {
    std::vector<int> vec{1, 2, 3, 4, 5};


    mystd::for_each(vec.begin(), vec.end());
    for(int elem : vec){
        std::cout << elem << "\n";
    }

    return 0;
}
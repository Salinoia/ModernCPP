#include <iostream>
#include <vector>
#include <stdexcept> // For std::out_of_range

template<typename T>
class Test {
public:
    const T& operator[](size_t index) const;
    void add(const T& value); // 添加一个元素到data
private:
    std::vector<T> data;
};

template<typename T>
const T& Test<T>::operator[](size_t index) const {
    if (index >= data.size()) {
        // throw std::out_of_range("Accessing elements beyond valid range in a template class.");
        throw 42;
    }
    return data[index];
}

template<typename T>
void Test<T>::add(const T& value) {
    data.push_back(value);
}

int main() {
    try {
        Test<int> test;

        // 添加一些元素到容器
        test.add(10);
        test.add(20);

        // 正常访问
        std::cout << "Element at index 0: " << test[0] << std::endl;

        // 越界访问，触发异常
        std::cout << "Element at index 2: " << test[2] << std::endl;
    }
    catch (const std::out_of_range& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
    }
    catch(...){
        std::cout << "That's some kind of humor." << std::endl;
    }
    /*
        Element at index 0: 10
        Element at index 2: Exception caught: Accessing elements beyond valid range in a template class.
    */
    return 0;
}

// MyArray.hpp
#ifndef MYARRAY_HPP
#define MYARRAY_HPP

#include <cstddef>    // for size_t
#include <algorithm>  // for std::copy, std::swap
#include <stdexcept>  // for std::out_of_range
#include <type_traits> // for std::is_pointer
#include <vector>

template<typename T>
class MyArray {
public:
    using iterator = T*;
    using const_iterator = const T*;

    // 构造函数
    explicit MyArray(size_t count = 0);

    // 补充initializer_list初始化方法
    MyArray(const std::initializer_list<T>& list);
    MyArray(std::initializer_list<T> && list);

    // 拷贝构造函数
    MyArray(const MyArray& other);

    // 移动构造函数
    MyArray(MyArray&& other) noexcept;

    // 拷贝赋值运算符
    MyArray& operator=(const MyArray& other);

    // 移动赋值运算符
    MyArray& operator=(MyArray&& other) noexcept;

    // 析构函数
    ~MyArray();

    // 元素访问
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // 获取数组大小
    size_t size() const;

    // 迭代器
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

private:
    T* data;// 修改为std::vector容器实现
    std::vector<T> vec_data;
    size_t m_size;

    void swap(MyArray& other) noexcept;
};

// 构造函数
template<typename T>
MyArray<T>::MyArray(size_t count) : data(nullptr), m_size(count) {
    if (count > 0) {
        data = new T[count]();
    }
}

template<typename T> 
struct get_type
{
    using type = T;
};

template<typename T> 
struct get_type<T*>
{
    using type = T;
};


template<typename T>
MyArray<T>::MyArray(const std::initializer_list<T>& list){
    if(list.size()){
        unsigned count = 0;
        m_size = list.size();
        data = new T[m_size]();
        if(std::is_pointer<T>::value){// 解决指针浅拷贝的方式，使用模板特化或者萃取机制，当模板体积较大时一般使用萃取机制，此处可以采取很巧妙的方式实现
            for(auto elem : list){
                data[count++] = new typename get_type<T>::type(*elem);// 注意：这段代码是一个非常经典的内存泄漏问题，如果使用裸指针管理则非常容易引起内存泄漏
            }
        }
        else{
            for(const auto& elem : list){
                data[count++] = elem;
            }
        }
    }
    else{
        data = nullptr;
    }
}

template<typename T>
MyArray<T>::MyArray(std::initializer_list<T> && list){
    if(list.size()){
        unsigned count = 0;
        m_size = list.size();
        data = new T[m_size]();
        for(const auto& elem : list){// 对于右值引用来说，如果是指针类型就直接转移权限了
            data[count++] = std::move(elem);
        }
    }
    else{
        data = nullptr;
    }
}

// 拷贝构造函数
template<typename T>
MyArray<T>::MyArray(const MyArray& other) : data(nullptr), m_size(other.m_size) {
    if (m_size > 0) {
        data = new T[m_size];
        std::copy(other.data, other.data + m_size, data);
    }
}

// 移动构造函数
template<typename T>
MyArray<T>::MyArray(MyArray&& other) noexcept : data(other.data), m_size(other.m_size) {
    other.data = nullptr;
    other.m_size = 0;
}

// 拷贝赋值运算符
template<typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray& other) {
    if (this != &other) {
        MyArray temp(other); // 利用拷贝构造函数
        swap(temp);
    }
    return *this;
}

// 移动赋值运算符
template<typename T>
MyArray<T>& MyArray<T>::operator=(MyArray&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        m_size = other.m_size;
        other.data = nullptr;
        other.m_size = 0;
    }
    return *this;
}

// 析构函数
template<typename T>
MyArray<T>::~MyArray() {
    if (data) {
        // 如果是指针类型，我们在构造中做了 new U(*elem)，这里要delete
        if (std::is_pointer<T>::value) {
            for (size_t i = 0; i < m_size; ++i) {
                delete data[i]; // data[i] 是 T = U*
            }
        }
        delete[] data;
    }
}

// 元素访问
template<typename T>
T& MyArray<T>::operator[](size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
const T& MyArray<T>::operator[](size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// 获取数组大小
template<typename T>
size_t MyArray<T>::size() const {
    return m_size;
}

// 迭代器
template<typename T>
typename MyArray<T>::iterator MyArray<T>::begin() {
    return data;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::begin() const {
    return data;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::cbegin() const {
    return data;
}

template<typename T>
typename MyArray<T>::iterator MyArray<T>::end() {
    return data + m_size;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::end() const {
    return data + m_size;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::cend() const {
    return data + m_size;
}

// 交换函数
template<typename T>
void MyArray<T>::swap(MyArray& other) noexcept {
    std::swap(data, other.data);
    std::swap(m_size, other.m_size);
}

#endif // MYARRAY_HPP

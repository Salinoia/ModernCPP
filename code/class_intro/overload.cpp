#include<stdexcept>
#include<iostream>
class vector2{
private:
    float x, y;
public:
    vector2(float x, float y) :x(x), y(y){}

    // 前置自增 (++vector2)
    vector2& operator++() {
        ++x;
        ++y;
        return *this;
    }

    // 后置自增 (vector2++)
    vector2 operator++(int) {
        vector2 temp = *this;  // 备份当前对象
        ++(*this);             // 调用前置自增
        return temp;           // 返回备份的对象
    }

    // 前置自减 (--vector2)
    vector2& operator--() {
        --x;
        --y;
        return *this;
    }

    // 后置自减 (vector2--)
    vector2 operator--(int) {
        vector2 temp = *this;  // 备份当前对象
        --(*this);             // 调用前置自减
        return temp;           // 返回备份的对象
    }    
    // 非const版本的下标操作符
    float& operator[](int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // const版本的下标操作符
    const float& operator[](int index) const {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // 非const版本的圆括号操作符
    float& operator()(int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // const版本的圆括号操作符
    const float& operator()(int index) const {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    friend std::ostream& operator<<(std::ostream& os, const vector2& vec){
        return os << vec.x << ", " << vec.y;
    }
    friend std::istream& operator>>(std::istream& is, vector2& vec){
        is >> vec.x >> vec.y;
        return is;
    }

    // vector2 add(const vector2& other){
    //     // 做一些额外的操作，比如日志记录、检查等
    //     return vector2(x + other.x, y + other.y);
    // }

    vector2 operator+(const vector2& other){
        return vector2(x + other.x, y + other.y); // 不做其余操作即可直接重载
        // return add(other);
    }

    bool operator==(const vector2& other) const{
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const vector2& other) const{
        // return !(operator==(other)); // That's weird
        return !(*this == other);
    }

    // 赋值运算符会默认进行重载
    vector2& operator=(const vector2& other){
        if(this == &other) return *this;
        x = other.x;
        y = other.y;
        return *this;
    }
};
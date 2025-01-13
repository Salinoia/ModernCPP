#include <cstring>
#include <iostream>


class TestString{
public:
    TestString(const TestString& test){// 拷贝构造函数
        if(test.str){
            str = new char[strlen(test.str) + 1]();
            strcpy(str, test.str);
        }
        else{
            str = nullptr;
        }
    }

    TestString(TestString&& test){// 移动构造函数
        if(test.str){
            str = test.str;
            test.str = nullptr;
        }
        else{
            str = nullptr;
        }
    }

    TestString& operator=(const TestString& test){// 赋值运算符
        if(this == &test){
            return *this;
        }
        if(str){
            delete[] str;
            str = nullptr;
        }
        
        if(test.str){
            str = new char[strlen(test.str) + 1]();
            strcpy(str, test.str);
            // strcpy_s(str, strlen(test.str) + 1, test.str); // msvc支持
        }
        else{
            str = nullptr;
        }
        return *this;
    }

    TestString& operator=(TestString&& test){// 移动赋值运算符
        if(this == &test){
            return *this;
        }
        if(str){
            delete[] str;
            str = nullptr;
        }
        if(test.str){
            str = test.str;
            test.str = nullptr;
        }
        else{
            str = nullptr;
        }
        return *this;
    }

    ~TestString(){
        if (str){
            delete[] str;
        }
    }

private:
    char* str;
};
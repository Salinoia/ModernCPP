#include <iostream>

template<typename T1, typename T2>
class Test{
public:
    Test(){
        std::cout << "common template" << "\n";
    }
};

template<typename T1, typename T2>
class Test<T1*, T2*>{
public:
    Test(){
        std::cout << "pointer template partial specialization" << "\n";
    }
};

template<typename T2>
class Test<int, T2>{
public:
    Test(){
        std::cout << "int template partial specialization" << "\n";
    }
};


template<>
class Test<int, int>{
public:
    Test(){
        std::cout << "int, int template full specialization" << "\n";
    }
};

int main(){
    Test<int*, int*> test1;
    Test<int, double> test2;
    Test<int, int> test3;
    Test<int, int*> test4;

    return 0;
}

/*
    pointer template partial specialization
    int template partial specialization
    int, int template full specialization
    int template partial specialization
*/
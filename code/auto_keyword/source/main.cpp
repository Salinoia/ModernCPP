#include<iostream>
#include<boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;


int main(){
    // 1. basic infer
    int x1 = 100;
    auto y1 = x1;
    std::cout << type_id_with_cvr<decltype(y1)>().pretty_name() << "\n";

    // 2.about reference
    int x2 = 100;
    int& ref2 = x2;
    auto y2 = ref2;
    std::cout << type_id_with_cvr<decltype(y2)>().pretty_name() << "\n";


    // 3.about const
    const int x3 = 100;
    const int* const ptr3 = &x3;
    auto y3 = ptr3;
    std::cout << type_id_with_cvr<decltype(y3)>().pretty_name() << "\n";

    // 4.auto& 
    const int x4 = 100;
    const int* const ptr4 = &x4;
    const auto& y4 = ptr4;
    std::cout << type_id_with_cvr<decltype(y4)>().pretty_name() << "\n";

    return 0;
}
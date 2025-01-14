#include <iostream>
#include <fstream>
#include <memory>
#include <vector>

int main(){
    std::vector<std::shared_ptr<int>> unique_int_vec;
    try{
        while(true){
            std::unique_ptr<int> uniqueI(new int[10000]());
            unique_int_vec.emplace_back(std::move(uniqueI));
        }
    }
    catch(const std::bad_alloc& e){
        std::cout << e.what() << std::endl;
    }
    catch(...){
        std::cout << "unknown exception" << std::endl;
    }
    return 0;
}
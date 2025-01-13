#include <iostream>
#include <vector>
#include <algorithm>
   
using std::vector;
int main(){
    vector<int> iVec{1, 2, 3, 4, 5};
    iVec.erase(iVec.begin());
    // iVec.erase(iVec.begin(), iVec.begin() + 4);// vector的erase操作的迭代器区间是左开右闭的，这和vector容器的设计是一致的
    //clear() pop_back()
    for(int elem : iVec){
        std::cout << elem << "\n";
    }
    
    std::cout << iVec.size() << "\n"; // 输出4
    std::cout << iVec.capacity() << "\n";// 输出5
    return 0;
}
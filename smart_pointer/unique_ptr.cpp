#include<iostream>
#include<memory>

std::shared_ptr<int> myFunc(std::unique_ptr<int> unique){
    std::shared_ptr<int> shared(std::move(unique));// 使用得很少，需要将独占式指针转化为共享式指针常常是因为先前设计失误
    return shared;
}

int main(){
    std::unique_ptr<int> unique1 = std::make_unique<int>(100);
    std::unique_ptr<int> unique2 = std::make_unique<int>(200);
    // std::unique_ptr<int> unique2(unique1);
    // unique1 = unique2;// 禁止复制构造函数和赋值运算符重载

    std::unique_ptr<int> unique3(std::move(unique1)); // 允许移动构造，因为原有资源已被释放，依然是独占

    unique3.reset(); // 用法与shared_ptr一致

    return 0;
}
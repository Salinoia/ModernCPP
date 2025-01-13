#include <iostream>
#include <memory>

class Parent;
class Child;

class Child{
public:
    std::shared_ptr<Parent> parentPtr;
    // std::weak_ptr<Parent> parentPtr; // 解决方法，将其中一个定义为weak_ptr
};

class Parent{
public:
    std::shared_ptr<Child> childPtr;
};

int main(){
    std::shared_ptr<Parent> parentPtr = std::make_shared<Parent>();
    std::shared_ptr<Child> childPtr = std::make_shared<Child>();

    parentPtr->childPtr = childPtr;
    childPtr->parentPtr = parentPtr; // 两个堆内存互相指，最后都不会释放，出现内存泄漏

    return 0;
}

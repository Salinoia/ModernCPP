#include <iostream>
#include <thread>
#include <mutex>

unsigned counter = 0;
std::mutex mutex1; // 第一个互斥量
std::mutex mutex2; // 第二个互斥量

void thread_func1() {
    for (unsigned i = 0; i < 100000; ++i) {
        
        std::lock(mutex1, mutex2);

        std::lock_guard<std::mutex> lock1(mutex1, std::adopt_lock);
        /*
            ...
            some code 
        */
        std::lock_guard<std::mutex> lock2(mutex2, std::adopt_lock); 
        ++counter; // 操作共享资源
    }
}

void thread_func2() {
    for (unsigned i = 0; i < 100000; ++i) {
        std::lock(mutex1, mutex2);

        std::lock_guard<std::mutex> lock3(mutex2, std::adopt_lock); 
        /*
            ...
            some code 
        */
        std::lock_guard<std::mutex> lock4(mutex1, std::adopt_lock); 
        ++counter; // 操作共享资源
    }
}

int main() {
    std::thread t1(thread_func1); // 线程1运行 thread_func1
    std::thread t2(thread_func2); // 线程2运行 thread_func2

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << counter << std::endl;
    return 0;
}
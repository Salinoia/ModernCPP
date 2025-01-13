#include <thread>
#include <iostream>
#include <mutex>
#include <chrono>
#include <optional>

std::mutex MyMutex;  // 互斥量，用来保护共享数据
unsigned g_num = 0;  // 全局变量，将在多个线程中被修改

// 线程函数，增加 g_num
void increment() {
    // 使用 lock_guard 自动管理锁，避免手动解锁
    /*
        std::lock_guard 是一个模板类，其主要作用是：
        自动加锁：当一个 std::lock_guard 对象被创建时，它会立即加锁它所持有的 std::mutex。
        自动解锁：当 std::lock_guard 对象超出作用域时，它的析构函数会自动释放（解锁）互斥量，确保解锁操作总是发生。
    */
    /*
        此时锁的作用域覆盖了整个 for 循环，这意味着两个线程会交替地获取锁、执行循环、释放锁。
        这种情况下，锁的粒度较大，线程之间的竞争会非常频繁，导致性能下降。
        如果将锁的作用域缩小到 for 循环内部（即每次自增操作都加锁和解锁），表面上看起来锁的粒度更小，
        但实际上会引入更多的锁操作，导致性能进一步下降，当然，若涉及到更为复杂的操作，应当将锁加入循环体内部
    */
    auto start = std::chrono::high_resolution_clock::now();
    std::lock_guard<std::mutex> lg(MyMutex);
    for(unsigned i = 0; i < 10000; ++i) {
        ++g_num;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Time Costs: " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count() << " ns." << std::endl;
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Final g_num: " << g_num << std::endl;

    return 0;
}
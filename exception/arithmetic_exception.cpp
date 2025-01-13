#include <iostream>
#include <limits>
#include <stdexcept> // 引入标准异常库

// 改进后的除法函数，抛出std::invalid_argument异常
int divide(int divisor, int dividend) {
    if (dividend == 0) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }
    return divisor / dividend;
}

// 改进后的输入函数，增加异常处理
void clientInputNum(const std::string& str, int& num) {
    std::cout << str << std::endl;
    while (std::cin >> num, !std::cin.eof()) {
        if (std::cin.bad()) {
            throw std::runtime_error("Input stream is corrupted.");
        } else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Number format error, please input again." << std::endl;
            continue;
        }
        break;
    }
}

int main() {
    int divisor = 0;
    int dividend = 0;
    
    try {
        clientInputNum("Please input divisor:", divisor);
        clientInputNum("Please input dividend:", dividend);
        std::cout << "Result: " << divide(divisor, dividend) << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cout << "Error: " << e.what() << std::endl; // 捕获除数为零的异常
    }
    catch (const std::runtime_error& e) {
        std::cout << "Error: " << e.what() << std::endl; // 捕获输入流错误的异常
    }
    catch (...) {
        std::cout << "Unknown error occurred." << std::endl; // 捕获其他未知异常
    }

    return 0;
}

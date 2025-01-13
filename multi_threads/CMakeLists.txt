# 最低CMake版本要求
cmake_minimum_required(VERSION 3.10)

# 项目名称
project(MyProject)

# 使用 C++11 标准
set(CMAKE_CXX_STANDARD 20)

# 启用多线程支持
find_package(Threads REQUIRED)

# 添加源代码文件

# add_executable(my_program my_thread.cpp deadlock.cpp)
add_executable(my_program deadlock.cpp)

# 设置编译选项
add_compile_options(-g -O2)


# 链接 pthread 库
target_link_libraries(my_program PRIVATE Threads::Threads)

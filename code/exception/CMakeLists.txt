cmake_minimum_required(VERSION 3.10)


project(MyProject VERSION 0.1 LANGUAGES C CXX )

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURECES arithmetic_exception.cpp)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g)
else()
    add_compile_options(-O2)
endif()

# find_package(CUDA REQUIRED)

add_executable(MyProject ${SOURECES})
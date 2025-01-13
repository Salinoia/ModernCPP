# Minimum required version of CMake
cmake_minimum_required(VERSION 3.18)

cmake_policy(SET CMP0167 NEW)

project(MYPROJECT VERSION 1.1 LANGUAGES C CXX)

include_directories(${CMAKE_SOURCE_DIR}/include)

# Specify the C++ standards
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set boost library searching path
set(BOOST_ROOT /usr/include/boost)
set(Boost_NO_SYSTEM_PATHS ON)

find_package(Boost 1.62.0 REQUIRED COMPONENTS system filesystem thread)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    MESSAGE( STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
    MESSAGE( STATUS "Boost_LIBRARIES = ${Boost_LIBRARIES}.")
    MESSAGE( STATUS "Boost_LIB_VERSION = ${Boost_LIB_VERSION}.")
endif()

# Add source files from specific directories
file(GLOB SOURCES "${CMAKE_SOURCE_DIR}/source/*.cpp")

# Add the executable target
add_executable(MYPROJECT ${SOURCES})

target_link_libraries(MYPROJECT ${Boost_LIBRARIES})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pg")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")

# Set build type to Debug
set(CMAKE_BUILD_TYPE Debug)

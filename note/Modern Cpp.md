## 1.cpp基本特性

### 1.1 程序执行过程

在 C++ 中，内存通常分为几个不同的区域，每个区域有不同的用途。以下是主要内存类别的概述：

------

- **内存类别总结**：

| **类别**               | **用途**               | **生命周期**               | **特点**                                                     |
| ---------------------- | ---------------------- | -------------------------- | ------------------------------------------------------------ |
| **Stack Memory**       | 局部变量、函数调用信息 | 函数作用域                 | 自动分配/回收，速度快但大小有限，通常栈空间默认大小为8MB；Linux中可通过修改limits.config解除栈空间以及posix消息队列大小限制 |
| **Heap Memory**        | 动态内存分配           | 手动控制（程序生命周期内） | 灵活，较慢，需要手动管理                                     |
| **Static Memory**      | 全局变量、静态变量     | 程序生命周期               | 无需显式管理，程序运行期间始终存在                           |
| **Constant Memory**    | 常量、只读变量         | 程序生命周期               | 不可变，编译器可以进行优化                                   |
| **Code(Text) Segment** | 程序的可执行代码       | 程序生命周期               | 只读，包含机器码                                             |

![](https://gitee.com/salinoia/image/raw/master/image-20241206114835673.png)

- **补充知识**

| **类别**                               | **用途**                    | **生命周期**               |
| -------------------------------------- | --------------------------- | -------------------------- |
| **Block Starting Symbol(BSS) Segment** | 未显式初始化的全局/静态变量 | 程序生命周期               |
| **Data Segment**                       | 已显式初始化的全局/静态变量 | 程序生命周期               |
| **Memory-Mapped Segment**              | 文件、共享内存              | 多变，通常是程序生命周期内 |
| **Thread Local Storage(TLS)**          | 线程特定数据                | 线程生命周期               |

![](https://gitee.com/salinoia/image/raw/master/image-20241206105747707.png)

------

- **为何全局/静态变量需要区分为`.bss`与`.data`两个区域**

**根本目的，减少程序的大小**：

假设在嵌入式系统上运行 C 程序，代码和常量存储在 ROM（闪存）中。在这种系统中，启动时需要执行“拷贝下行”（将 ROM 中的初始化数据复制到 RAM）操作，将 ROM 中的初始化数据复制到 RAM 中，具体伪代码如下：

```cpp
for(i = 0; i < 显式初始化对象数量; i++)
{
    .data[i] = 初始化值[i];
}

memset(.bss, 0, 隐式初始化对象数量);
```

其中，`.data` 和 `.bss` 存储在 RAM 中，`初始化值` 存储在 ROM 中。如果 `.data` 和 `.bss` 是一个段（segment），那么 ROM 中需要填充大量零值，这会显著增加 ROM 大小；对于 RAM 驱动的可执行文件，虽然没有真正的 ROM，但原理类似；此外，`memset` 通常是高效的内联汇编，能够加快“拷贝下行”的速度。

**总结**：

`.data` 和 `.bss` 的区别在于，初始化时 `.data` 的复制操作可以顺序进行，而 `.bss` 段则需要填充为零。如果不分段处理，初始化过程会浪费时间。

------

- **`const`关键字介绍**

`const`修饰变量**使编译器视其为常量**，但**本质上仍然属于是可读写区的变量**，而不是只读区的常量，有着本质的区别；

`const`修饰的变量并非是不可修改的，本身C++就提供了`mutable`关键字可修改const修饰的常量


### 1.2 new关键字及内存泄漏

1. **`new` 关键字的功能**：

   - 用于动态分配内存，可以分配单变量或数组。
   - 动态分配的内存由程序员管理，需手动释放以避免内存泄露。

2. **动态分配内存的行为**：

   - 分配单个对象的内存
     - 如果分配的类型是普通变量，直接分配对应的内存。
     - 如果分配的是类对象，则会调用该类的构造函数。如果类未定义构造函数，则会报错。
   - 分配数组对象的内存
     - 普通变量数组：使用`()`可以将所有元素初始化为零。
     - 类对象数组：调用默认构造函数初始化；若无默认构造函数，分配失败并报错。

3. **内存泄露**：

   - 动态分配的内存如果未通过`delete`或`delete[]`释放，将导致内存泄露。
   - **内存泄露是最严重的错误之一，会导致堆内存逐渐被占用最后崩溃，程序不怕报错，就怕已经运行成功以后突然崩溃**；
   - 在Visual Studio开发中，可以使用**`VLD`**(Visual Leak Detector)工具进行检测；

   智能指针可以较好避免出现内存泄漏的问题；

   异常处理也可以恰当处理程序异常，或报错处理、或终止进程、或忽略，不要让异常莫名其妙，这是重要的程序设计理念；

### 1.3 命名空间

1. **基本概念**：

   - 在 C++ 中，**命名空间** 是一种用于组织代码的机制，它可以将不同的标识符（如变量、函数、类、对象等）组织到不同的范围内，以避免命名冲突；

   - 通过命名空间，开发者可以确保不同的代码模块或库之间不会因命名相同而产生冲突，尤其是在大型项目或多个第三方库合作时，比如较为经典的`String`类；

   - C++标准库所有的功能都定义在`std`命名空间中，因此使用时注意加上`std::`前缀；

2. **使用场景**：

   - 命名空间有助于组织代码，使得不同的模块或功能代码被分开管理。尤其是当项目变得庞大时，命名空间可以确保代码的可维护性和可扩展性，例如

   ~~~cpp
   namespace Graphics {
       class Renderer { /* 渲染相关代码 */ };
       void render() { /* 渲染相关函数 */ }
   }
   
   namespace Networking {
       class Server { /* 网络服务器相关代码 */ };
       void sendData() { /* 发送数据的函数 */ }
   }
   ~~~
   
   - 当多个库或者模块共享公共代码时，命名空间可以将这些代码组合成一个独立的单元，从而避免直接暴露到全局作用域；

   - 在函数或类的头文件中使用 `using namespace` 是**极其不推荐**的，尤其是使用 `using namespace std;`。这样会污染全局命名空间，可能会引起命名冲突。通常在源文件中使用 `using` 语句更为安全。

3. **命名空间的实现原理**

      命名空间的实现**并不是通过将 C++ 转换为 C 语言来实现的**，而是 C++ 编译器的一部分，它利用了 C++ 的 **符号解析** 和 **链接机制** 来实现命名空间的功能。


- **符号重命名（名称修饰）**
   - C++ 编译器通过对每个标识符进行“名称修饰”（**name mangling**），为每个命名空间内的标识符添加一个特定的前缀（如命名空间的名称），从而区分不同命名空间中的相同标识符。
   - 这种方式使得即便多个命名空间或全局作用域中存在同名的函数、类、变量等，它们在编译后生成的符号名是唯一的，不会发生冲突。

- **符号解析与链接**

   - 当C++ 代码被编译时，编译器会根据作用域（即命名空间）将标识符分配给不同的符号，并在目标代码中生成对应的符号地址。
   - **链接器**（Linker）会在程序链接时解析这些符号，确保每个标识符对应正确的命名空间，进而将不同命名空间中的符号正确地连接到程序中。


- **内部符号管理**
   - 在编译过程中，C++ 编译器会使用一个 **符号表** 来管理所有符号的名字和相关信息。当遇到命名空间时，编译器会将所有在该命名空间内定义的标识符归入该命名空间的作用域，并在生成目标代码时适当修改这些符号。
   - 如果一个标识符在某个命名空间内被声明，编译器就会把该符号存入该命名空间的符号表中。这样，即使标识符在多个命名空间中重复出现，它们也能通过不同的符号名来区分。

4. **`using`关键字**

  - **引入命名空间成员**：在当前作用域中直接使用命名空间内的成员，避免每次访问时都需要加上命名空间的前缀；例如：`using namesapce Eigen;`
  - **使用别名**：为类型或命名空间指定别名；例如：`using Eigen::Index`

### 1.4 Cpp的标准输入输出简介

​	C语言提供的函数接口简单方便，但彼此之间不存在关联；C++引入继承功能，可以使得子类父类具有关联性，极大提高各种输入输出功能之间的耦合性；

​	这就是`io`库的由来，C++ 提供了标准输入输出功能，主要通过 `<iostream>` 头文件中的标准流来实现。


---------
​	常用的标准流有：

1. **标准输入 (`std::cin`)**：

   - 用于从键盘读取数据。
   - 使用 `>>` 运算符接收输入。

   ```cpp
   int num;
   cin >> num;  // 从键盘输入
   ```

2. **标准输出 (`std::cout`)**：

   - 用于向屏幕输出数据。
   - 使用 `<<` 运算符输出信息。

   ```cpp
   cout << "Hello, World!" << endl;  // 输出到屏幕
   ```

3. **标准错误 (`std::cerr`)**：

   - 用于输出错误信息，通常不进行缓冲。

   ```cpp
   cerr << "Error!" << endl;  // 输出错误信息
   ```

4. **标准日志 (`std::clog`)**：

   - 用于输出日志信息，会进行缓冲。

   ```cpp
   clog << "Log message" << endl;  // 输出日志信息
   ```

5. **文件输入输出**：

   - 用 `std::ifstream` 从文件读取，使用 `std::ofstream` 向文件写入。

   ```cpp
   ofstream outFile("file.txt");
   outFile << "Hello, file!" << endl;
   outFile.close();
   ```

C++ 标准输入输出通过 `std::cin`、`std::cout`、`std::cerr`、`std::clog` 和文件流类进行数据的读取、输出和日志记录。

### 1.5 `auto`关键字的使用

`auto` 是 C++ 11引入的一种类型推断机制，它可以根据变量的初始化值自动推导出该变量的类型；

使用`auto`的前提是，使用时就清楚知道编译器会给auto推断什么类型；


- **`auto` 只能推断出类型，引用不是类型**

`auto` 默认推断变量的类型，但引用类型无法被自动推断，引用符号 (`&`) 必须显式添加。

 `auto` 会推断出 `y1` 是 `int` 类型，而不是引用类型，因为 `auto` 默认推断对象类型，不能推断引用类型。

- **引用会直接替换为引用指向的对象**

引用本质上是对象的别名，而非独立的对象。`auto` 会直接将引用替换为它指向的对象。

`ref2` 是 `x2` 的引用，而 `auto y2 = ref2;` 会使 `y2` 成为 `x2` 的副本；这说明 `auto` 会根据引用指向的对象的类型来推断类型（即推断为 `int`）。

- **`auto` 在推断类型时忽略值类型的 `const` 修饰符（对指针的影响）**

当使用 `auto` 推断指针类型时，`auto` 会保留指针本身的常量性，但忽略指针指向对象的 `const` 修饰符。

`auto` 推断 `y3` 的类型为 `int*`，而不是 `const int*`。`auto` 会保留指针本身的常量性，但不保留指针指向对象的 `const` 修饰符。这样 `y` 变成了一个指向非 `const` 的 `int*` 指针，虽然原本的 `ptr` 是 `const int*`。

- **`auto` 在推断类型时如果有引用符号，则会保留值类型和指向对象的 `const` 修饰符**

如果在推断类型时使用了引用符号，`auto` 会同时保留值类型和指向对象的 `const` 修饰符。`const auto& y4` 保留了 `ptr4` 的类型，即 `const int*&`，而不会忽略指针指向对象的 `const` 修饰符。`auto&` 会根据指针本身和指针指向对象的类型推断出 `y` 的类型。

--------
​	其实很好理解，在传递值时，修改这个值并不会对原有的值造成影响；而传递引用时，修改这个值会直接对原有的值造成影响。

**代码示例**：

```cpp
#include<iostream>
#include<boost/type_index.hpp>

using boost::typeindex::type_id_with_cvr;


int main(){
    // 1. basic infer
    int x1 = 100;
    auto y1 = x1;
    std::cout << type_id_with_cvr<decltype(y1)>().pretty_name() << "\n";

    // 2.about reference
    int x2 = 100;
    int& ref2 = x2;
    auto y2 = ref2;
    std::cout << type_id_with_cvr<decltype(y2)>().pretty_name() << "\n";


    // 3.about const
    const int x3 = 100;
    const int* const ptr3 = &x3;
    auto y3 = ptr3;
    std::cout << type_id_with_cvr<decltype(y3)>().pretty_name() << "\n";

    // 4.auto& 
    const int x4 = 100;
    const int* const ptr4 = &x4;
    const auto& y4 = ptr4;
    std::cout << type_id_with_cvr<decltype(y4)>().pretty_name() << "\n";

    return 0;
}
```

**运行结果**：

![image-20241210165649858](https://gitee.com/salinoia/image/raw/master/image-20241210165649858.png)

**附`CMakeLists.txt`**

```cmake
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
```

------

**补充**：

- **`auto` 不会影响编译速度，甚至可能提高速度**

使用 `auto` 不会对编译速度产生负面影响，反而可能会加快编译速度。在声明变量时（如 `XX a = b`），当 `XX` 是传统类型时，编译器需要检查 `b` 的类型是否可以转换为 `XX`。然而，当 `XX` 是 `auto` 时，编译器可以直接根据 `b` 的类型推导出 `a` 的类型，因此不需要做类型转换检查。因此，性能差异几乎可以忽略不计，甚至在某些情况下，`auto` 还可能提高编译速度。

- **避免滥用 `auto`**

最重要的一点是不要滥用 `auto`。在使用 `auto` 时，必须确保你明确了解变量的类型。对一些不清楚的地方滥用 `auto`，可能会导致预期之外的结果。使用类型时，应该优先考虑类型安全，以确保代码的可靠性，避免潜在的问题。

- **`auto` 主要用于模板相关代码**

`auto` 最常见的应用场景是在模板相关的代码中。在一些简单变量的情况下，使用模板通常会导致代码的可读性下降，并且缺乏经验的开发者可能会引入安全性问题。在这些情况下，`auto` 可以简化类型声明，使代码更加简洁且易于阅读。然而，如果使用不当，`auto` 可能会使实际类型变得不明确，从而导致混乱或错误。

### 1.6 静态变量，指针和引用

- 在 C++ 中，变量的存储位置主要分为三种：静态变量区、栈区和堆区。

  **静态变量区** 静态变量区是编译时就已经确定了内存地址的位置，主要用于存储全局变量和静态变量。静态变量区中的变量在程序运行时始终存在，生命周期与程序的生命周期相同。

- **指针的存储** 指针本质上是一个变量，它存储的是另一个变量的地址，无论这个指针是存储在栈上还是堆上，它始终有一个地址。指针与普通变量在存储方式上没有区别，都是存储数据的变量。

  - **指针的大小**：在 32 位系统中，`int` 变量和指针的大小通常都是 32 位。指针是一个地址值，它指向某个数据的位置。
  - 指针的操作符
    - `&a`：获取变量 `a` 的地址。
    - `a`：代表变量 `a` 本身的值。
    - `*a`：表示“解引用”，即访问指针 `a` 所指向地址的值。


- **指针的灵活性与副作用** 指针可以灵活地操作内存，比如指针的加减操作会直接影响内存中的地址。因此，指针虽然强大，但也带来了很多副作用。例如，指针可能会导致内存泄漏、悬挂指针等问题。

  **引用的引入**： 为了避免指针的副作用，C++ 引入了引用。引用可以视为“受限的指针”，它本质上是一个“类型`*const`”指针，因此不能修改它指向的地址。引用必须在声明时初始化，且不能为空。

  **引用的特点**：

  - 引用不像指针一样是一个对象，因此编译器不将其视为对象。
  - 操作引用相当于操作引用指向的对象。引用不允许指向空地址，从根本上杜绝了修改内存地址的可能性。

**示例代码**：

```cpp
#include <iostream>

unsigned test(){
    static unsigned callCount = 0;
    return ++callCount;
}

int main(){
    test();
    test();
    test();
    unsigned testFuncCallCount = test();
    std::cout << testFuncCallCount <<std::endl;// 此处会输出4

    int i = 20;
    int* pi = &i;
    *pi = 10;// 直接修改值
    std::cout << pi << std::endl;
    ++pi;
    std::cout << pi << std::endl;

    // 以下两种写法，引用和常量指针在表达效果上可视为“等价”
    int& refI = i;
    int* const pi = &i;

    return 0;
}
```

### 1.7 左值与右值，左值引用与右值引用

1. **左值与右值**

C++11引入了**左值**（Lvalue）和**右值**（Rvalue）的概念，这是理解C++中资源管理、移动语义和性能优化等关键内容的基础；

- **左值（Lvalue）**
  
   - **定义**：拥有地址的对象叫做左值。左值是指可以在内存中持久存在并且可以访问其内存地址的对象。
   
   - **来源**：左值这个概念来自C语言，指的是可以出现在赋值语句等号左边的对象（例如：`a = b` 中的 `a`）。
   
   - **特性**：
   
     - 左值是可以取得地址的，因此你可以对左值使用取地址操作符（`&`）。
   
     - 左值可以出现在赋值语句的**左边**（例如：`i = 10`），但也可以出现在右边（例如：`int i = 10; int j = i;`）。

- **示例**：

```cpp
int i = 10;  // i 是一个左值
int* p = &i; // 可以取 i 的地址
++i; // 此时是一个左值
++i; // 此时是一个右值 
```

- **右值（Rvalue）**

  - **定义**：不是左值的对象就是右值。右值通常是没有持久存在的对象，通常是临时值或字面量值。


  - **特性**：
    - 右值**没有地址**，因此无法对右值进行取地址操作。
    - 右值不能出现在赋值语句的**左边**，因为它没有可修改的内存位置。右值一般只能出现在赋值语句的**右边**。
    


- **示例**：

  ```cpp
  int getValue() { return 10; }
  int x = getValue(); // getValue() 返回的值是右值
  x = 5; // 5 是右值
  ```

2. **引用的分类**

引用在 C++ 中有不同的分类，理解这些引用的不同类型对于掌握 C++11 的新特性，尤其是右值引用、移动语义等非常重要。

(1) **普通左值引用**

- **定义**：普通左值引用是最常见的一种引用类型，它是一个对象的别名，可以引用一个左值（即具有地址的对象）。
- **限制**：普通左值引用只能绑定左值，不能绑定常量对象，也就是说它不能绑定一个 `const` 类型的对象。

**示例**：

```cpp
int a = 10;
int& refA = a;  // 正常绑定左值引用
int& leftRefA = a;
refA = 20;  // 修改 a 的值
```

- 重要的是，普通左值引用无法绑定常量对象。例如，下面的代码是错误的：

  ```cpp
  const int a = 10;
  int& refA = a;  // 错误：不能绑定常量对象
  ```

(2) **`const` 左值引用**

- **定义**：`const` 左值引用是指对常量对象的引用，允许绑定常量对象，同时也允许绑定左值和右值。
- **用途**：`const` 左值引用可以避免不必要的拷贝，特别是对于大型对象或者临时对象时，它提供了一个更加高效的访问方式。

**示例**：

```cpp
int a = 10;
const int& refA = a;  // 正常绑定常量左值引用
refA = 20;  // 错误：不能修改引用的值

const int& refB = 30;  // 也可以绑定右值（常量左值引用可以绑定右值）
```

- 通过 `const` 左值引用，可以绑定左值和右值，但无法修改它所引用的对象的内容。

-  `const`左值引用特别有用，它可以绑定到临时对象（右值）上，并防止不必要的拷贝。例如：

  ```cpp
  const int& rrefC = 100;  // 绑定右值
  ```

(3) **右值引用**

- **定义**：右值引用是 C++11 引入的新特性，用来绑定右值。右值引用是通过 `&&` 来表示的。
- **限制**：右值引用只能绑定右值（临时对象等）。
- **注意**：右值引用在后续章节中会有更深入的讨论，特别是与移动语义和资源管理有关的内容，现阶段只需记住它的语法和用途。

**示例**：

```cpp
int&& rrefD = 10;  // 绑定右值
int&& rightRef = 10;
```

- 右值引用的一个常见用途是“移动语义”，但在这一节中暂时不涉及实际用途。

(4) **万能引用**（后续课程讲解）

- **定义**：万能引用是 C++11 中提出的一种特殊引用类型，通常使用 `T&&` 表示，并且能够绑定左值和右值。这种引用类型通常出现在模板中，可以根据传入的参数类型自动推导出正确的引用类型。
- **应用**：万能引用的用途比较复杂，涉及到模板和完美转发等概念。在本节课中不做深入讲解，等到后续学习模板时会详细介绍。

**示例**（在模板中）：

```cpp
template <typename T>
void func(T&& arg) {
    // 这里的 T&& 是万能引用
}
```

### 1.8 move函数与临时对象

本节是对右值基础的补充，主要讲解右值引用的语法、`move` 函数以及临时对象等概念。

需要注意的是，右值引用的具体应用，特别是在性能优化和资源管理方面，将在第二章的**移动语义**中详细探讨。


1. **`move` 函数**

**`move` 的作用：**

- **右值关注值而非地址**：右值主要代表临时对象或没有持久存储位置的对象，它们的“地址”通常不重要。`move` 函数的作用是将一个左值强制转换为右值，从而让编译器将该对象视作右值。
- **移除左值的地址属性**：`move` 不改变对象的内容，只是通过类型转换，让编译器不再关心其地址，从而允许对该对象进行右值的操作（如资源转移、避免不必要的拷贝等）。调用 `move` 后，原本的左值不再保持其地址属性，**并且之后不能再使用它的地址**；更为直接的说，不再调用这个对象本身。

**`move` 函数的语法：**

​	`std::move` 是一个标准库函数，通常用于将左值转换为右值，从而使对象可以被传递给接受右值的函数（如右值引用或移动构造函数）。

**示例：**

```cpp
#include <iostream>
#include <utility>  // std::move

class MyClass {
public:
    MyClass() { std::cout << "Constructor called\n"; }
    MyClass(const MyClass&) { std::cout << "Copy constructor called\n"; }
    MyClass(MyClass&&) { std::cout << "Move constructor called\n"; }
};

int main() {
    MyClass obj1;  // 调用构造函数
    MyClass obj2 = std::move(obj1);  // 调用移动构造函数
    // 现在 obj1 的状态是未定义的，因此不能继续使用它，直接视为已销毁
}
```

- 解释：
  - `obj1` 被传递给 `std::move`，将其转化为右值引用。
  - 移动构造函数将资源从 `obj1` 转移到 `obj2`，这避免了拷贝。
  - `obj1` 的地址和原始内容已经被“移动”，因此之后不应该再访问它。

**`move` 的注意事项：**

- 使用 `std::move` 后，原始对象的状态可能会变得不确定，因此之后不能继续使用该对象，尤其是不能再取它的地址或进行任何依赖对象状态的操作。
- `move` 只是一个类型转换工具，它本身并不会“移动”数据，实际的资源转移发生在移动构造函数或移动赋值运算符中。

------

2. **临时对象（Right-value Object）**

- **临时对象的概念：**

  - 右值是没有持久存储地址的对象。**临时对象**（Temporary Object）就是一种特殊的右值，它是程序执行过程中生成的中间对象，通常是由表达式产生的。

  - 临时对象的生命周期非常短暂，它们通常在表达式结束后就被销毁。临时对象不具备明确的地址属性，因此它们会作为右值被处理。
  - 临时对象的生命周期通常非常短，它们在表达式结束时被销毁。在 `std::move` 之后，它们的资源就会被转移到新的对象中。


-  **右值引用与临时对象：**

  - 右值引用（`T&&`）的主要作用就是引用这些临时对象，并在后续处理中避免不必要的拷贝。通过右值引用，C++ 可以“窃取”临时对象的资源，将其转移而不再进行复制操作。

  - 右值引用通常会绑定到临时对象，避免了对临时对象的拷贝，提高了程序的效率，特别是在涉及到大对象或复杂资源管理时，具体表现形式为移动构造和资源转移。


**示例：**

```cpp
int&& temp = 10;  // `10` 是一个临时对象，`temp` 是右值引用
int&& temp2 = 5 + 5;  // `5 + 5` 产生一个临时对象，绑定到 `temp2`
```

### 1.9 可调用对象

在 C++ 中，如果一个对象能够使用调用运算符 `()` 并且可以传递参数，那么这个对象就是一个**可调用对象**。这个概念非常重要，在后续的 C++ 编程中会反复使用，尤其是在函数指针、仿函数、lambda 表达式等场景中。

------

**可调用对象的分类**

1. **函数**：
- 函数本身就是最典型的可调用对象。它可以直接通过 `()` 运算符来调用并传递参数。
2. **仿函数**：
- 仿函数是具有 `operator()` 成员函数的类对象。当类实现了 `operator()` 后，类的对象就可以像函数一样被调用。因此这些对象也可以作为可调用对象。
3. **Lambda 表达式**：

  - **Lambda 表达式非常重要，它是现代 C++ 中常用的工具，能够极大提高代码的简洁性和可读性。**

    - Lambda 表达式是 C++11 引入的一个特性，允许我们定义匿名函数，通常用于简化代码。Lambda 表达式可以在需要的地方直接定义函数，而无需单独定义一个函数。

    - Lambda 的基本格式为 `[] {}`，也可以指定参数、返回类型等。

   - **完整的 Lambda 表达式语法**：

     ```cpp
     [] () -> return_type { /* 函数体 */ }
     ```

   - **Lambda 的组成部分：**

     1. **`[]` 捕获列表**：表示 lambda 表达式可以访问外部变量。
        - `[]` 表示不捕获任何变量。
        - `[=]` 按值捕获所有外部变量。
        - `[&]` 按引用捕获所有外部变量。
        - `[=, &i]` 按值捕获所有外部变量，`i` 以引用捕获。
        - `[&i]` 只捕获 `i` 以引用方式，其他变量按值捕获。
        - `[i]` 只捕获 `i` 以值捕获。
        - `[&i]` 只捕获 `i` 以引用捕获。
     2. **`() 参数列表`**：指定 lambda 的参数，和普通函数一样。
     3. **`-> ret` 返回类型**：指定返回类型，如果不指定，C++ 会自动推导。
     4. **`{}` 函数体**：实际的代码执行部分，与普通函数相同。

   - **示例代码：**

     ```cpp
     #include<iostream>
     #include<functional>
     
     void test(int i){
         std::cout << i << "\n";
         std::cout << "Hello world.\n" << std::endl; 
     }
     
     class Test{
     public:
         void operator()(int i){
             std::cout << i << "\n";
             std::cout <<  "Here is a functor.\n" << std::endl;
         }
     };
     
     using fp_type = void(*)(int); // 函数指针
     using func_type = std::function<void(int)>;
     
     void myFunc0(fp_type fp, int i){
         fp(i);
     }
     
     void myFunc1(func_type func, int i){
         func(i);
     }
     
     int main(){
         // 可调用对象
         // 1.最主要用法：为另一个函数传递参数
         myFunc0(test, 200);
     
         // 2.仿函数
         Test t;
         t(100);
     
         // 3.Lambda表达式
         int i = 10;
         int j = 20;
     
         // 基础用法
         auto lambda = [&i](int elem) -> int{// 捕获引用，修改变量本身
             i = 30;
             std::cout << elem << "\n";
             std::cout << "Here is a basic lambda expression.\n" << std::endl;
             return elem + i;
         };
     
         std::cout << lambda(200) << std::endl;
     
         // 常见用法：作为函数参数传递
         myFunc0([](int elem){
             std::cout << elem << std::endl;
             // std::cout << j << std::endl;// 错误，函数指针强制要求捕获列表为空
             std::cout << "Function Pointer Lambda. \n" << std::endl;
         }, 200);
     
         myFunc1([i](int elem){
             std::cout << elem << std::endl;
             std::cout << i << std::endl; // 引入C++11的std::function，能够灵活地接受任何符合签名的可调用对象
         }, 200);
     }
     ```

## 2.类

### 2.1 类介绍，构造函数及析构函数

**类的介绍**：

**面向对象 vs 面向过程：**

- **面向过程**：是按照计算机的工作逻辑编写程序，关注的是数据流和控制流；C 语言是典型的面向过程语言，它直接对应到汇编语言，汇编则与硬件紧密联系。

- **面向对象**：是按照人类的思维方式来编码，着重描述现实世界的对象及其行为。C++ 完全支持面向对象，允许开发者通过定义类来模拟现实世界的事物和它们之间的交互。

**举个例子：**

- **面向对象的思想**：要将大象装进冰箱，可以分为三步：打开冰箱、放入大象、关上冰箱。这里，“人”和“冰箱”是对象。人具有“发出指令”的能力，冰箱具有“执行开关门”的能力。这些行为通过类的方法进行描述。
- **面向过程的思维**：如果用 C 语言来模拟，我们只能通过结构体定义“人”和“冰箱”对象，每个对象包含相应的属性（如人的“手”属性和冰箱的“门”属性），然后使用函数来实现这些动作。这就是面向过程的方式——把功能拆解成一个个独立的函数。

​	**类是面向对象编程（`OOP`）的基石**，它帮助我们将现实世界的对象和它们的行为封装起来，避免了面向过程编程中大量不必要的函数调用。

------

**构造函数**：

- **构造函数的定义和作用**：

  - 构造函数是类中一种特殊的成员函数，它在对象创建时自动调用，用来初始化对象的成员变量（**`RAII`准则**）。

  - 构造函数的目的是保证每个对象在创建时都有一个有效的初始状态。C++ 允许通过构造函数自定义初始化方式。

- **构造函数的类型：**

1. **普通构造函数**：用于初始化对象，通常带有参数，用于为对象的成员变量赋值。
2. **拷贝构造函数**：用于通过另一个对象来初始化新对象。即通过已有对象复制数据到新对象。
3. **移动构造函数**：用于通过将另一个对象的资源“移交”给新对象来初始化。
4. **默认构造函数**：如果没有定义任何构造函数，C++ 会自动为类生成一个默认构造函数，该构造函数什么也不做，对象成员不会被初始化

- 构造函数是类对象创建时**必须**调用的函数，用于初始化对象；即使没有显式定义构造函数，编译器会提供一个默认构造函数。

------

- **析构函数**：

  - **析构函数的定义和作用**：

    - 析构函数是类的一种特殊成员函数，当对象生命周期结束时自动调用。它通常用于释放对象占用的资源（例如动态分配的内存、文件句柄等）。

    - 栈上的对象会在其作用域结束时被销毁，调用析构函数。

    - 堆上的对象需要通过 `delete` 显式释放内存，析构函数会在释放内存时自动调用。

- 析构函数用于在对象销毁时执行清理工作，释放资源等。几乎所有类都应该写析构函数，但并非每个类都需要显式地定义析构函数。

- 如果类没有显式定义析构函数，编译器会生成一个默认析构函数。在大多数情况下，如果类没有管理动态资源，可以不定义析构函数。

**代码示例：**

```cpp
#include<iostream>

struct TestStruct
{
    /*
        C语言的特点是：
        结构体对象声明的是位于栈区和堆区的
        而函数的声明则是严格位于代码区  
        单从汇编的角度来看是无法解决的
    */
    int i;
    int j[20];
    int *k;
};

void initialize(TestStruct test){
    test.k = (int*)malloc(test.i * sizeof(int));
}

class TestClass{
public:
    //默认构造函数
    TestClass(){};
    // 普通构造函数
    TestClass(int i_, int j_, int k_):i(i_), j(j_), pi(new int(k_)){}
    int get_integer_i(){
        return this->i;
    }
    // 拷贝构造函数
    TestClass(const TestClass& CppTest): i(CppTest.i), j(CppTest.j), pi(new int(*CppTest.pi)){}

    // 移动构造函数

    /*
        暂略
    */

   // 动态申请内存的情况必须显式析构；
   // 网络协议栈、文件等同理
   ~TestClass(){
    delete this->pi;
   }

private:
    int i;
    int j;
    int *pi;
};

int main(){
    TestStruct CTest;
    TestClass CppTest0;
    TestClass CppTest1(1, 2);
    TestClass CppTest2(CppTest0);


    std::cout << CTest.i << "\n";// 此时调用实际上是未定义行为，出来的值一定是未知的
    std::cout << CppTest0.get_integer_i() << std::endl;
    std::cout << CppTest1.get_integer_i() << std::endl;
    std::cout << CppTest2.get_integer_i() << std::endl;
    return 0;
}// 当类生命周期结束时，析构函数会被调用
```

### 2.2 this，常成员函数与常对象

**this关键字**：

`this` 是 C++ 中的一个关键字，**编译器**将其解释为**指向调用成员函数的对象的指针**；

在C++中，每一个成员函数实际上接受一个额外的隐式参数，即`this`指针：

- **成员函数调用**：在调用成员函数如`obj.display()`时，编译器会将其转化为`display(&obj)`，其中`&obj`即是`this`指针；
- **成员变量访问**：访问成员变量如 `value` 被编译器转换为 `this->value`，即通过 `this` 指针访问对象的成员；

在现代编译器中，许多成员函数会被内联，这意味着函数的代码被直接插入到调用点，`this` 指针的使用可能被优化掉，编译器可能将 `obj.getValue()` 直接编译为访问 `obj.value`，无需显式使用`this`指针；

对于虚函数，`this` 指针在运行时指向正确的对象实例，以支持多态性：

```cpp
class Base {
public:
    virtual void show() { std::cout << "Base" << std::endl; }
};

class Derived : public Base {
public:
    void show() override { std::cout << "Derived" << std::endl; }
};

Base* obj = new Derived();
obj->show(); // 输出 "Derived"
```

**常成员函数与常对象**：

在大型C++项目中，**合理使用`const`关键字是提高代码安全性、可维护性和可读性的有效手段**，`const`通过防止意外修改数据，明确接口意图，提供编译器优化机会，以及促进良好的编程习惯，减少潜在错误，确保多人协作时的数据一致性和接口契约的明确性

- **常成员函数**：指在函数声明末尾加上`const`关键字的成员函数，这类函数承诺在执行过程中不会修改对象的任何成员变量。可以将其理解为将`this`指针转换为指向常对象的指针（即`const ClassName* this`），从而禁止在函数内部对成员变量进行修改。

- **常对象**：通过在对象声明时使用`const`关键字修饰的对象。一旦定义为常对象，其成员变量的值在初始化后便无法更改，确保了对象状态的不可变性。在大型程序中，常对象具有重要意义，因为它们提供了一种可靠的方式来保护关键数据不被意外修改，减少了潜在的错误风险。此外，常对象有助于优化程序性能，编译器可以更有效地进行优化处理，因为它们知道这些对象的状态不会改变。

```cpp
class Example {
public:
    void display() const { // 常成员函数
        // value = 10; // 错误：无法修改成员变量
        std::cout << value << std::endl;
    }

    void setValue(int val) {
        value = val;
    }

private:
    int value;
};

int main() {
    const Example obj; // 常对象
    obj.display();
    // obj.setValue(5); // 错误：无法调用非常成员函数
    return 0;
}
```

**常成员函数特性**：

① **常成员函数无法调用非常成员函数**

- **原因**：常成员函数承诺不修改对象状态，而非常成员函数可能会修改成员变量，违背`const`承诺。
- **结果**：编译器会阻止常成员函数调用非常成员函数，确保对象的不可变性。

② **尽量使用常成员函数以减少出错**

- **优势**
  - **提高代码安全性**：防止意外修改对象状态。
  - **增强可读性**：明确函数的意图，便于理解与维护。
  - **优化编译器性能**：编译器可进行更多优化，如内联和常量折叠。
- **最佳实践**：对于不会修改对象状态的成员函数，优先声明为常成员函数。

③ **同名常成员函数与非常成员函数的重载**

- **允许重载**：可以在同一类中根据`const`修饰符重载成员函数。

- **调用优先级**

  - **常量对象**：优先调用常成员函数，确保对象的只读性质。
  - **非常量对象**：优先调用非常成员函数，如不存在则调用常成员函数。

- **代码示例**

  ```cpp
  class Example {
  public:
      void display() const { /* 常成员函数实现 */ }
      void display() { /* 非常成员函数实现 */ }
  };
  
  const Example constObj;
  Example nonConstObj;
  
  constObj.display();    // 调用常成员函数
  nonConstObj.display(); // 调用非常成员函数
  ```

**常对象特性**：

① **常对象不能调用非常成员函数**

- **原因**：常对象（`const`对象）承诺其状态在创建后不会被修改，因此只能调用那些保证不修改对象状态的常成员函数。

- **机制**：非常成员函数可能修改对象的成员变量，与常对象的只读性质相冲突。编译器通过类型检查机制，阻止常对象调用非常成员函数，以维护`const`契约。

- **代码示例**：

  ```cpp
  cpp复制代码class Example {
  public:
      void display() const { /* 常成员函数实现 */ }
      void modify() { /* 非常成员函数实现 */ }
  };
  
  int main() {
      const Example constObj;
      constObj.display(); // 合法
      // constObj.modify(); // 错误：无法调用非常成员函数
      return 0;
  }
  ```

② **常成员函数确保对象状态的不变性**

- **数据保护**：在大型程序中，很多对象在创建后其状态不再需要改变。将这些函数声明为常成员函数，可以防止意外修改，确保数据的一致性和可靠性。

- **接口稳定性**：常成员函数定义了对象的只读接口，使得接口更加稳定，便于其他模块或组件依赖和调用。

- **优化机会**：编译器可以利用`const`信息进行更多优化，如缓存、内联展开等，提高程序性能。

- **代码示例**

  ```cpp
  class Configuration {
  public:
      std::string getSetting() const { return setting; } // 常成员函数
      void setSetting(const std::string& newSetting) { setting = newSetting; } // 非常成员函数
  
  private:
      std::string setting;
  };
  
  void initialize(const Configuration& config) {
      std::cout << config.getSetting(); // 安全调用常成员函数
      // config.setSetting("NewValue"); // 错误：无法调用非常成员函数
  }
  ```

### 2.3 inline, mutable, default, delete

**`inline`关键字**

普通函数在调用时需要给函数分配栈空间以供函数执行，压栈等操作会影响成员运行效率，于是C++提供了内联函数将函数体放到需要调用函数的地方，用空间换效率；随着处理器性能和缓存容量的提升，函数调用开销相对减小，**内联对性能的影响有所下降**；现代编译器具备更高级的内联决策能力，如基于分析的自动内联，减少了对程序员手动干预的需求，过度内联可能导致代码膨胀，影响可读性和维护性，因此在现代开发中，`inline`的使用更加谨慎和有限，其使用场景和频率在大幅降低减少。

**① `inline`关键字的定义与作用**

- **定义**：

  - 在函数声明或定义中，将`inline`关键字置于返回类型之前，以建议编译器将该函数内联展开。
  - `inline`关键字必须与函数的定义同时出现，单独在声明中使用不会使函数成为内联函数。

- **作用**：

  - **内联扩展**：建议编译器在每个调用点替换函数体，减少函数调用的开销（如栈操作和跳转指令）。
  - **优化执行效率**：适用于小型、频繁调用的函数，通过消除调用开销提升运行速度。
  - **避免链接时重复定义**：内联函数通常在头文件中定义，允许多个翻译单元使用而不违反“一次定义规则”（ODR）。

- **代码示例**：

  ```cpp
  // 内联函数定义
  inline int add(int a, int b) {
      return a + b;
  }
  
  // 错误用法：仅在声明中使用inline不会使函数内联
  inline int multiply(int a, int b); // 仅声明，不内联
  
  // 正确用法：在定义中使用inline
  inline int multiply(int a, int b) {
      return a * b;
  }
  ```

**② `inline`关键字的实现机制与注意事项**

- **实现机制**：

  - **编译器优化决策**：`inline`仅为编译器提供内联建议，实际是否内联由编译器根据函数的复杂度、大小、调用频率等因素决定。
  - **隐式参数优化**：内联函数在调用点展开代码，避免了函数调用的隐式`this`指针传递和栈帧分配。

- **注意事项**：

  - 空间与时间权衡
    - **优点**：减少函数调用开销，提高执行效率，适用于小型、性能关键的函数。
    - **缺点**：增加二进制代码体积，可能导致指令缓存不命中率上升，尤其在内联大型函数时影响性能。
  - 递归函数与虚函数
    - **递归函数**：由于调用深度不确定，通常不适合内联，编译器一般不会内联递归函数。
    - **虚函数**：需要动态绑定，内联内虚函数调用较为复杂，编译器通常不会内联虚函数。
  - 调试与可维护性
    - 过度使用`inline`可能导致代码膨胀，增加调试难度和维护复杂性。

- **代码示例**：

  ```cpp
  class Calculator {
  public:
      // 常见内联函数
      inline int square(int x) {
          return x * x;
      }
  
      // 递归函数，不适合内联
      inline int factorial(int n) {
          if (n <= 1) return 1;
          return n * factorial(n - 1); // 编译器通常不会内联
      }
  
      // 虚函数，不适合内联
      virtual inline void display() {
          std::cout << "Calculator Display" << std::endl;
      }
  };
  ```

**③ `inline`关键字的最佳实践**

- **最佳实践**：

  - 适用场景
    - **小型、频繁调用的函数**：如访问器（getter）和设置器（setter）等简单函数。
    - **模板函数**：由于模板函数通常在头文件中定义，编译器倾向于内联以优化性能。
  - 避免滥用
    - 不适合内联复杂或大型函数，以防止代码膨胀和性能下降。
    - 避免在递归函数和需要动态绑定的虚函数中使用`inline`。
  - 依赖编译器优化
    - 现代编译器具备智能的内联优化能力，通常可以自行决定内联策略，**程序员应更多依赖编译器的优化而非手动标记`inline`**。

- **代码示例**：

  ```cpp
  // 模板函数通常在头文件中定义，编译器倾向于内联
  template <typename T>
  inline T max(T a, T b) {
      return (a > b) ? a : b;
  }
  
  // 使用inline优化简单的getter函数
  class Point {
  public:
      inline int getX() const { return x; }
      inline int getY() const { return y; }
  
      void setX(int newX) { x = newX; }
      void setY(int newY) { y = newY; }
  
  private:
      int x, y;
  };
  
  int main() {
      Point p;
      p.setX(10);
      p.setY(20);
      std::cout << "X: " << p.getX() << ", Y: " << p.getY() << std::endl;
      return 0;
  }
  ```

------

**`mutable`关键字**

**① `mutable` 的作用**

- **概念**：`mutable` 使得类的成员变量可以在 `const` 成员函数中被修改。
- 主要用途
  - **统计信息**：记录函数调用次数或对象访问次数。（**这可能是`mutable`关键字唯一的用处了**）
  - **缓存机制**：在 `const` 成员函数中存储临时计算结果。
  - **日志记录**：记录日志信息。
- **与 `const` 的关系**：通常与 `const` 成员函数配合使用，用于在不改变对象外部状态的情况下修改内部状态。

**示例代码**：

```cpp
#include <iostream>

class Example {
public:
    Example(int val) : value(val), accessCount(0) {}

    int getValue() const {
        accessCount++; // 允许修改 mutable 成员
        return value;
    }

    int getAccessCount() const { return accessCount; }

private:
    int value;
    mutable int accessCount;
};

int main() {
    const Example ex(42);
    ex.getValue();
    ex.getValue();
    std::cout << "Access count: " << ex.getAccessCount() << std::endl; // 输出: 2
}
```

**② 使用注意事项**

- 设计谨慎
  - `mutable` 是一种权宜之计，仅在确实必要时使用，过度使用可能反映设计不良。
- 限制
  - 不能修饰 `static` 成员变量。
  - 不能修饰 `const` 成员变量。
- 代码维护性
  - 滥用 `mutable` 可能**导致代码难以理解**，尤其是在团队协作中。

**示例代码**（注意事项）：

```cpp
class Sample {
public:
    void modify() const {
        count++; // mutable 成员变量可修改
        // value++; // 错误：非 mutable 成员变量不能修改
    }

private:
    int value = 0;
    mutable int count = 0;
};
```

------

**`default`关键字**

在现代C++中，为代码提供更好的直观性和维护性，**推荐用`default`显式标记需要使用默认实现的函数**；

- **作用**：
  - 显式指定使用编译器生成的默认函数（构造函数、复制构造函数、赋值运算符、析构函数等）。
  - 避免意外遗漏默认函数的生成，提升代码的可读性和直观性。
- **优点**：
  - **简化代码**：显式声明默认函数，避免手动实现无意义的函数。
  - **代码直观**：即使没有默认构造函数，也可以通过`default`显式标记，明确使用系统提供的默认实现。
- **推荐实践**：
  - 在现代C++中，即使类没有自定义构造函数，也建议将默认函数用`default`标记，以增强代码的可读性和可维护性。

**代码示例**：

```cpp
#include <iostream>

class Example {
public:
    Example() = default;                // 默认构造函数
    ~Example() = default;               // 默认析构函数
    Example(const Example&) = default;  // 默认复制构造函数
    Example& operator=(const Example&) = default; // 默认赋值运算符
};

int main() {
    Example ex1;
    Example ex2 = ex1; // 调用默认复制构造函数
    ex2 = ex1;         // 调用默认赋值运算符
    return 0;
}
```

------

`delete`关键字

**① `delete`关键字的作用**

1. **禁止默认生成的特殊函数**：
   - C++ 会自动生成默认构造函数、复制构造函数、赋值运算符等特殊函数。在某些情况下，不希望编译器自动生成这些函数，可通过`= delete`显式禁止其生成。
2. **代替传统方式**：
   - 在 C++11 之前，为避免生成某些函数，通常将这些函数声明为私有或仅声明而不定义。这种方法麻烦且不直观。C++11 引入了`delete`关键字，可以更加清晰地表达“不允许使用此函数”的意图。
3. **用法**：
   - 禁止默认构造函数、复制构造函数、赋值运算符等生成。
   - 禁止不合理的函数调用（如类型转换、特定参数的函数调用）。

------

**② 示例代码**

**1. 禁止默认特殊函数**：

```cpp
#include <iostream>

class NonCopyable {
public:
    NonCopyable() = default;                        // 允许默认构造
    NonCopyable(const NonCopyable&) = delete;       // 禁止复制构造
    NonCopyable& operator=(const NonCopyable&) = delete; // 禁止赋值运算符
};

int main() {
    NonCopyable obj1;
    // NonCopyable obj2 = obj1; // 错误：复制构造函数被删除
    // obj2 = obj1;            // 错误：赋值运算符被删除
    return 0;
}
```

**2. 禁止不合理的函数调用**：

```cpp
#include <iostream>

class Example {
public:
    void func(int x) = delete;       // 禁止以整数调用
    void func(const char* str) {     // 允许以字符串调用
        std::cout << "String: " << str << std::endl;
    }
};

int main() {
    Example ex;
    // ex.func(42);                  // 错误：函数以整数调用被删除
    ex.func("Hello");                // 正确
    return 0;
}
```

**3. 在单例模式中使用**：

单例类通常不允许用户显式销毁其实例。通过将析构函数标记为`delete`，可以避免程序员错误地销毁单例对象。

**禁用析构函数的意义**：

- 禁止对象的销毁操作，防止程序员无意中删除某些不应被删除的对象（如单例模式的对象或具有特殊管理机制的对象）。
- 禁用析构函数可以保护类实例，确保其生命周期受到严格控制，但**只有在特殊生命周期下会使用`delete`标记析构函数**。

```cpp
#include <iostream>

class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance; // 局部静态变量
        return instance;
    }

    Singleton(const Singleton&) = delete;            // 禁用复制构造
    Singleton& operator=(const Singleton&) = delete; // 禁用赋值运算符

    ~Singleton() = delete; // 禁用析构函数

private:
    Singleton() {} // 构造函数私有化
};

int main() {
    Singleton& s = Singleton::getInstance();

    // delete &s; // 编译错误：析构函数被禁用
    return 0;
}
```

### 2.4 友元类与友元函数

**① 友元的定义与作用**

- **友元的定义**：
  - 友元（`friend`）是一种特殊的声明，允许指定的函数或类访问另一个类的私有成员或保护成员。
  - 友元可以是普通函数、类、或者类的成员函数。
- **友元的主要用途**：
  - 在某些场景下，简化访问私有成员的操作，避免额外的接口设计。
  - 必须使用友元功能的场景包括某些**运算符重载**（如`<<`和`>>`）。

------

**② 注意事项**

1. **破坏封装性**：
   - 友元会破坏类的封装性，因为它允许外部函数或类直接访问私有成员。
   - 在面向对象设计中，封装是重要的设计原则，友元的使用可能会违背这一原则，因此通常不推荐广泛使用。
2. **推荐替代方案**：
   - 通常，可以通过定义适当的**公共接口函数**解决访问问题，避免使用友元。
3. **友元的实际用途**：
   - **运算符重载**：某些运算符（如`<<`、`>>`等）需要访问类的私有成员，这种场景下友元是必要的。

------

**③ 示例代码**

**1. 基本用法**：

```cpp
#include <iostream>

class MyClass {
private:
    int value;

public:
    MyClass(int val) : value(val) {}

    // 声明普通函数为友元
    friend void printValue(const MyClass& obj);
};

void printValue(const MyClass& obj) {
    std::cout << "Value: " << obj.value << std::endl; // 直接访问私有成员
}

int main() {
    MyClass obj(42);
    printValue(obj); // 输出: Value: 42
    return 0;
}
```

**2. 友元类**：

```cpp
#include <iostream>

class MyClass {
private:
    int value;

public:
    MyClass(int val) : value(val) {}

    // 声明类为友元
    friend class FriendClass;
};

class FriendClass {
public:
    void displayValue(const MyClass& obj) {
        std::cout << "Value: " << obj.value << std::endl; // 访问私有成员
    }
};

int main() {
    MyClass obj(42);
    FriendClass friendObj;
    friendObj.displayValue(obj); // 输出: Value: 42
    return 0;
}
```

**3. 运算符重载（友元的常见和主要用途）**：

```cpp
#include <iostream>

class MyClass {
private:
    int value;

public:
    MyClass(int val) : value(val) {}

    // 声明运算符为友元
    friend std::ostream& operator<<(std::ostream& os, const MyClass& obj);
};

std::ostream& operator<<(std::ostream& os, const MyClass& obj) {
    os << "Value: " << obj.value; // 直接访问私有成员
    return os;
}

int main() {
    MyClass obj(42);
    std::cout << obj << std::endl; // 输出: Value: 42
    return 0;
}
```

### 2.5 重载运算符

很多时候我们想让类对象也能像基础类型的对象一样进行作基础操作，比如“+”，“-”，“*”，“\”，也可以使用某些运算符“=”，“()”，“[]”,“<<”，“>>”。但是一般的类即使编译器可以识别这些运算符，类对象也无法对这些运算符做出应对，我们必须对类对象定义处理这些运算符的方式；C++提供了定义这些行为的方式，就是“operator 运算符”来定义运算符的行为，`operator`是一个关键字，C++重载运算符不能改变运算符的元数。

重载运算符在 C++ 中非常重要，它能让用户自定义的类具备与内建类型相同的操作性，**尤其是在STL模板编程时**，提升代码的可读性和功能性。

- **一元运算符重载**

  一元运算符作用于单个操作数，常见重载的一元运算符包括：

  `++`	`--`	`[]`	`()`

```cpp
#include<stdexcept>
class vector2{
private:
    float x, y;
public:
    vector2(float x, float y) :x(x), y(y){}

    // 前置自增 (++vector2)
    vector2& operator++() {
        ++x;
        ++y;
        return *this;
    }

    // 后置自增 (vector2++)
    vector2 operator++(int) {
        vector2 temp = *this;  // 备份当前对象
        ++(*this);             // 调用前置自增
        return temp;           // 返回备份的对象
    }

    // 前置自减 (--vector2)
    vector2& operator--() {
        --x;
        --y;
        return *this;
    }

    // 后置自减 (vector2--)
    vector2 operator--(int) {
        vector2 temp = *this;  // 备份当前对象
        --(*this);             // 调用前置自减
        return temp;           // 返回备份的对象
    }    
    // 非const版本的下标操作符
    float& operator[](int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // const版本的下标操作符
    const float& operator[](int index) const {// 返回常量引用，禁止修改 x 或 y
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // 非const版本的圆括号操作符
    float& operator()(int index) {
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }

    // const版本的圆括号操作符
    const float& operator()(int index) const {// 返回常量引用，禁止修改 x 或 y
        if (index == 0) return x;
        else if (index == 1) return y;
        else throw std::out_of_range("Index out of range");
    }
};
```

- **二元运算符重载**

  二元运算符是作用于两个操作数的运算符，在操作数之间进行计算或赋值操作

  -  `+`，`-`，`*`，`/`

  - `>`，`<`，`==`

  -  `<<`，`>>`

```cpp
#include<iostream>
class vector2{
private:
    float x, y;
public:
    vector2(float x, float y) :x(x), y(y){}
    // vector2 add(const vector2& other){
    //     // 做一些额外的操作，比如日志记录、检查等
    //     return vector2(x + other.x, y + other.y);
    // }

    vector2 operator+(const vector2& other){
        return vector2(x + other.x, y + other.y); // 不做其余操作即可直接重载
        // return add(other);
    }

    bool operator==(const vector2& other) const{
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const vector2& other) const{
        // return !(operator==(other)); // That's weird
        return !(*this == other);
    }

    // 赋值运算符会默认进行重载
    vector2& operator=(const vector2& other){
        if(this == &other) return *this;
        x = other.x;
        y = other.y;
        return *this;
    }
    
    friend std::ostream& operator<<(std::ostream& os, const vector2& vec){
        return os << vec.x << ", " << vec.y;
    }
    friend std::istream& operator>>(std::istream& is, vector2& vec){
        is >> vec.x >> vec.y;
        return is;
    }
    
};
```

### 2.6 普通继承及其实现原理

C++非继承的类相互是没有关联性的，继承可以将具有共有特性的类作为父类，然后将子类通过继承的方式进行封装，例如一个游戏的不同枪类武器、雷达不同波形参数的设计；

**公有继承**是最常用的继承方式，表示“是一个”关系，允许子类继承父类的公有和保护成员。

**保护继承**和**私有继承**较少使用，主要用于一些特殊的需求：

- **保护继承**：用于在子类中访问父类的成员，但不暴露给外部。
- **私有继承**：表示实现关系，用于复用基类的功能但不让外部访问基类接口。

```cpp
#include <string>
#include <vector>
#include <iostream>
#include <memory>
#include <typeinfo>

// 定义元素效果策略接口
class ElementEffect {
public:
    virtual void applyEffect() const = 0;
    virtual ~ElementEffect() {}
};

// 具体元素效果
class FireEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Fire effect!" << std::endl;
    }
};

class IceEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Ice effect!" << std::endl;
    }
};

class LightningEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Lightning effect!" << std::endl;
    }
};

class WindEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Wind effect!" << std::endl;
    }
};

class EarthEffect : public ElementEffect {
public:
    void applyEffect() const override {
        std::cout << "Applying Earth effect!" << std::endl;
    }
};

// 基类 Weapon
class Weapon {
private:
    std::string name;
    double damage;
protected:
    std::unique_ptr<ElementEffect> elementEffect;
public:
    // Modify constructor to accept unique_ptr and transfer ownership
    Weapon(const std::string& weaponName, double dmg, std::unique_ptr<ElementEffect> elemEffect)
        : name(weaponName), damage(dmg), elementEffect(std::move(elemEffect)) {}

    virtual ~Weapon() {}

    virtual void attack() const = 0;

    std::string getName() const { return name; }
    double getDamage() const { return damage; }
};

// 派生类 Bow
class Bow : public Weapon {
private:
    int range; // 弓箭的射程
public:
    Bow(const std::string& bowName, double dmg, std::unique_ptr<ElementEffect> elemEffect, int rng)
        : Weapon(bowName, dmg, std::move(elemEffect)), range(rng) {}

    void attack() const override {
        std::cout << "Shooting an arrow with " << getDamage() << " damage. ";
        elementEffect->applyEffect();// 基类指针调取子类函数，实现动态多态绑定
        std::cout << " Range: " << range << " meters." << std::endl;
    }

    int getRange() const { return range; }
};

// 派生类 Spear
class Spear : public Weapon {
private:
    double thrustSpeed; // 长枪的刺击速度
public:
    Spear(const std::string& spearName, double dmg, std::unique_ptr<ElementEffect> elemEffect, double speed)
        : Weapon(spearName, dmg, std::move(elemEffect)), thrustSpeed(speed) {}// 子类的构造函数必须先调用父类的构造函数

    void attack() const override {
        std::cout << "Thrusting with a spear dealing " << getDamage() << " damage. ";
        elementEffect->applyEffect();
        std::cout << " Thrust speed: " << thrustSpeed << " m/s." << std::endl;
    }

    double getThrustSpeed() const { return thrustSpeed; }
};

// Test function
void testWeaponsOptimized() {
    // Create weapons by transferring ownership of ElementEffect
    std::unique_ptr<Weapon> weapon1 = std::make_unique<Bow>("Longbow", 50.0, std::make_unique<WindEffect>(), 150);
    std::unique_ptr<Weapon> weapon2 = std::make_unique<Spear>("Fire Spear", 75.0, std::make_unique<FireEffect>(), 30.5);
    std::unique_ptr<Weapon> weapon3 = std::make_unique<Bow>("Ice Bow", 60.0, std::make_unique<IceEffect>(), 120);
    std::unique_ptr<Weapon> weapon4 = std::make_unique<Spear>("Earth Spear", 80.0, std::make_unique<EarthEffect>(), 28.0);

    // Store in a container of unique_ptrs
    std::vector<std::unique_ptr<Weapon>> weapons;
    weapons.push_back(std::move(weapon1));
    weapons.push_back(std::move(weapon2));
    weapons.push_back(std::move(weapon3));
    weapons.push_back(std::move(weapon4));

    // Iterate and perform attacks
    for(const auto& weapon : weapons) {
        weapon->attack();
    }

    // Example type conversion
    std::unique_ptr<Weapon> base = std::make_unique<Spear>("Lightning Spear", 85.0, std::make_unique<LightningEffect>(), 32.0);
    base->attack();

    std::cout << std::string(typeid(*base).name()) << std::endl;

    // Attempt to cast to Spear
    Spear* spear = dynamic_cast<Spear*>(base.get());
    if(spear) {
        std::cout << "Successfully casted to Spear: " << spear->getName() << std::endl;
    } else {
        std::cout << "Failed to cast to Spear." << std::endl;
    }

    // No need to manually delete; unique_ptr handles it
}

int main() {
    testWeaponsOptimized();
    return 0;
}
```

**继承实现原理**：

**构造父类部分**：当创建一个子类对象时，首先调用父类的构造函数。如果父类有默认构造函数，它会首先被调用。即**使在子类中没有明确调用父类构造函数，编译器也会默认调用父类的构造函数（如果没有特殊指定）**，如果父类的默认构造函数被禁用，且子类实例化时未调用父类构造函数，则对象无法实例化；在父类构造函数执行时，父类的成员变量会被初始化。无论父类的成员变量是公有（public）、保护（protected）还是私有（private），它们在父类的构造函数中都会被初始化，但**只有父类的成员函数或友元函数能够访问父类的私有成员**；接着，执行子类的构造函数，初始化子类自己的成员变量；**析构函数则是先析构子类再析构父类**。

### 2.7 多态，虚函数及其实现原理，override关键字

**多态（Polymorphism）**是面向对象编程的一个重要特性，它允许通过基类（Base）指针或引用来调用派生类（Derived）的函数，从而实现不同类的对象以相同接口表现出不同的行为。多态有两种形式：**静态多态**和**动态多态**。

- 静态绑定：程序在编译时就已经确定了函数的地址，比如非虚函数就是静态绑定。
- 动态绑定：程序在编译时确定的是程序寻找函数地址的方法，只有在程序运行时才可以真正确定程序的地址，比如虚函数就是动态绑定。

当父类指针指向子类对象，且子类重写父类某一函数时。父类指针调用该函数，就会产生以下的可能：

- **该函数为虚函数：父类指针调用的是子类的成员函数；**
- **该函数不是虚函数：父类指针调用的是父类的成员函数；**

- **子父类的虚函数申明必须完全相同，且最好显式申明虚函数**，为防止开发人员误写，C++11增加了`override`关键字以检验；
- **父类的析构函数必须是虚函数，当父类指针指向子类对象时，若不虚析构会造成严重的内存泄漏。**

代码示例如上节，其中`ElementEffect`基类与各元素派生类的实现可以完美体现出动态多态以及纯虚函数的作用。

--------

**虚函数是如何实现动态绑定的**？

**虚函数表（vtable）**：

- 每个包含虚函数的类由编译器生成一个虚函数表，存储该类所有虚函数的地址指针。
- 每个派生类拥有独立的vtable，继承并可能重写基类的虚函数指针。

**虚指针（vptr）**：

- 每个对象包含一个隐藏的虚指针，指向所属类的vtable。
- vptr通常位于对象内存布局的最前端，确保在运行时能够访问正确的vtable。

**内存布局**：

- **vtable布局**：由函数指针数组组成，按照虚函数的声明顺序排列，支持单继承和多继承的虚函数重写。
- **对象布局**：包含vptr及类的其他数据成员，vptr指向对应的vtable，实现虚函数的动态调用。

------

**纯虚函数：强制性要求派生类实现功能，无法实例化，但构造函数与析构函数依然不能省略。**

### 2.8 类的静态成员变量与静态函数

静态成员变量和静态成员函数在类中的作用和特性如下：

- 静态成员变量：

  - **生命周期**：静态成员变量在程序运行期间始终存在，其生命周期从程序开始运行到结束。

  - **作用范围**：静态成员变量属于类而非类的某个对象，所以它与普通成员变量的作用范围类似，但它可以通过类名直接访问，也可以通过对象访问。

  - **初始化要求**：静态成员变量**必须在类外进行初始化，不能通过构造函数初始化**。因为它是静态的，存在于静态存储区，且在编译期就已分配内存。

  - **内存管理**：静态成员变量只会有一份，不会为每个对象单独存储。

- 静态成员函数：

  - **作用**：静态成员函数通常用于操作静态成员变量。它不依赖于具体的对象实例，因此只能访问静态成员变量和其他静态成员函数。

  - **访问**：静态成员函数可以通过类名直接调用，也可以通过对象来调用。其设计目的是为了保持封装性，确保静态成员变量的访问方式统一、规范。

```cpp
struct Entity{
	static int x,y;
    
    void Print(){
		std::cout << x << "," << y << std::endl;
    }
}

int Entity::x;
int Entity::y;

int main(){
	Entity e;
    e.x = 2;
    e.y = 3;
    
    Entity e1;// 正常编程方法
    Entity::x = 5;
    Entity::y = 8;
    
    e.Print();
    Entity::Print();// 打印两次 5，8
    std::cin.get();
}
```

### 2.9 RTTI（运行时类型识别）

**RTTI**（Run-Time Type Identification）是C++中的一种机制，用于在程序运行时识别对象的真实类型。

要用于通过基类的指针或引用来检查这些指针或引用所指向的对象的实际派生类。这种机制支持C++的多态性，让程序能够在运行时动态判断类型。

主要使用场景：

- **异常处理**：RTTI在异常处理中的作用非常重要，用于捕获不同类型的异常对象。通过RTTI可以准确获取异常对象的类型，并且在捕获时选择相应的处理方式。
- **IO操作**：RTTI可以用于输入输出操作中的类型识别，尤其是在多态对象的序列化/反序列化中，通过RTTI能够正确处理对象的类型转换。

实现方法:

- **typeid**：
  - `typeid` 是C++中的一个运算符，用来返回一个 `type_info` 对象，包含了所指向对象的真实类型信息。
  - `type_info` 结构体的 `name()` 函数可以返回对象的类型名称。
  - `typeid(*base).name()`的返回值和编译器有关，如`msvc`会返回`class Spear`，`gcc/clang`则会返回`5Spear`。
  - **注意**：`typeid` 的使用通常需要包含 `<typeinfo>` 头文件。
- **dynamic_cast**：
  - `dynamic_cast` 是C++中用于执行类型转换的运算符，特别是用于将基类指针或引用转换为派生类指针或引用。
  - 该操作需要使用RTTI来验证转换是否成功，通常用于多态对象。
  - 如果转换成功，返回派生类类型的指针；如果失败，返回 `nullptr`（对于指针转换）或抛出 `std::bad_cast` 异常（对于引用转换）。

------

`typeid` 依赖于运行时类型信息（RTTI）来识别对象的实际类型。只有当类中包含虚函数时，编译器才会为该类生成虚表（vtable），从而启用 RTTI：

- **有虚函数**：类具有虚表，`typeid` 能准确识别对象的真实类型（父类指针指向子类对象时也能识别出子类）。
- **无虚函数**：类没有虚表，`typeid` 只能识别指针或引用的声明类型，而无法区分实际对象的派生类型。

### 2.10 虚继承

**多继承**指的是一个类同时继承自多个基类，例如：

```cpp
class Derived : public Base1, public Base2 {
    // ...
};
```

- 多继承最需要注意的点就是重复继承的问题，这个问题将在虚继承中解决
- 多继承会使整个程序的设计更加复杂，平常不推荐使用。C++语言中用到多继承的地方主要就是接口模式。相较于C++，java直接取消了多继承的功能，添加了接口的概念。
- **多继承这个语法虽然在某些情况下使代码写起来更加简洁，但会使程序更加复杂难懂，一般来说除了接口模式不推荐使用。**

------

1. **虚继承的概念**

**虚继承（Virtual Inheritance）** 是一种 C++ 中用于解决多重继承中菱形继承（Diamond Inheritance）问题的机制。菱形继承会导致派生类继承了多个基类的同名成员，从而产生二义性。虚继承通过确保派生类只继承一次共同的基类，来避免这种二义性问题。

```cpp
#include <iostream>
#include <string>

// 基类 Attack
class Attack {
public:
    Attack() { std::cout << "Attack constructor" << std::endl; }
    void showAttackType() const { std::cout << "This is a generic attack." << std::endl; }
};

// 物理攻击类，虚继承自 Attack
class PhysicalAttack : virtual public Attack {
public:
    PhysicalAttack() { std::cout << "PhysicalAttack constructor" << std::endl; }
    void performPhysicalAttack() const {
        std::cout << "Performing physical attack!" << std::endl;
    }
};

// 魔法攻击类，虚继承自 Attack
class MagicAttack : virtual public Attack {
public:
    MagicAttack() { std::cout << "MagicAttack constructor" << std::endl; }
    void performMagicAttack() const {
        std::cout << "Performing magic attack!" << std::endl;
    }
};

// 角色类，同时继承物理攻击和魔法攻击
class Character : public PhysicalAttack, public MagicAttack {
public:
    Character() { std::cout << "Character constructor" << std::endl; }

    void performAllAttacks() const {
        performPhysicalAttack();
        performMagicAttack();
        showAttackType(); // 调用基类 Attack 的方法，确保只有一个 Attack 实例
    }
};

int main() {
    std::cout << "Creating Character object:" << std::endl;
    Character hero;
    
    std::cout << "\nHero attacks:" << std::endl;
    hero.performAllAttacks();
    
    return 0;
}
```

**输出：**

```cpp
Creating Character object:
Attack constructor
PhysicalAttack constructor
MagicAttack constructor
Character constructor

Hero attacks:
Performing physical attack!
Performing magic attack!
This is a generic attack.
```

2. **虚继承的实现原理介绍**

虚继承的实现涉及编译器为类创建一个**虚继承表（Virtual Inheritance Table，简称 VITable）**，用于管理共享的基类成员。具体实现原理如下：

- **虚继承表的存在：**
  - **使用虚继承的每个类都会有一个虚继承表**，表中存放了虚继承基类的偏移地址。这确保了所有通过虚继承路径继承自同一个基类的子类共享同一个基类实例。
- **偏移地址的管理：**
  - 在上述示例中，`PhysicalAttack` 和 `MagicAttack` 都通过虚继承继承自 `Attack`。当 `Character` 同时继承 `PhysicalAttack` 和 `MagicAttack` 时，编译器通过虚继承表检测到 `Attack` 已经被虚继承，并确保 `Character` 只有一个 `Attack` 的实例。
  - 具体来说，每个通过虚继承继承的类（如 `PhysicalAttack`、`MagicAttack`）会在其虚继承表中记录 `Attack` 的偏移地址。当 `Character` 继承 `PhysicalAttack` 和 `MagicAttack` 时，编译器检查虚继承表，发现 两个表对应的偏移值相同，表示`Attack` 已经存在，避免再次实例化 `Attack`。
- **内存布局：**
  - 虚继承会影响对象的内存布局。编译器需要在对象内存中保留空间用于虚继承表指针，以便在运行时正确访问共享的基类成员。这也意味着虚继承可能带来一定的性能开销。

虚继承除了游戏开发以外的领域，别的地儿基本上用不上。

### 2.11 移动构造函数与移动赋值运算符

对一个体积较大的累进行大量拷贝操作时非常消耗性能的，因此C++11引入了“对象移动”的操作，简单来说，移动语义的核心在于:

- **资源所有权的转移**：移动构造函数和移动赋值运算符会“窃取”源对象的资源（例如，指针），然后将源对象置于一个有效但未指定的状态。这避免了资源的深拷贝。

- **效率提升**：由于只是指针或句柄的转移，而不是整个资源的复制，移动操作通常比拷贝操作更高效，特别是对于大型或复杂的对象。

如下例所示，实际开发使用`std::string`和`std::unqiue_ptr`

```cpp
#include <cstring>
#include <iostream>


class TestString{
public:
    TestString(const TestString& test){// 拷贝构造函数
        if(test.str){
            str = new char[strlen(test.str) + 1]();
            strcpy(str, test.str);
        }
        else{
            str = nullptr;
        }
    }

    TestString(TestString&& test){// 移动构造函数
        if(test.str){
            str = test.str;
            test.str = nullptr;
        }
        else{
            str = nullptr;
        }
    }

    TestString& operator=(const TestString& test){// 赋值运算符
        if(this == &test){
            return *this;
        }
        if(str){
            delete[] str;
            str = nullptr;
        }
        
        if(test.str){
            str = new char[strlen(test.str) + 1]();
            strcpy(str, test.str);
            // strcpy_s(str, strlen(test.str) + 1, test.str); // msvc支持
        }
        else{
            str = nullptr;
        }
        return *this;
    }

    TestString& operator=(TestString&& test){// 移动赋值运算符
        if(this == &test){
            return *this;
        }
        if(str){
            delete[] str;
            str = nullptr;
        }
        if(test.str){
            str = test.str;
            test.str = nullptr;
        }
        else{
            str = nullptr;
        }
        return *this;
    }

    ~TestString(){
        if (str){
            delete[] str;
        }
    }

private:
    char* str;
};
```

**注意：**

- **在进行转移操作后，被转移的对象就不能继续使用了，所以对象移动一般都是对临时对象进行操作（因为临时对象很快就要销毁了）**
- **右值引用（`Type&&`）用于表示即将被销毁或不再需要的对象，允许资源的所有权转移（移动），因此绝不能加上`const`修饰**

------

**默认移动构造函数和默认移动赋值运算符**会在以下情况下自动生成：

- 类没有自定义拷贝构造函数和拷贝赋值运算符。
- 类的每个非静态成员都支持移动（即它们自己有有效的移动构造和移动赋值）。

**可以移动**的条件：

- 基本数据类型（如 `int`, `double`）可以移动。
- 具有移动语义的类（例如使用了右值引用的类）也可以移动。

简而言之，只有在没有自定义拷贝操作且所有成员支持移动时，C++编译器才会自动生成移动构造和移动赋值操作符。

## 3.智能指针

### 3.1 智能指针概述

智能指针是C++提供的用于管理动态分配内存的对象，它自动管理对象的生命周期，避免手动管理内存所带来的问题（如内存泄漏、悬挂指针等）。

可分为共享型智能指针和独享型智能指针，均是指向堆内存

**常见类型的智能指针：**

1. **`std::unique_ptr`**：
   - **唯一拥有**：一个对象只能由一个 `unique_ptr` 管理。不能复制，只能移动。
   - **自动释放**：当 `unique_ptr` 被销毁时，它所管理的对象会被自动释放。
2. **`std::shared_ptr`**：
   - **共享拥有**：多个 `shared_ptr` 可以共同拥有同一个对象。
   - **引用计数**：每个 `shared_ptr` 会维护一个引用计数，当计数归零时，所管理的对象才会被销毁。
3. **`std::weak_ptr`**：
   - **不影响引用计数**：`weak_ptr` 用来观察一个对象（通常由 `shared_ptr` 管理），但不会影响其引用计数。
   - **防止循环引用**：通常用于打破 `shared_ptr` 之间的循环引用问题。

**注意：**

​	**智能指针不要和裸指针混用！！！**

​	**智能指针不要和裸指针混用！！！**

​	**智能指针不要和裸指针混用！！！**

### 3.2 shared_ptr

在动态分配内存时，堆上的内存必须通过栈上的内存来寻址，也就是说**栈上的指针是寻找堆内存的唯一方式**；

当然，堆上的指针也可以指向堆内存，但终究是要通过栈来寻址的，在现代程序中，需要共享一片内存时，优先使用`shared_ptr`，可以极大程度减少内存泄漏的问题，下面将介绍shared_ptr的初始化方式：

- **使用new运算符初始化（不推荐）**

- **使用make_shared函数进行初始化** 

- **使用复制构造函数初始化也是可以的**

```cpp
#include <memory>
#include <iostream>

struct MyStruct {
    MyStruct() { std::cout << "MyStruct Constructor\n"; }
    ~MyStruct() { std::cout << "MyStruct Destructor\n"; }
};

int main() {
    // std::shared_ptr<int> shared(new int(100));// 不推荐的初始化方式

    // int* pi = new int(100);
    // std::shared_ptr<int> shared_3(pi);// 禁止使用裸指针初始化

    // 智能指针对于数组的支持并不好
    std::shared_ptr<int> shared(new int[100]());
    std::cout << shared.get()[10] << "\n";

    std::shared_ptr<MyStruct> ptr1 = std::make_shared<MyStruct>();

    std::cout << ptr1.unique() << "\n"; // 输出1，此时是独占的

    std::shared_ptr<MyStruct> ptr2 = ptr1; // 引用计数增加到2
    std::cout << "Before reset, use_count: " << ptr1.use_count() << "\n"; // 输出 2
    std::cout << ptr2.unique() << "\n"; // 输出0，此时不是独占的
    ptr1.reset(); // ptr1 释放所有权，引用计数减少到1


    std::cout << "After reset, ptr1 use_count: " << ptr1.use_count() << "\n"; // 输出 0
    ptr2.reset();// 当 ptr2 释放所有权，引用计数降为0，MyStruct 被销毁
    if(ptr2.get() == nullptr) std::cout << "ptr2 is nullptr" << "\n"; // 输出ptr2 is nullptr
    std::cout << "After reset, ptr2 use_count: " << ptr2.use_count() << "\n"; // 输出 0

    

    return 0;
}
```

智能指针可以像普通函数一样使用，`shared_ptr`对指针相关的操作都进行了重载，用法上可以直接当成普通指针调用

------

**关于`unique()`与`swap()`**:

- 判断此时`shared_ptr`指向的内存是否被独占；
- 交换两个智能指针指向的内存；

------

**关于`reset()`**：

- `reset()` 会使 `shared_ptr` **释放其当前管理的对象**。若是有参调用，则`shared_ptr`会直接改变对象指向的内存，若是无参调用， `reset()` 没有传入新的指针（注意这个参数不能是另一个智能指针，如果想重置为智能指针，直接调用赋值运算符即可，`shared_ptr`对`=`进行了重载），则 `shared_ptr` 会变为空（即不再指向任何对象）；
- `shared_ptr` 内部维护一个引用计数（reference count）。调用 `reset()` 后，当前 `shared_ptr` 对象所持有的引用计数会减少。如果该 `shared_ptr` 是最后一个持有该资源的 `shared_ptr`，引用计数会降为零，此时管理的对象会被自动销毁（调用其析构函数）；
- 调用 `reset()` 后，`shared_ptr` 本身会被重置为一个空的状态，相当于它不再指向任何对象。此时，`shared_ptr` 的 `get()` 方法将返回 `nullptr`；
- 如果有其他 `shared_ptr` 实例共享同一个资源，**调用 `reset()` 只会影响当前的 `shared_ptr` 对象**，不会影响其他仍然持有该资源的 `shared_ptr`。只有当最后一个持有该资源的 `shared_ptr` 调用 `reset()` 或被销毁时，资源才会被释放。

------

**关于`get()`函数**：

- **强烈不推荐使用！**一些第三方库或旧的 API 可能只接受裸指针作为参数，此时可以使用 `get()` 传递指针，如果一定要使用，一定不能`delete`返回的指针；

- **破坏所有权语义**：`shared_ptr` 旨在通过引用计数机制自动管理资源的生命周期，确保资源在最后一个拥有者释放时被正确销毁。使用 `get()` 获取裸指针后，可能会在不明确所有权关系的情况下使用该指针，破坏了智能指针所提供的所有权管理优势；
- **增加悬空指针风险**：通过 `get()` 获取裸指针并在 `shared_ptr` 被销毁或重置后继续使用该裸指针，容易导致悬空指针（Dangling Pointer）的出现，从而引发未定义行为或程序崩溃；
- **容易导致资源管理错误**：裸指针的使用需要手动管理资源，可能会引入内存泄漏、双重释放等问题。而智能指针则通过自动管理资源生命周期，减少了这类错误的可能性；
- **违背 RAII 原则**：RAII（资源获取即初始化）是一种通过对象生命周期管理资源的编程理念。使用 `get()` 获取裸指针并在其上进行手动资源管理，违背了 RAII 的初衷，降低了代码的安全性和可维护性。

------

**使用智能指针作为函数参数时，可以直接通过值传递（即复制传递）而无需担心效率问题。**

- **固定大小**：`shared_ptr` 的大小是固定的，通常在32位系统上占用8字节（两个4字节的指针），在64位系统上占用16字节（两个8字节的指针）。这两个指针分别指向实际管理的对象和管理引用计数的控制块。
- **复制开销小**：因为 `shared_ptr` 只是包含两个指针，复制它们的开销非常小，不会显著影响性能。
- **引用计数共享**：即使通过值传递复制了 `shared_ptr`，新的副本仍然指向同一个对象，并共享同一个引用计数。因此，复制传递不会导致对象被多次创建或管理不当。

### 3.3 weak_ptr

- 虽然`shared_ptr`在管理内存方面接近完美，但它有一个显著缺陷——**循环引用**问题；`shared_ptr`通过引用计数来管理内存，这使得它能够在最后一个指向某对象的指针被销毁时自动释放内存。然而，若两个`shared_ptr`互相引用，就会形成一个循环引用（即它们彼此持有对方的`shared_ptr`），这会导致两者的引用计数永远不会降为0，内存无法被释放，造成内存泄漏；
- `weak_ptr`的设计是为了帮助解决`shared_ptr`的循环引用问题。与`shared_ptr`不同，`weak_ptr`不会增加或减少引用计数，因此它不会干扰`shared_ptr`的生命周期管理。

```cpp
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
```

------

**内存变化分析**

```bash
Stack:
+-----------------------+
| main()                |
| - parentPtr (shared)  | ---> Parent (ref count: 2)
| - childPtr (shared)   | ---> Child  (ref count: 2)
+-----------------------+

Heap:
+----------------------+        +----------------------+
| Parent Object        |        | Child Object         |
| - childPtr (shared)  | -----> | - parentPtr (shared) |
|                      | <----- |                      |
+----------------------+        +----------------------+

Control Blocks:
Parent's control block: ref count = 2
Child's control block: ref count = 2

```

------

**解决办法**

```cpp
#include <iostream>
#include <memory>

class Parent;
class Child;

class Child{
public:
    std::weak_ptr<Parent> parentPtr; // 使用 weak_ptr 打破循环引用
};

class Parent{
public:
    std::shared_ptr<Child> childPtr;
};

int main(){
    std::shared_ptr<Parent> parentPtr = std::make_shared<Parent>();
    std::shared_ptr<Child> childPtr = std::make_shared<Child>();

    parentPtr->childPtr = childPtr;
    childPtr->parentPtr = parentPtr; // 使用 weak_ptr，不增加引用计数

    return 0;
}
```

- `weak_ptr`需要绑定到一个`shared_ptr`对象；它只会观察目标对象，并不拥有目标对象的所有权。
- 当所有的`shared_ptr`引用计数变为0时，目标对象的内存会被释放，而`weak_ptr`不会阻止这一过程。这种设计方式在处理复杂数据结构（如双向链表或图）时非常有用，可以有效避免内存泄漏。

### 3.4 unique_ptr

**`std::unique_ptr`**

- **独占所有权**：`unique_ptr` 拥有其所管理对象的独占所有权，不能被复制，只能被移动。
- **轻量级**：由于不涉及引用计数，`unique_ptr` 通常比 `shared_ptr` 更轻量。
- **适用场景**：适用于明确的所有权关系，不需要多个指针共享同一资源的场景

```cpp
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
```

### 3.5 智能指针的使用范围

**尽量使用智能指针**

- **推荐使用场景**：在绝大多数情况下，应优先选择智能指针来管理动态内存，以确保资源的自动释放和防止内存泄漏。
- **不能使用智能指针的情况**
  - **必须使用C语言指针的函数**：某些底层或系统级别的函数仅接受C语言风格的裸指针，无法替代智能指针。例如：
    - **网络传输函数**：如Windows下的 `send`、`recv` 函数，只能使用C语言指针。
    - **C语言文件操作函数**：部分C语言的文件操作接口需要使用裸指针，尽管C++提供了智能指针支持的文件操作，推荐在大型项目中优先使用C++的文件操作功能。
- 仅在上述必须使用C语言指针的特定场景下，使用裸指针；其他所有情况下均应使用智能指针。

**智能指针的选择策略**

- **优先选择 `std::unique_ptr`**
  - **特点**：独占所有权，轻量级，不涉及引用计数。
  - **适用场景**：资源有明确的所有者，不需要共享。
- **在需要共享所有权时使用 `std::shared_ptr`**
  - **特点**：通过引用计数实现共享所有权，适用于多个所有者需要访问同一资源的情况。
  - **注意事项**：`shared_ptr` 可能引发循环引用问题。
- **处理循环引用时使用 `std::weak_ptr`**
  - **用途**：与 `shared_ptr` 配合使用，打破循环引用链，防止内存泄漏。
  - **使用场景**：当 `shared_ptr` 之间存在相互引用时，使用 `weak_ptr` 来引用部分对象，从而避免引用计数无法归零。

## 4.模板与泛型编程（基础）

### 4.1 模板介绍，类模板与模板实现原理

**模板的重要性**

- **模板**是C++三大编程范式之一，与**面向过程**和**面向对象**并列，构成C++的核心特性。
- **面向过程**对应C语言，强调函数和过程的调用。
- **面向对象**通过**类**和**对象**实现数据封装、继承和多态。
- **模板与泛型编程**允许编写与类型无关的代码，提高代码的重用性和灵活性，是现代C++编程的重要组成部分。

**模板的介绍**

- **模板**提供了实现某些特定功能的能力，这些功能在其他语法下难以实现。然而，模板的复杂性较高，初学者可能难以理解和掌握。
- **模板分类**
  - **类模板（Class Templates）**：用于定义与类型无关的类。例如，`std::vector<T>`。
  - **函数模板（Function Templates）**：用于定义与类型无关的函数。
    - **普通函数模板**：独立于任何类的函数模板。
    - **成员函数模板**：属于类模板或普通类的模板化成员函数。

------

**类模板基础**

- **类模板的结构与使用**具有固定的模式和语法，理解和掌握这一结构对于后续学习函数模板至关重要。
- **典型特征**：
  - 使用`template<typename T>`或`template<class T>`声明模板参数。
  - 模板参数化类成员和方法，使类在不同类型下复用。

**代码示例**：

**`MyArray.hpp`**

```cpp
// MyArray.hpp
#ifndef MYARRAY_HPP
#define MYARRAY_HPP

#include <cstddef>    // for size_t
#include <algorithm>  // for std::copy, std::swap
#include <stdexcept>  // for std::out_of_range

template<typename T>
class MyArray {
public:
    using iterator = T*;
    using const_iterator = const T*;

    // 构造函数
    explicit MyArray(size_t count = 0);

    // 拷贝构造函数
    MyArray(const MyArray& other);

    // 移动构造函数
    MyArray(MyArray&& other) noexcept;

    // 拷贝赋值运算符
    MyArray& operator=(const MyArray& other);

    // 移动赋值运算符
    MyArray& operator=(MyArray&& other) noexcept;

    // 析构函数
    ~MyArray();

    // 元素访问
    T& operator[](size_t index);
    const T& operator[](size_t index) const;

    // 获取数组大小
    size_t size() const;

    // 迭代器
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

private:
    T* data;
    size_t m_size;

    void swap(MyArray& other) noexcept;
};

// 构造函数
template<typename T>
MyArray<T>::MyArray(size_t count) : data(nullptr), m_size(count) {
    if (count > 0) {
        data = new T[count]();
    }
}

// 拷贝构造函数
template<typename T>
MyArray<T>::MyArray(const MyArray& other) : data(nullptr), m_size(other.m_size) {
    if (m_size > 0) {
        data = new T[m_size];
        std::copy(other.data, other.data + m_size, data);
    }
}

// 移动构造函数
template<typename T>
MyArray<T>::MyArray(MyArray&& other) noexcept : data(other.data), m_size(other.m_size) {
    other.data = nullptr;
    other.m_size = 0;
}

// 拷贝赋值运算符
template<typename T>
MyArray<T>& MyArray<T>::operator=(const MyArray& other) {
    if (this != &other) {
        MyArray temp(other); // 利用拷贝构造函数
        swap(temp);
    }
    return *this;
}

// 移动赋值运算符
template<typename T>
MyArray<T>& MyArray<T>::operator=(MyArray&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        m_size = other.m_size;
        other.data = nullptr;
        other.m_size = 0;
    }
    return *this;
}

// 析构函数
template<typename T>
MyArray<T>::~MyArray() {
    delete[] data;
}

// 元素访问
template<typename T>
T& MyArray<T>::operator[](size_t index) {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

template<typename T>
const T& MyArray<T>::operator[](size_t index) const {
    if (index >= m_size) {
        throw std::out_of_range("Index out of range");
    }
    return data[index];
}

// 获取数组大小
template<typename T>
size_t MyArray<T>::size() const {
    return m_size;
}

// 迭代器
template<typename T>
typename MyArray<T>::iterator MyArray<T>::begin() {
    return data;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::begin() const {
    return data;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::cbegin() const {
    return data;
}

template<typename T>
typename MyArray<T>::iterator MyArray<T>::end() {
    return data + m_size;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::end() const {
    return data + m_size;
}

template<typename T>
typename MyArray<T>::const_iterator MyArray<T>::cend() const {
    return data + m_size;
}

// 交换函数
template<typename T>
void MyArray<T>::swap(MyArray& other) noexcept {
    std::swap(data, other.data);
    std::swap(m_size, other.m_size);
}

#endif // MYARRAY_HPP
```

**`main.cpp`**

```cpp
// main.cpp
#include <iostream>
#include "MyArray.hpp"

int main() {
    // 创建一个 MyArray<int>，包含5个元素，初始化为0
    MyArray<int> arr1(5);
    std::cout << "arr1 size: " << arr1.size() << "\n";
    std::cout << "arr1 elements: ";
    for (size_t i = 0; i < arr1.size(); ++i) {
        std::cout << arr1[i] << " ";
    }
    std::cout << "\n";

    // 修改 arr1 的元素
    for (size_t i = 0; i < arr1.size(); ++i) {
        arr1[i] = static_cast<int>(i * 10);
    }
    std::cout << "arr1 elements after modification: ";
    for (const auto& elem : arr1) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 测试拷贝构造函数
    MyArray<int> arr2 = arr1;
    std::cout << "arr2 (copy of arr1) elements: ";
    for (const auto& elem : arr2) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 修改 arr2，确保 arr1 不受影响
    arr2[0] = 100;
    std::cout << "After modifying arr2:\n";
    std::cout << "arr1 elements: ";
    for (const auto& elem : arr1) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    std::cout << "arr2 elements: ";
    for (const auto& elem : arr2) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 测试移动构造函数
    MyArray<int> arr3 = std::move(arr1);
    std::cout << "arr3 (moved from arr1) elements: ";
    for (const auto& elem : arr3) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    std::cout << "arr1 size after move: " << arr1.size() << "\n";

    // 测试拷贝赋值运算符
    MyArray<int> arr4;
    arr4 = arr2;
    std::cout << "arr4 (assigned from arr2) elements: ";
    for (const auto& elem : arr4) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // 测试移动赋值运算符
    MyArray<int> arr5;
    arr5 = std::move(arr2);
    std::cout << "arr5 (moved from arr2) elements: ";
    for (const auto& elem : arr5) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
    std::cout << "arr2 size after move: " << arr2.size() << "\n";

    // 测试异常处理
    try {
        std::cout << "Accessing out-of-bounds index:\n";
        std::cout << arr5[10] << "\n"; // 这将抛出异常
    } catch (const std::out_of_range& e) {
        std::cout << "Caught exception: " << e.what() << "\n";
    }

    return 0;
}
```

**输出结果**

```bash
g++ -std=c++11 -o MyArrayTest main.cpp MyArray.hpp 
./MyArrayTest
arr1 size: 5
arr1 elements: 0 0 0 0 0 
arr1 elements after modification: 0 10 20 30 40 
arr2 (copy of arr1) elements: 0 10 20 30 40 
After modifying arr2:
arr1 elements: 0 10 20 30 40 
arr2 elements: 100 10 20 30 40 
arr3 (moved from arr1) elements: 0 10 20 30 40 
arr1 size after move: 0
arr4 (assigned from arr2) elements: 100 10 20 30 40 
arr5 (moved from arr2) elements: 100 10 20 30 40 
arr2 size after move: 0
Accessing out-of-bounds index:
Caught exception: Index out of rangearr1 size: 5
arr1 elements: 0 0 0 0 0 
arr1 elements after modification: 0 10 20 30 40 
arr2 (copy of arr1) elements: 0 10 20 30 40 
After modifying arr2:
arr1 elements: 0 10 20 30 40 
arr2 elements: 100 10 20 30 40 
arr3 (moved from arr1) elements: 0 10 20 30 40 
arr1 size after move: 0
arr4 (assigned from arr2) elements: 100 10 20 30 40 
arr5 (moved from arr2) elements: 100 10 20 30 40 
arr2 size after move: 0
Accessing out-of-bounds index:
Caught exception: Index out of range
```

**模板的实现原理**

- **模板编译机制**：两阶段编译，模板在编译时经历两次编译过程

  - ##### 第一阶段：模板解析与检查

    - **语法检查**：编译器首先检查模板定义的基本语法是否正确，例如括号匹配、类型名正确等。
    - **延迟实例化**：此时，编译器不会为模板生成具体的代码，因为模板是通用的，具体的类型尚未确定。

  - ##### 第二阶段：模板实例化与代码生成

    - **实例化触发**：当模板被使用（例如创建一个具体类型的 `MyArray<int>` 实例时），编译器会根据提供的类型参数进行模板实例化。
    - **类型替换**：编译器将模板中的模板参数（如 `T`）替换为具体类型（如 `int`），生成特定类型的类或函数代码。
    - **代码生成**：根据替换后的具体类型，编译器生成对应的机器代码。

- **声明与实现同文件原则**

  - 由于模板实例化需要在编译期知道具体实现，**模板的声明与实现必须放在同一个文件中**，通常采用`.hpp`或`.tpp`等头文件扩展名。
  - 将声明与实现分离（如分别置于`.hpp`和`.cpp`文件中）**会导致链接错误**，因为**模板实例化时无法找到具体实现**。

- **文件组织**

  - **头文件（.hpp）**：通常包含模板类或模板函数的声明和实现，以确保在编译期可见。
  - **源文件（.cpp）**：不适用于模板的实现，除非使用显式模板实例化（较为复杂，通常不推荐）。

### 延申：模板与RTTI

C++ 中模板（Templates）和运行时类型信息（RTTI, Run-Time Type Information）虽然都涉及类型，但它们服务于不同的目的，且在设计理念和实现机制上有显著区别。这也是为何C++对RTTI的支持相对较弱的原因：

#### 1. 模板与RTTI的基本概念

- **模板（Templates）**：
  - **编译时机制**：模板是在编译期进行类型参数化，通过代码生成实现对不同类型的复用。
  - **类型泛化**：允许编写与类型无关的通用代码，提高代码的复用性和灵活性。
  - **静态多态性**：通过模板实现的多态性在编译期确定，消除了运行时的开销。
- **运行时类型信息（RTTI）**：
  - **运行时机制**：RTTI 提供在程序运行时查询对象的实际类型信息。
  - **动态多态性**：通过 `dynamic_cast` 和 `typeid` 等操作实现，支持在继承体系中进行安全的类型转换和类型识别。
  - **类型安全**：确保在类型转换时的安全性，防止非法转换导致的未定义行为。

#### 2. 模板与RTTI的设计目标不同

- **模板的目标**：
  - **编译期优化**：通过模板实现的代码在编译期就确定了具体类型，编译器可以进行高度优化，生成高效的机器代码。
  - **类型无关性**：模板允许开发者编写与具体类型无关的通用代码，极大地提升了代码的复用性。
- **RTTI的目标**：
  - **运行时灵活性**：提供在运行时动态识别对象类型的能力，支持基于类型的决策和安全的类型转换。
  - **动态行为支持**：在需要动态多态性的场景下（如插件系统、反射机制等），RTTI 提供必要的类型信息支持。

#### 3. C++ 对RTTI支持较弱的原因

- **性能开销**：
  - **运行时开销**：RTTI 需要在运行时维护类型信息（如虚表指针），这**会带来一定的性能和内存开销**。
  - **模板的零开销**：**模板在编译期展开，不增加运行时开销**，与C++追求高性能的设计理念一致。
- **语言复杂性与编译时间**：
  - **编译器复杂性**：全面支持RTTI会增加编译器的复杂性，尤其是在多继承、虚继承等复杂继承体系中。
  - **模板的广泛使用**：C++ 模板在泛型编程中扮演核心角色，过度依赖RTTI可能与模板的设计理念和使用方式产生冲突。
- **类型系统设计**：
  - **静态类型优先**：C++ 是一个静态类型语言，强调在编译期的类型安全和优化，RTTI 的设计与此理念有一定的张力。
  - **有限的动态特性**：**相比于像Java、C#等语言，C++ 在设计上并未将动态类型系统作为核心特性，因此RTTI的功能相对有限**。

#### 4. 模板与RTTI的互补性

- **不同用途**：
  - **模板**：适用于需要在编译期实现类型泛化和代码复用的场景，如容器类（`std::vector<T>`）、算法库等。
  - **RTTI**：适用于需要在运行时进行类型识别和动态行为决策的场景，如对象序列化、插件系统等。
- **互补使用**：
  - 在实际开发中，模板和RTTI 可以根据需求互补使用。例如，使用模板实现高效的通用容器，同时在需要动态类型识别的部分使用RTTI 提供类型安全的动态转换。

#### 5. 现代C++对RTTI的态度

- **可选性**
  - C++ 提供了RTTI支持，但并非强制启用。开发者可以通过编译选项（如 `-fno-rtti`）关闭RTTI，以减少运行时开销，提升性能。
- **替代方案**
  - 现代C++ 提供了多种替代RTTI的机制，如**类型擦除**（Type Erasure）、**多态模板**（Polymorphic Templates）等，这些机制在一定程度上弥补了RTTI的不足，同时保持了高效的性能。

### 4.2 initializer_list与typename

在 C++11 中，**`std::initializer_list`** 提供了一种简洁的方式来对容器或对象进行统一初始化。它本质上是一个轻量级的“只读”数组视图，存储了初始化时传入的元素。以下是对 `std::initializer_list` 的简单介绍：

可以使用花括号 `{}` 创建一个 `std::initializer_list`，例如：

```cpp
std::initializer_list<int> ilist{1, 2, 3, 4};
```

也常用于函数或类的构造函数参数中，如：

```cpp
void printList(std::initializer_list<int> lst) {
    for (auto elem : lst) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

int main() {
    printList({1, 2, 3, 4});
    return 0;
}
```

- **常用成员函数**

  - `begin()` / `end()`：返回首元素和尾后迭代器，可以用范围 `for` 循环访问。
  - `size()`：返回元素个数。

  需要注意的是，`std::initializer_list` 的元素是**只读**的，无法修改内部元素，也没有插入或删除的操作，且生命周期受限，在离开创建它的作用域后，不能保证对其元素的访问仍有效。

- **应用场景**

  - **统一初始化**：与花括号初始化搭配使用，让函数或构造函数接收可变数量的同类型参数，提升代码的可读性和灵活性，在函数调用、容器初始化等场景中十分常见。
  - **简化容器初始化**：例如 `std::vector` 构造函数可以直接接收 `std::initializer_list` 以简洁地初始化容器。
  - **可读性**：使用花括号初始化能让代码更直观地表达意图。

- **局限性**

  - 不支持修改：`std::initializer_list` 是不可变序列，一旦创建，无法在其上进行元素修改、插入或删除操作。
  - 只能使用 const 引用：如果需要在函数中对初始化的数据进行修改，需要使用其他容器类型（如 `std::vector`）。

------

**警告**：下面的示例只演示“如何让代码变得自洽”；
**在真实工程中**，推荐使用智能指针或标准容器，而不是手写“二级指针”深拷贝：

```cpp
template<typename T>
class MyArray {
public:
	/*
		...
	*/
    // 补充initializer_list初始化方法
    MyArray(const std::initializer_list<T>& list);
    MyArray(std::initialzer_list<T> && list);
private:
    // T* data;// 修改为std::vector容器实现
    std::vector> data;
};

// 模板特化最经典的应用，通过萃取获取类型
template<typename T> 
struct get_type
{
    using type = T;
};

template<typename T> 
struct get_type<T*>
{
    using type = T;
};


template<typename T>
MyArray<T>::MyArray(const std::initializer_list<T>& list){
    if(list.size()){
        unsigned count = 0;
        m_size = list.size();
        data = new T[m_size]();
        if(std::is_pointer<T>::value){
            // 解决指针浅拷贝的方式，使用模板特化或者萃取机制，当模板体积较大时一般使用萃取机制，此处可以采取很巧妙的方式实现
            for(auto elem : list){
                data[count++] = new typename get_type<T>::type(*elem);
                // 注意：这段代码是一个非常经典的内存泄漏问题，如果使用裸指针管理则非常容易引起内存泄漏
            }
        }
        else{
            for(const auto& elem : list){
                data[count++] = elem;
            }
        }
    }
    else{
        data = nullptr;
    }
}

template<typename T>
MyArray<T>::MyArray(std::initializer_list<T> && list){
    if(list.size()){
        unsigned count = 0;
        m_size = list.size();
        data = new T[m_size]();
        for(const auto& elem : list){// 对于右值引用来说，如果是指针类型就直接转移权限了
            data[count++] = std::move(elem);
        }
    }
    else{
        data = nullptr;
    }
}

// 析构函数
template<typename T>
MyArray<T>::~MyArray() {
    if (data) {
        // 如果是指针类型，我们在构造中做了 new U(*elem)，这里要delete
        if (std::is_pointer<T>::value) {
            for (size_t i = 0; i < m_size; ++i) {
                delete data[i]; // data[i] 是 T = U*
            }
        }
        delete[] data;
    }
}

//	```
//	main.cpp
#include <iostream>
#include "MyArray.hpp"

int main() {
    int i1 = 10;
    int i2 = 20;
    int i3 = 30;
    int i4 = 40;
    std::initializer_list<int*> iList{&i1, &i2, &i3, &i4};
    MyArray<int*> arrayPi(iList);
    for(unsigned i = 0; i < 4; ++i){
        std::cout << *arrayPi[i] << std::endl;
    }
}
```

------

`typename` 是 C++ 中的一个关键字，主要用于模板编程，表示类型名称。它的主要作用是在模板定义中表明某一标识符是一个类型，而不是一个变量或其他实体。

**关键场景及用法**

- **声明类型成员**
   在模板中，如果类型依赖于模板参数（依赖类型），需要用 `typename` 明确告诉编译器该标识符是一个类型。例如：

  ```cpp
  template <typename T>
  void func() {
      typename T::value_type val; // 明确表示 T::value_type 是一个类型
  }
  ```

  如果不加 `typename`，编译器可能会误认为 `T::value_type` 是一个变量或静态成员。

- **区分依赖类型与非依赖类型**
   在模板的嵌套类型中，如果类型依赖模板参数（即依赖上下文未确定的类型），就需要使用 `typename` 指定。例如：

  ```cpp
  template <typename T>
  class Container {
  public:
      typename T::iterator it; // 指明 T::iterator 是一个类型
      using iterator = T*;
      iterator begin();
  private:
  	T* data;
  };
  
  template typename T>
  typename Container<T>::iterator Container<T>::begin(){//  在返回值中指明依赖类型 
  	return data;
  }
  
  ```

- **模板形参中的约束**
   在 `template<typename T>` 中的 `typename`，表明 T 是一个类型参数，这种用法是模板的基础。例如：

  ```cpp
  template <typename T>
  class MyClass {
      // 使用 T 作为类型
  };
  ```

**常见错误及注意事项**

- 如果类型不是依赖于模板参数的（非依赖类型），则不能使用 `typename`。如：

  ```cpp
  int::value_type val; // 错误：int 的类型成员已经确定
  ```

- 如果在依赖类型中遗漏了 `typename`，会导致编译错误。例如：

  ```cpp
  template <typename T>
  void func() {
      T::value_type val; // 错误：未明确指示 value_type 是类型
  }
  ```

  应改为：

  ```cpp
  typename T::value_type val;
  ```

- C++早期版本使用`class`来作为模板参数，后由于类型依赖的原因不得不引入`typename`，现在通常使用`typename`

### 4.3 函数模板，成员函数模板

**函数模板**

- **定义与用途**：编写与类型无关的通用函数，适用于通用算法（如排序、遍历）。
- **特点**：支持模板参数推导，通常独立于类，灵活且类型安全。
- **优点**：代码复用、类型安全、灵活性强。
- **场景**：适合实现通用算法和操作。

**成员函数模板**

- **定义与用途**：类模板中的模板函数，处理不同类型的数据或操作。
- **特点**：支持独立的模板参数，语法较复杂，依赖于类模板。
- **优点**：高度灵活、代码复用、类型安全。
- **场景**：适用于类内部需要处理多类型数据的成员函数。

**代码示例**：

```cpp
// main.cpp
#include <iostream>
#include <vector>

namespace mystd{
    template<typename iter_type, typename func_type>// 函数模板
    void for_each(iter_type first, iter_type last, func_type func){
        for(auto iter = first; iter != last; ++iter){
            func(*iter);
        }
    }

    template<typename T>
    class MyVector{
    public:
        template<typename T2>
        void outPut(const T2& elem);
    };
    template<typename T>
    template<typename T2>
    void MyVector<T>::outPut(const T2& elem){// 成员函数模板
        std::cout << elem << "\n";
    }
};


int main() {
    std::vector<int> vec{1, 2, 3, 4, 5};
    // mystd::for_each<std::vector<int>::iterator, void(*)(int &elem)>(vec.begin(), vec.end(), [](int& elem){ // 一般没人会闲着这样写
    //     ++elem;
    // });

    mystd::for_each(vec.begin(), vec.end(), [](int& elem){
        ++elem;
    });

    for(int elem : vec){
        std::cout << elem << "\n";
    }

    mystd::MyVector<int> myVec;
    myVec.outPut<int>(20);

    return 0;
}
```

- **共同点**：都支持代码复用、类型安全和模板参数推导。
- **区别**：
  - **函数模板**：独立于类，适用于通用操作。
  - **成员函数模板**：属于类的成员，适用于处理多类型数据。

### 4.4 默认模板参数

默认模板参数是一个经常使用的特性，在模板声明中为模板参数提供默认类型，当用户在实例化模板时未显式指定该参数时，使用默认类型；比如在定义vector对象时，就可以使用默认分配器，几乎所有的STL都有默认模板参数；模板参数就和普通函数的默认参数一样，一旦一个参数有了默认参数，它之后的参数都必须有默认参数：

```cpp
// main.cpp
#include <iostream>
#include <vector>
#include <functional>

namespace mystd{
    using void_int_func_type = std::function<void(int&)>;// 默认模板参数
    template<typename iter_type, typename func_type = void_int_func_type>// 函数模板
    void for_each(iter_type first, iter_type last, func_type func = [](int& elem){
        ++elem;
    })
    {
        for(auto iter = first; iter != last; ++iter){
            func(*iter);
        }
    }

    template<typename T, typename allocator_type = std::allocator<T>>
    class MyVector{
    public:
        template<typename T2>
        void outPut(const T2& elem);
    };
    template<typename T, typename allocator>
    template<typename T2>
    void MyVector<T, allocator>::outPut(const T2& elem){// 成员函数模板
        std::cout << elem << "\n";
    }
};


int main() {
    std::vector<int> vec{1, 2, 3, 4, 5};


    mystd::for_each(vec.begin(), vec.end());
    for(int elem : vec){
        std::cout << elem << "\n";
    }

    return 0;
}
```

### 4.5 模板重载与特化

**函数模板的重载**

- **定义**：函数模板可以像普通函数一样进行重载，通过定义多个具有相同名称但参数类型不同的模板函数，以应对不同的使用场景。
- **用途**：处理不同类型的参数时提供不同的实现。例如，针对 `char*` 和 `std::string` 类型的字符串处理，优化性能或实现特定功能。
- **特点**
  - 与普通函数重载规则一致，编译器根据参数类型选择最合适的模板。
  - 仅限于函数模板，类模板不支持重载。

**代码示例**：

```cpp
#include <iostream>

template<typename T>
void test(const T& parm){
    std::cout << "void test(const T& parm)" << "\n";
}

template<typename T>
void test(const T* parm){
    std::cout << "void test(T* parm)" << "\n";
}


void test(double parm){
    std::cout << "void test(double parm)" << "\n";
}


int main(){
    test(100);
    int i = 10;
    test(&i);
    test(2.2);
    return 0;
    /*
    输出结果：
    void test(const T& parm)
    void test(const T& parm)
    void test(double parm)
	*/
}
```

2. **类模板的特化**

- **定义**：类模板不能被重载，但可以通过特化（全特化和偏特化）实现对特定类型的定制化处理。
  - **全特化**：
    - **概念**：为特定的模板参数组合提供专门的实现。
    - **特点**：模板参数完全匹配，所有参数类型均已明确。
    - **用途**：当需要为某一特定类型组合提供优化或不同的实现时使用。
  - **偏特化**：
    - **概念**：对模板参数的一部分进行特化，保留其他部分为通用形式。
    - **特点**：仅部分模板参数被明确，其他参数保持模板化。
    - **用途**：当需要对一类相关类型进行定制化处理时使用，例如指针类型的部分特化。
- **函数模板特化**：
  - 对于函数模板，特化与重载效果类似，通常通过重载实现不同类型的处理，而不常使用特化。

**示例代码**：

```cpp
#include <iostream>

template<typename T1, typename T2>
class Test{
public:
    Test(){
        std::cout << "common template" << "\n";
    }
};

template<typename T1, typename T2>
class Test<T1*, T2*>{
public:
    Test(){
        std::cout << "pointer template partial specialization" << "\n";
    }
};

template<typename T2>
class Test<int, T2>{
public:
    Test(){
        std::cout << "int template partial specialization" << "\n";
    }
};


template<>
class Test<int, int>{
public:
    Test(){
        std::cout << "int, int template full specialization" << "\n";
    }
};

int main(){
    Test<int*, int*> test1;
    Test<int, double> test2;
    Test<int, int> test3;
    Test<int, int*> test4;

    return 0;
    /*
    输出结果：
    pointer template partial specialization
    int template partial specialization
    int, int template full specialization
    int template partial specialization
	*/
}
```

3.**总结**：

- **函数模板重载**适用于需要根据不同参数类型提供不同函数实现的场景，通过重载机制简化代码逻辑，提高效率。
- **类模板特化**则用于类模板在处理特定类型组合时需要不同实现的情况，通过全特化和偏特化实现灵活的类定义。
- 在复杂程序中，**模板重载与特化**是实现高效、灵活和可维护代码的重要手段，掌握这两者的使用方法有助于应对多样化的编程需求。

## 5.STL（基础）

### 5.1 **STL简介** 

STL是C++的标准模板库（Standard Template Library），它定义在`std`命名空间中，包含常用的容器、算法、迭代器等。STL显著提高了开发效率，因为它提供了预先实现的模板类，可以减少程序员手写代码的工作量。

在C++开发中，有三类人：

- **不会用STL的人**：不使用STL，可能需要自己手动实现数据结构和算法。
- **会用STL但不懂实现原理的人**：使用STL，但对其背后的实现原理不太清楚。
- **既会使用STL，又懂得实现原理的人**：全面理解STL的工作机制，并能在必要时自定义或者优化其实现。

------

**泛型编程的概念**：

 泛型编程（`Generic Programming(GP)` ）是指使用模板进行编程，通过将数据类型作为参数，使得程序代码能够适应不同的数据类型。泛型编程实现了很多传统编程语言中难以实现的功能。模板是泛型编程的基础，但它**并不属于面向对象编程（`Object Oriented  Programming(OOP)`）的范畴，泛型编程与面向对象编程是并列的概念**。

**`GP`与`OOP`的编程思想有很大差异**:

- **`OOP`将`data`与`methods`结合；**
- **`GP`将`data`与`methods`分开；**

**STL与泛型编程的结合**：

STL是泛型编程的典型应用，它展示了如何用模板技术使得不同的数据结构和算法能够以相同的方式运作。比如，`vector`和`map`是两种完全不同的数据结构，但STL通过“迭代器”机制使得它们可以以相似的方式进行遍历和操作。这种灵活性和通用性正是泛型编程的优势所在。学习STL时，应该从全局的角度理解它的结构和设计理念，而不是仅仅关注某一个容器的使用方法。

------

**STL库包含六个重要的组成部分**：

- 容器（如`vector`, `map`, `list`等）
- 算法（如`sort`, `find`, `accumulate`等）
- 迭代器（用来访问容器的元素）
- 函数对象（可传递给算法以进行定制操作）
- 配接器（容器适配器，改变容器的接口）
- 分配器（管理内存分配）

### 5.2 容器 

C++标准委员会将容器分为三种，**顺序式容器、关联式容器与无序式容器**，但实际无序式容器也是一种关联式容器

1. **顺序容器（Sequence Containers）**

顺序容器按元素的插入顺序存储数据，元素之间的位置有明确的顺序。常见的顺序容器有：

- `vector`：动态数组，支持随机访问，适合于需要频繁访问元素的情况。

- `deque`：双端队列，支持两端插入和删除。
- `list`：双向链表，支持在中间插入和删除元素，但不支持高效的随机访问。
- `array`：固定大小的数组，元素个数在编译时确定。
- `forward_list`：单向链表，只支持从头部插入和删除元素。

顺序容器的特点是元素的顺序性非常重要，常用于需要按序访问或处理元素的场景。

2. **关联容器（Associative Containers）**

关联容器根据某种键值（key）来存储元素，并能够高效地根据键值进行查找。关联容器内部的元素有一定的排序，通常是根据键的顺序进行排序的。常见的关联容器有：

- `set`：存储唯一的键（元素本身即为键），按排序顺序存储。
- `map`：存储键值对（key-value），按键的顺序排序，允许通过键快速查找对应的值。
- `multiset`：存储可重复的键，按排序顺序存储。
- `multimap`：存储键值对（key-value），允许键重复，按键的顺序排序。

关联容器的特点是通过某种排序算法（如红黑树）组织元素，使得查找、插入和删除的时间复杂度通常为O(log N)，非常适合需要频繁查找和有序访问的场景。

3. **无序容器（Unordered Containers）**

无序容器存储元素时不保证元素的顺序，使用哈希表（hash table）来管理元素，因此查找、插入和删除的时间复杂度通常为O(1)，但在最坏情况下，可能会退化到O(N)。常见的无序容器有：

- `unordered_set`：存储唯一的键，元素的顺序是不确定的。
- `unordered_map`：存储键值对，元素的顺序是不确定的。
- `unordered_multiset`：存储可重复的键，元素的顺序是不确定的。
- `unordered_multimap`：存储键值对，允许键重复，元素的顺序是不确定的。

无序容器的特点是它们通过哈希表实现元素的快速查找，因此通常适用于不关心元素顺序、需要快速查找和插入的场景。

------

#### 顺序式容器

**vector**：

**将元素置于一个动态数组中，支持随机存储元素（也就是用索引直接存取）**

`vector`数组尾部添加或删除元素非常迅速，但在中部或头部就比较费时。

**构造函数**：

```cpp
#include <iostream>
#include <vector>
#include <stdexcept>
#include <deque>
#include <set>
   
using std::vector;
int main(){
    /*
        1.默认构造函数，以下vec1、vec2的构造函数是一样的，申请一个空容器
        vector(const std::allocator<int> &__a)
        __a – An allocator object.
        Creates a %vector with no elements.
    */
    vector<int> vec1;
    // vector<int> vec2();// 不推荐，因为编译器会出现 “Vexing Parse”（令人困惑的解析）问题，编译器会将 vector<int> vec2(); 解析为函数声明，而不是想要的 vector<int> 变量的默认构造。

    /*
        2.填充的方式构造vector容器
        vector(std::size_t __n, const std::allocator<int> &__a = std::vector<int>::allocator_type())
        vector(std::size_t __n, const int &__value, const std::allocator<int> &__a = std::vector<int>::allocator_type())
        __a – An allocator.

        This constructor fills the %vector with __n copies of __value.


        Creates a %vector with copies of an exemplar element.
    */
    vector<int> vec_filled_default(10);
    vector<int> vec_filled(10, 5);

    /*
        3.复制构造函数
        vector(const std::vector<int> &__x)
        vector(const std::vector<int> &__x, const std::allocator<int> &__a)
    */
    vector<int> vec3(vec_filled); 

    /*
        4.移动构造函数
            vector(std::vector<int> &&__rv, const std::allocator<int> &__m)
            vector(std::vector<int> &&__rv, const std::allocator<int> &__m, std::false_type)
                用途：这个构造函数用于处理非 trivial（非平凡）的类型。std::false_type 是一个常量类型，用来表示类型的某些特性。
                内部实现：这个构造函数处理的是那些需要“深拷贝”的情况，特别是当源 vector 不是简单的类型（如 int）时。它可能需要对容器内的对象进行适当的析构、复制等处理。
                背景：对于复杂的类型（例如具有自定义构造函数、析构函数或拷贝控制的类型），std::false_type 的存在表明我们需要进行一些额外的工作，如“深拷贝”操作来完成移动
            vector(std::vector<int> &&__rv, const std::allocator<int> &__m, std::true_type)
                用途：这个构造函数用于处理平凡类型（trivial types），即不需要复杂的析构和拷贝控制的类型（如 int, float 等）。
                内部实现：对于这些类型，构造函数直接移动元素，而不需要进行深拷贝。它可以利用 std::move 来实现高效的元素转移。
                背景：对于原始数据类型（如基本类型 int 或 char），不需要额外的深拷贝操作，直接通过内存的转移即可，这样效率更高。
            SFINAE（Substitution Failure Is Not An Error）原则
            std::false_type 构造函数： 
            这个构造函数会在容器存储的类型是非平凡类型时被选择。
            例如，如果 std::vector 存储的是自定义类型，那么编译器会选择这个构造函数来处理类型的拷贝和析构。
            因为这些类型的元素需要额外的拷贝构造和析构操作，不能直接通过 std::move 处理。

            std::true_type 构造函数： 
            当 std::vector 存储的是平凡类型（如 int, double 等），编译器会选择这个构造函数，因为这些类型的元素没有复杂的构造和析构，直接使用 std::move 就能有效地转移元素。
    */
    vector<int> vec4(std::move(vec3));// std::vector(std::vector&& other) noexcept;

    std::initializer_list iList{1, 2, 3, 4, 5};
    vector<int> vec5(iList);// vector(std::initializer_list<int> __l, const std::allocator<int> &__a)

    /*
        5.迭代器构造，可以不局限于vector自身，例如set、deque，也可以自己选择分配器
        vector<_InputIterator, <unnamed>>(_InputIterator __first, _InputIterator __last, const std::allocator<int> &__a)
    */
    std::deque iDeque{1, 2, 3, 4};
    vector<int> vec6(iDeque.cbegin(), iDeque.cend());

    return 0;
}
```

**增加函数**

```cpp
#include <iostream>
#include <vector>
#include <algorithm>
   
using std::vector;
int main(){
    vector<int> iVec(10, 5);
    int i = 10;
    iVec.push_back(i);// void push_back(const int __x)
    iVec.push_back(20);// void push_back(int &&__x)

    // auto iter = iVec.insert(++iVec.cbegin(), {1, 2, 3, 4});
    // auto iter = iVec.insert(++iVec.cbegin(), i);
    // auto iter = iVec.insert(++iVec.cbegin(), 3, 100);
    auto iter = iVec.insert(++iVec.cbegin(), 100);
    // std::vector<int, std::allocator<int>>::iterator insert(std::vector<int>::const_iterator __position, const int &__x)
    // insert()函数重载很多，但返回值都是一致的，不做赘述
    std::cout << std::distance(iVec.begin(), iter) << "\n"; // 输出1

    
    return 0;
}
```

**删除函数**

```cpp
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
```

**索引函数**

```tex
1. T& at(const size_t pos)：返回pos位置元素的引用。
2. const T& at(const size_t pos) const：at函数的常量版本。
3. T& front()：返回首元素的引用。
4. const T& front() const：front函数的常量版本。
5. T& back()：返回尾元素的引用。
6. const T& back() const：back函数的常量版本。
7. std::vector<T>::iterator begin()：返回指向容器第一个元素的迭代器。
8. std::vector<T>::const_iterator begin() const：begin函数的常量版本。
9. std::vector<T>::const_iterator cbegin() const：可以主动调用的begin函数的常量版本。
10. std::vector<T>::iterator end()：返回指向容器最后一个元素的下一个元数的迭代器。
end()函数也有两个常量版本，和begin类似，就不写了。
11. std::vector<T>::reverse_iterator rbegin()：反向迭代器，指向最后一个元素。
同样有两个常量版本。
12. reverse_iterator rend()：反向迭代器，指向第一个元素之前的元素。
同样有两个常量版本。
```

**大小函数**

```tex
1. bool empty() const：判断容器是否为空，若未空，则返回true，否则返回false。

2. size_t size() const：返回当前容器中元素的个数。

3. size_t capacity() const：返回当前容器不扩张所能容纳的最大元素数量。 capacity() >= size()

4. size_t max_size() const：返回当前机器可以存储的元素数量最大值。 取决于硬件条件
```

**其他函数**

```tex
1. void swap(std::vector<T> & vec)：交换两个同类型容器的的数据。

2. void assign(int n, const T& x)：将容器设置为n个x。

3. void assign(const_iterator first, const iterator last)：将当前容器的元素设置为[first, last)。
first,last都是迭代器，可以不是vector类型的迭代器，deque，list类型也可以。

4. void assign(const std::initialize_list<T> initList)：将容器元素设置为initialize_list的元素。

5. void resize(size_t newSize)：将容器的容量设置为newSize。
```

**deque**：

`deque`是`double end queue`的缩写；`deque`在内存的实现并非直接连续，而是分段再串接起来的分段连续，实现为多个连续的内存块（通常是一个数组的数组）；本质仍然是优化过的动态数组，只不过相比于单纯的动态数组，在前面添加或删除元素非常快了。**利用`deque` iterators模拟连续空间**，其在头部和尾部增删元素都很快（略慢于vector），在中间插入则比较耗时（和vector一样）；

`deque`的各种函数与`vector`类似，不再一一赘述，主要提及`deque`在前方操作的相关函数

```tex
1.void push_front(const T& value);

2.void push_front(T&& value);

3.void pop_front();
```

**list**：

实现为双向链表，每个元素都包含指向前后元素的指针。链表是分散的内存块，可以在任意位置插入和删除元素， **不能随机存取元素（也就是list无法用索引存取元素）**，在任何位置插入和删除元素都比较迅速（在任何位置插入删除元素的时间相同，在元素头部操作慢于`deque`，在元素尾部操作慢于`deque`和`vector`）；

**string**：

`string`可以说是各大`STL`里区别和实现最五花八门的容器，因为其本质是对字符串的封装，虽然是一个顺序容器，所以和其他容器在用法上有较大区别

(1) **`const char* c_str() const`**

- **功能**：返回一个指向`string`内部字符数据的指针，这个指针指向的是一个以 `'\0'` 结尾的 C 风格字符串。
- **注意事项**：返回的是`string`内部的字符数组指针，如果`string`的内容发生变化（例如通过修改字符串的内容），返回的指针指向的内容也会变化。
- **使用场景**：当需要将`string`对象作为C风格字符串传递给一些C语言接口（如`printf`或库函数）时，`c_str()`非常有用。

(2) **`const char* data() const`**

- **功能**：返回指向`string`内部字符数组的指针，但**与 `c_str()` 的区别在于**，返回的字符串后面没有 `'\0'` 字符。这意味着你得到的字符数组没有以`null`结尾。
- **使用场景**：当你只关心字符串内容，而不需要以 C 风格字符串的形式使用（例如传递给只需字符数组的函数时），`data()`可以提供更高效的访问。

(3) **`size_t copy(char* const ptr, size_t count, const size_t off) const`**

- 功能：将`string`对象中的一部分字符复制到一个外部的字符数组中。
  - `ptr`：目标数组的指针，复制的字符将存放在这里。
  - `count`：要复制的字符数。
  - `off`：从`string`中的哪个位置开始复制。
- **使用场景**：这种方法允许你从`string`的任意位置开始复制部分字符串，并控制复制的字符数。它在处理字符串截取时非常有用，尤其是需要将部分字符串拷贝到外部字符数组时。

2. **字符串比较函数**

`string`类提供了强大的字符串比较功能，其中最常用的是`compare`函数。

**`compare`函数**：

- 功能`compare`函数允许你比较两个`string`对象，或者将一个`string`对象的一部分与另一个字符串进行比较。
  - 它返回一个整数值，表示比较结果：
    - **返回值为0**：表示两个字符串相等。
    - **返回值小于0**：表示当前`string`对象小于比较的字符串（字典顺序上）。
    - **返回值大于0**：表示当前`string`对象大于比较的字符串。
- **重载版本**：`compare`函数有多个重载形式，支持比较整个`string`对象，也支持仅比较`string`的部分内容（通过指定起始位置和长度）。

`compare`函数提供了比直接使用`==`、`<`、`>`等运算符更灵活的方式来进行字符串的比较，尤其是在需要比较字符串的某一部分时。

**forward_list**：

单向链表，简单来说就是受限的list，凡是list不支持的功能，它都不支持。做各种支持的操作效率都会高于list，最典型的就排序算法了，forword_list要优于list。

- `forward_list` 只提供前向迭代器，而不是双向迭代器，因此它也不支持反向迭代器；
- `forward_list` 不提供成员函数 size()，因为追求效率减少开销；
- `forward_list` 没有指向最末元素的锚点，基于这个原因，不提供用以处理最末元素的成员 `back()`,`push_back()`,`pop_back()`，只有`push_front()`，`pop_front()`;

#### **关联式容器**

- **基本特性**：
  - 元素的存储顺序由元素的值决定，与插入顺序无关。
  - 内部采用“红黑树”这种高度平衡的二叉树结构实现，确保高效的查找、插入和删除操作。
- **主要类型**：
  - **`set/multiset`**
    - `set`：存储唯一的元素，自动排序。
    - `multiset`：允许存储重复的元素，自动排序。
  - **`map/multimap`**
    - `map`：存储键值对，键唯一，按键自动排序。
    - `multimap`：允许相同键的多个键值对，按键自动排序。
- **使用场景**：
  - 适用于需要有序存储且频繁进行插入和删除操作的场景。
  - 适合需要根据元素值或键值进行快速查找和有序遍历的情况。

#### 无序式容器

- **基本特性**：
  - 元素存储顺序不依赖于元素的值，而是基于哈希表的特性，实现真正的无序存储。
  - 哈希表的实现方式提供了常数时间复杂度的查找、插入和删除操作（平均情况下）。
- **主要类型**：
  - **`unordered_map/unordered_multimap`**
    - 类似于`map`和`multimap`，但基于哈希表实现，键值无序。
  - **`unordered_set/unordered_multiset`**
    - 类似于`set`和`multiset`，但基于哈希表实现，元素无序。
- **使用场景**：
  - 适用于对元素的插入和删除顺序不关心，但需要高效的查找和修改操作的场景。
  - 当需要处理大量数据且查找操作频繁时，使用无序容器可以显著提升性能。

### 5.3 迭代器

- **迭代器的介绍**：
  - **定义与功能**：迭代器是一种用于顺序访问容器中各个元素的工具。它提供了一种统一的访问方式，使得程序员无需关心不同容器的底层存储结构，就能以相同的方式遍历和操作数据。
  - **抽象化访问**：通过迭代器，用户可以抽象化地处理各种容器（如`vector`、`list`、`set`等），提高代码的通用性和可维护性。
- **迭代器的作用**：
  - **解耦设计**：迭代器使得容器、算法和迭代器自身在设计和使用时相互独立，彼此不受具体实现细节的影响。这种解耦设计提高了系统的灵活性和模块化程度。
  - **无缝耦合**：尽管容器和算法相互独立，但通过迭代器的桥梁作用，它们能够无缝地协同工作。这意味着算法可以在不同的容器上复用，而不需要针对每种容器重新设计。
  - **灵活操作**：迭代器允许用户以灵活的方式操作容器中的元素，如遍历、修改、插入和删除，而无需了解容器的具体实现。这大大简化了编程过程，提高了开发效率。

### 5.4 算法

下面简要介绍C++ STL中常用的算法，并按照常见功能进行分类，供参考与学习。

------

#### 1.查找算法（Searching Algorithms）

- **`std::find(first, last, value)`**
   在区间 `[first, last)` 中查找第一个与 `value` 相等的元素，返回迭代器。若未找到，返回 `last`。
- **`std::find_if(first, last, unary_predicate)`**
   在区间中查找第一个满足一元谓词 `unary_predicate` 的元素。
- **`std::find_if_not(first, last, unary_predicate)`**
   在区间中查找第一个不满足谓词的元素。
- **`std::search(first1, last1, first2, last2)`**
   在 `[first1, last1)` 中查找子序列 `[first2, last2)` 的第一次出现位置。
- **`std::adjacent_find(first, last)`**
   查找相邻的重复元素并返回其迭代器位置。
- **`std::binary_search(first, last, value)`**
   判断在有序区间中是否存在 `value`。存在返回 `true`，否则返回 `false`。（仅适用于有序序列）
- **`std::equal_range/ std::lower_bound/ std::upper_bound`**
   在有序序列中进行区间查找或定位元素位置。

------

#### 2.排序算法（Sorting Algorithms）

- **`std::sort(first, last)`**
   对 `[first, last)` 区间进行原地快速排序，默认使用 `<` 进行比较，若想要修改排列逻辑，需要自定义第三个参数。
- **`std::stable_sort(first, last)`**
   稳定排序，保持相等元素的原有相对顺序。
- **`std::partial_sort(first, middle, last)`**
   仅对 `[first, middle)` 排序，使之成为区间中最小（或最大）的若干元素，同时保证区间 `[middle, last)` 未必有序。
- **`std::nth_element(first, nth, last)`**
   重排元素使得 `nth` 位置上的元素是有序的（该元素左侧都不大于它，右侧都不小于它），并不保证其它元素的全序。
- **`std::is_sorted(first, last)`**
   判断区间是否已排序。

------

#### 3.删除和替换算法（Removing and Replacing）

> 注意：STL中 “删除” 类算法只是将符合条件的元素移动到区间末尾，并返回新的逻辑区间终点，真正物理删除需配合容器操作（如 `erase`）完成。

- **`std::remove(first, last, value)`**
   将区间中等于 `value` 的元素移到末尾并返回新末尾迭代器。
- **`std::remove_if(first, last, unary_predicate)`**
   将满足一元谓词的元素移到末尾并返回新末尾迭代器。
- **`std::remove_copy(first, last, dest, value)`**
   复制区间中不等于 `value` 的元素到目的区间。
- **`std::replace(first, last, old_value, new_value)`**
   将区间中等于 `old_value` 的元素替换为 `new_value`。
- **`std::replace_if(first, last, unary_predicate, new_value)`**
   将满足谓词的元素替换为 `new_value`。
- **`std::replace_copy(first, last, dest, old_value, new_value)`**
   复制同时进行替换操作，结果放入目的区间。

------

#### 4.排列组合算法（Permutation Algorithms）

- **`std::next_permutation(first, last)`**
   将 `[first, last)` 区间的元素变换为下一种字典序排列，若没有更大的排列，则变换为最小排列并返回 `false`。
- **`std::prev_permutation(first, last)`**
   将区间元素变换为上一种字典序排列，若没有更小的排列，则变换为最大排列并返回 `false`。

------

#### 5.算术算法（Numeric Algorithms）

- **`std::accumulate(first, last, init_value)`**
   对区间元素进行累加操作，初始值为 `init_value`。
- **`std::inner_product(first1, last1, first2, init_value)`**
   计算两个区间的内积（点积），并以 `init_value` 为初值。
- **`std::iota(first, last, value)`**
   从 `value` 开始递增赋值，如 `[value, value+1, value+2, ...]`。
- **`std::adjacent_difference(first, last, dest)`**
   计算相邻元素的差值，结果写入 `dest`。
- **`std::partial_sum(first, last, dest)`**
   计算部分和（前缀和），依次将累加结果放入 `dest`。

------

#### 6.生成和异变算法（Generate & Modify）

- **`std::generate(first, last, generator)`**
   使用可调用对象 `generator()` 为区间元素赋值。
- **`std::fill(first, last, value)`**
   将 `[first, last)` 区间填充为 `value`。
- **`std::swap_ranges(first1, last1, first2)`**
   交换两个等长区间内的元素。
- **`std::transform(first, last, dest, unary_op)`**
   对区间元素应用一元操作 `unary_op`，结果写入 `dest`。
- **`std::copy(first, last, dest)`** / **`std::copy_if(first, last, dest, pred)`**
   复制 / 有条件复制区间元素到目的区间。

------

#### 7.关系算法（Relational & Comparison）

- **`std::equal(first1, last1, first2)`**
   判断 `[first1, last1)` 与 `[first2, first2 + (last1-first1))` 元素是否相等。
- **`std::lexicographical_compare(first1, last1, first2, last2)`**
   以字典序比较两个区间。
- **`std::mismatch(first1, last1, first2)`**
   找到两个区间中第一个不匹配的位置。

------

#### 8.集合算法（Set Algorithms）

> 要求操作区间有序。

- **`std::set_union(first1, last1, first2, last2, dest)`**
   求并集，结果写入 `dest`。
- **`std::set_intersection(first1, last1, first2, last2, dest)`**
   求交集，结果写入 `dest`。
- **`std::set_difference(first1, last1, first2, last2, dest)`**
   求差集，结果写入 `dest`。
- **`std::set_symmetric_difference(first1, last1, first2, last2, dest)`**
   求对称差集，结果写入 `dest`。

------

#### 9.堆算法（Heap Operations）

> 针对满足堆性质的范围进行操作，常用于优先级队列等情形。

- **`std::make_heap(first, last)`**
   将 `[first, last)` 建立为一个堆，默认最大堆。
- **`std::push_heap(first, last)`**
   将区间中最后一个元素加入堆结构，使 `[first, last)` 依然满足堆性质。
- **`std::pop_heap(first, last)`**
   将堆顶元素移到区间末尾，同时 `[first, last-1)` 依然满足堆性质。
- **`std::sort_heap(first, last)`**
   对堆进行排序（执行后将不再是堆结构）。
- **`std::is_heap(first, last)`**
   判断 `[first, last)` 是否满足堆性质。

------

### 5.5 仿函数

**仿函数（Functor）**本质上就是一个重载了 `operator()` 的类对象。与普通函数不同的是，它将函数的逻辑封装在类中，并通过创建该类对象的方式来执行“函数调用”。在使用STL算法时，经常需要一个可调用对象作为参数，而STL本身也提供了大量现成的可调用对象（如 `std::plus<>`、`std::less<>`、`std::greater()` 等），方便我们直接使用，无需额外定义。这样，仿函数既具有函数的可调用性，又具备类对象的灵活性，可以在不同场景中作为策略或行为来传递。

### 5.6 适配器与分配器

1. **容器适配器**

- **定义**：容器适配器是一种对基础容器的包装，使其表现出不同的行为或功能。
- **作用**：通过适配器，可以让基础容器像栈、队列、优先级队列等特定数据结构一样工作。例如：
  - `stack<int>` 使用适配器将基础容器包装成栈（LIFO）行为。
  - `queue<int>` 将基础容器包装成队列（FIFO）行为。
  - `priority_queue<int>` 将基础容器包装成优先级队列。

------

2. STL**中的适配器类型**

​	STL 提供了三种顺序容器适配器，没有关联容器的适配器：

- **`stack`（栈）**：实现后进先出（LIFO）功能，默认使用 `deque` 容器。
- **`queue`（队列）**：实现先进先出（FIFO）功能，默认使用 `deque` 容器。
- **`priority_queue`（优先级队列）**：实现基于优先级的队列，默认使用 `vector` 容器。

------

3. **容器适配器的使用**

- **引入头文件**：

  - `#include <stack>`：使用 `stack`。
  - `#include <queue>`：使用 `queue` 或 `priority_queue`。

- **适配器与基础容器的关系**：

  - 不同的适配器有默认基础容器，但也可以指定其他支持的基础容器。
  - 适配器支持的基础容器
    - `stack`：默认 `deque`，可用 `vector` 和 `list`。
    - `queue`：默认 `deque`，可用 `list` 和 `deque`。
    - `priority_queue`：默认 `vector`，可用 `vector` 和 `deque`。

  | **适配器类型**   | **默认容器** | **可选容器**      | **说明**                                  |
  | ---------------- | ------------ | ----------------- | ----------------------------------------- |
  | `stack`          | `deque`      | `vector`, `list`  | 容器需提供 `push_back()` 和 `pop_back()`  |
  | `queue`          | `deque`      | `list`, `deque`   | 容器需提供 `push_back()` 和 `pop_front()` |
  | `priority_queue` | `vector`     | `vector`, `deque` | 容器需支持随机访问操作                    |

- **初始化方式**：

  - **普通初始化**：直接声明对象，如 `stack<int> stk;`。
  - **指定基础容器**：覆盖默认容器类型，如 `stack<int, vector<int>> stk;`。

**分配器**

- **问题**：使用 `new` 和 `delete` 动态分配内存，容易产生内存碎片化，导致性能下降。
- **解决方案**：分配器（Allocator）通过优化内存分配方式，提高堆内存的利用效率。
- **默认行为**：STL 提供了默认的分配器，一般情况下无需自定义分配器，直接使用默认分配器即可满足需求。

## 6.io库（基础）

### 6.1 io库介绍

即`input`与`output`数据在内存、磁盘、输入输出设备之间移动交互就是io功能

![image-20250107213915117](https://gitee.com/salinoia/image/raw/master/image-20250107213915117.png)

**io库组成简单介绍**

- **基础类及功能**

- **`ios` 类**：定义输入输出的最基本操作，是所有IO类的基类。
  - **`istream` 类**：从输入设备（如键盘）读取数据到内存，常用对象：`cin`。
  - **`ostream` 类**：将数据从内存输出到输出设备（如显示器），常用对象：`cout`。
  - **`iostream` 头文件**：引入了 `istream` 和 `ostream` 类及其对象，直接支持 `cin` 和 `cout`。

- **文件流功能**

  - **`ifstream` 类**（继承自 `istream`）：实现从磁盘文件读取数据到内存。

  - **`ofstream` 类**（继承自 `ostream`）：实现从内存写入数据到磁盘文件。

  - **`fstream` 头文件**：引入了 `ifstream` 和 `ofstream` 类，支持文件的输入输出。

- **字符串流功能**

  - **`istringstream` 类**（继承自 `istream`）：实现从字符串读取数据到内存。

  - **`ostringstream` 类**（继承自 `ostream`）：实现从内存写入数据到字符串。

  - **`sstream` 头文件**：引入了 `istringstream` 和 `ostringstream` 类，支持字符串与内存的交互。

------

通过这三个核心头文件（`iostream`、`fstream`、`sstream`），C++ IO库实现了设备、文件、字符串与内存间的数据流动功能。

### 6.2 io库注意事项

1.**`io`对象无法使用拷贝构造函数和赋值运算符**

所以使用流对象无法使用值传递，一般使用引用进行传递，在重载输出运算符时都是使用引用

- `io` 对象通常与底层的操作系统资源（如文件句柄、标准输入/输出流、网络连接等）相关联。这些资源在 C++ 中通常是**不可共享的**或**状态敏感的**。如果允许拷贝构造和赋值操作，可能会导致多个 `io` 对象同时访问同一资源，导致以下问题：
  - **数据竞争**：两个对象同时向同一个流写入数据会产生未定义行为。
  - **资源管理问题**：多个对象释放同一个资源（如文件句柄）可能导致重复释放错误。

------

2.**`io`对象的状态**

- **`io`操作是非常容易出现错误的操作**

​	一些错误是可以修复的，另一部分则发生在系统更深处，已经超出了应用程序可以修正的范围；

​	且由于C++的设计逻辑是尽早暴露错误，所以必要的错误检查是相当重要的	

​	代码示例如下：

```cpp
#include <iostream>
#include <limits>
#include <stdexcept>


int main(){
    int i;
    while(std::cin >> i , !std::cin.eof()){// 等价于while(expression1, expression2){ expression1; ......}
        if(std::cin.bad()){// 系统性错误,无法修复
            throw std::runtime_error("cin is corrupted");
        }
        else if(std::cin.fail()){// 格式性错误，可以修复
            std::cin.clear();// 清除状态栏
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// 忽略缓冲区
            // std::basic_istream<char, std::char_traits<char>>::__istream_type & ignore(std::streamsize __n)
            std::cout << "data format error, please try again" <<std::endl;
            continue;
        }
        std::cout << i << std::endl;
    }
    std::cout << "process complete" << std::endl;
    return 0;
}
```

​	**所以在使用io对象时都应该判断io对象的状态。**

​	**比如：while(cin >> val)** **或if(cin >> val)（不要只用这两个进行控制，最好搭配iostate来使用）**

- `iostate`

`iostate` 是 C++ 输入/输出（I/O）库中定义的一个类型，它表示流对象的状态标志。它用于描述流的当前状态，例如流是否处于错误状态、到达文件末尾等。

`iostate` 是一个位域类型（bitmask type），定义在 `<ios>` 头文件中，是 `std::ios_base` 类的成员类型。常用的状态标志如下：

| 状态标志                 | 含义                                                     |
| ------------------------ | -------------------------------------------------------- |
| `std::ios_base::goodbit` | 表示流处于正常状态，无错误。                             |
| `std::ios_base::eofbit`  | 表示流到达输入的末尾（EOF，End of File）。               |
| `std::ios_base::failbit` | 表示非致命错误，例如流读取或写入失败（通常是格式错误）。 |
| `std::ios_base::badbit`  | 表示严重错误，例如硬件故障、不可恢复的流错误。           |

流对象通过以下函数来检查其状态：

- **`good()`**

  - 检查流是否处于正常状态（无任何错误标志）。
  - 返回 `true` 表示流处于正常状态。

  ```cpp
  if (stream.good()) {
      // Stream is in good state
  }
  ```

- **`eof()`**

  - 检查流是否到达了文件末尾。
  - 返回 `true` 表示到达了 EOF。

  ```cpp
  if (stream.eof()) {
      // End of file reached
  }
  ```

- **`fail()`**

  - 检查流是否处于失败状态（包括 `failbit` 和 `badbit`）。
  - 返回 `true` 表示流操作失败。

  ```cpp
  if (stream.fail()) {
      // Stream operation failed
  }
  ```

- **`bad()`**

  - 检查流是否处于严重错误状态（`badbit`）。
  - 返回 `true` 表示发生了不可恢复的错误。

  ```cpp
  if (stream.bad()) {
      // Serious error occurred
  }
  ```

- **`rdstate()`**

  - 返回流的当前状态，类型为 `iostate`。
  - 可以通过位操作检查多个状态。

  ```cpp
  std::ios_base::iostate state = stream.rdstate();
  if (state & std::ios_base::failbit) {
      // Stream has failbit set
  }
  ```

- **`clear()`**

  - 清除流的错误状态，将流重置为正常状态。
  - 可以通过传入参数设置新的状态。

  ```cpp
  stream.clear(); // 清除所有错误状态
  stream.clear(std::ios_base::failbit); // 设置为 failbit
  ```

以下示例展示了如何使用 `iostate` 和流状态函数：

```cpp
#include <iostream>
#include <fstream>
#include <limits>

int main() {
    std::ifstream file("example.txt");
    if (!file) {
        std::cerr << "Error opening file!" << std::endl;
        return 1;
    }

    int value;
    while (file >> value) {
        std::cout << "Read value: " << value << std::endl;
    }

    if (file.eof()) {
        std::cout << "End of file reached." << std::endl;
    } else if (file.fail()) {
        std::cout << "Non-critical input error (e.g., format issue)." << std::endl;
        file.clear(); // 清除错误标志
        file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (file.bad()) {
        std::cerr << "Critical I/O error occurred." << std::endl;
    }

    file.close();
    return 0;
}
```

------

其实最主要的用法还是上面那段代码，记熟了至少acm模式没啥问题。

### 6.3 iostream

比较常用的就是这个getline了，getline其实并不复杂，不过是按行接收数据罢了，**因为存储在string对象中，所以不容易出现格式错误，但仍然可能出现系统错误**

```cpp
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>


int main(){
    std::string str;
    // char c;
    // std::cin.get(c);
    while(std::getline(std::cin, str), !std::cin.eof()){//  现代C++首选方式
        if(std::cin.bad()){// string不会出现格式错误，所以不用检查fail()
            throw std::runtime_error("cin is corrupted");
        }
        std::cout << str << std::endl;
    }
    std::cout << "process complete" << std::endl;
    return 0;
    
}
```

### 6.4 fstream

**io库默认没有给ifstream和ofstream类提供对象，需要自己定义**：
示例代码：

```cpp
#include <iostream>
#include <fstream>
#include <string>
#include <limits>

int main(){
    std::string fileName;
    std::string fileContent;
    while(std::cin>>fileName, !std::cin.eof()){
        if(std::cin.bad()){
            throw std::runtime_error("cin is corrupted");
        }

        std::ifstream ifs(fileName);
        if(ifs.is_open()){
            while(std::getline(ifs, fileContent)){
                std::cout << fileContent << std::endl;
            }
            if(ifs.bad()){
                throw std::runtime_error("ifs is corrupted");
            }
            ifs.close();// fsteam的内存分配位于操作系统内核态，必须由程序员手动释放
        }
        else{
            ifs.clear();
            ifs.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "file not exists, please try again" << std::endl;
            continue;
        }
    }
    std::cout << "process complete" << std::endl;
    return 0;    
}
```

- **文件模式**

  - `in`以读的方式打开，常与 `std::ifstream` 关联，文件必须存在，否则打开失败

  - `out`以写的方式打开，常与 `std::ofstream` 关联，默认会截断文件内容，如果需要保留内容，需同时指定 `app` 或 `in`

  - `app` 在进行写操作时定位到文件末尾，常用于日志文件或追加数据

  - `ate` 打开文件后立即定位到文件末尾，允许从任意位置读写

  - `trunc `截断文件（也就是文件已有的全部删除，重新开始写）

  - `binary` 以二进制方式打开文件，适合处理非文本数据（如图像、音频等）

- **默认操作方式**
  
  - 与`ifstream` 关联的文件默认`in`模式打开
  
  - 与`ofstream` 关联的文件默认`out`模式打开
  
  - 与`fstream`关联的文件默认`in`和`out`模式打开
  
  - 默认情况下，即使我们没有指定`trunc`，以`out`模式打开的文件也会被截断
  
    为了保持以out模式打开的文件的内容，必须同时指定`app`模式或`in`模式
  
  - 只可以对`ifstream`或`fstream`的对象设定`in`的模式
  
  - 只可以对`ofstream`或`fstream`的对象设定`out`的模式
  
  - 只有当`out`模式被设置时才可以设置`trunc`模式
  
  - `ate`和`binary`模式可以应用与任何类型的文件流对象，且可以与任何其它文件模式组合使用

最主要使用的场景还是`app(Append)`和`binary()`。

### 6.5 sstream

`istringstream`从string对象读取数据

`ostringstream`向string对象写数据

`stringstream`既可以从string对象读取数据，也可以向string对象写数据

```cpp
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

int main(){
    std::istringstream istrStream("hello worlld");
    istrStream.str("1212332");// 返回void
    std::ostringstream ostrStream;
    std::stringstream strStream;// 文件流默认无关联，字符串流默认关联空字符串
    std::cout << istrStream.str() << std::endl;// 返回值，string的拷贝，不是引用
    return 0;
}
```

**string流常见作用**

`string->int`

```cpp
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

int main(){
    std::istringstream istrStream("hello worlld");
    istrStream.str("1212");
    int i = 0;
    istrStream >> i;
    if(istrStream.bad()){
        throw std::runtime_error("istringstream is corrupted");
    }
    else if(istrStream.fail()){
        istrStream.clear();
        istrStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "streamm format incorrect" << std::endl;
    }
    else{
        std::cout << i << std::endl;
    }
    return 0;
}
```

`int->string`

```cpp
#include <iostream>
#include <sstream>
#include <string>
#include <limits>

int main(){
    int strI = 10;
    std::ostringstream ostrStream;
    ostrStream << strI << std::endl;// 此处需要使用endl刷新缓存区，而不能简单使用换行
    if(ostrStream.bad()){
        throw std::runtime_error("ostringstream is corrupted");
    }
    else{
        std::cout << ostrStream.str();
    }
    return 0;
}
```

## 7.多线程（基础）

### 7.1 基本概念

多线程是C++开发必须掌握的模块，现代程序都是多线程程序了。因为单核处理器的性能早已经达到了瓶颈，只能往多核方向发展；

对于一个计算机来说，是不是说核越多好呢？不是，多核会导致单核的工作性能下降。当核数多到一定程度后，反而总体运行效率下降了。不过，这并不影响现代计算机核数越来越多的趋势；现代网络通信，音频，视频，游戏服务器都是多线程程序。

------

**并发与并行**

- 并行是指有多个处理器。每个处理器各执行一个线程，互不抢占CPU资源，如果线程数量多于CPU，也没有办法，只能将处理器的时间划分为多个时间段，再将时间段分配给各个线程。

- 并发是指只有一个处理器，但多个线程被轮换快速执行，使得宏观上有了同时执行的效果。作用原理是将单处理器的时间划分为多个时间段，再分配给不同的线程。同一时间段只能有一个线程在运行，其它线程均处于挂起状态。

------

**进程与线程**

**线程是调度的基本单位，而进程则是资源拥有的基本单位**。

- **进程**（Process），是指计算机中已执行的程序，曾经是分时系统的基本运作单位。在面向进程设计的系统（如早期的UNIX，Linux 2.4及更早的版本）中，是程序的基本执行实体；在面向线程设计的系统（如当代多数操作系统、Linux 2.6及更新的版本）中，进程本身不是基本执行单位，而是线程的容器。

  - 进程是计算机中的程序对某些数据集合的一次运行活动，是系统进行资源分配和调度的最基本单位，是操作系统的结构基础。再用大白话说一遍，一个可执行程序执行起来，就是一个进程。当然，一个程序要执行起来需要各种资源，这些资源就是数据集合。

  - 在**面向进程设计的计算机结构**中，进程是程序的基本执行单位，进程包括程序执行的所有资源，同时自己也可以执行。

  - 在**面向线程设计的计算机结构**中，线程才是程序的基本执行单位，进程不过是线程的容器罢了。进程就像一个仓库，里面存放了程序的所有资源，进程中的线程才是真正执行程序的单元。

- **线程**（Thread），**是进程当中的一条执行流程**，将进程划分为两个或多个线程（实例）或子进程，由单处理器（单线程）或多处理器（多线程）或多核处理系统并发执行。

| 特性             | **Linux**                              | **macOS**                                      | **Windows**                                           |
| ---------------- | -------------------------------------- | ---------------------------------------------- | ----------------------------------------------------- |
| **内核类型**     | 开源的 Unix-like 内核                  | Unix-like 内核                                 | 专有的、基于 NT 内核                                  |
| **系统架构**     | 主要是基于 **进程** 的设计             | 主要是基于 **进程** 的设计                     | 主要是基于 **线程** 的设计                            |
| **进程管理**     | 支持多任务和多进程，进程间隔离         | 支持多任务和多进程，进程间隔离                 | 支持多任务和多进程，进程间隔离                        |
| **线程管理**     | **线程是轻量级进程，支持 POSIX 线程**  | 线程是进程的执行单元，支持 POSIX 和 Cocoa 线程 | **线程是最小的调度单位**，支持 Pthreads 和 Win32 线程 |
| **调度器**       | 基于 **进程** 调度，内核调度支持多线程 | 基于 **进程** 调度，但线程调度优先             | 基于 **线程** 调度，线程优先                          |
| **系统调用接口** | 提供标准的 **POSIX** 接口              | 提供 **POSIX** 兼容接口和 Apple 定制接口       | 提供 **WinAPI** 接口                                  |

------

​	对于现代C++开发来说，C++11标准提供的多线程功能才是主流，其实现采取了Windows的模式，即**进程作为线程的容器，线程作为最小执行单元**，Linux和macOS同样也支持这些功能。

### 7.2 线程的创建

------

1. **主线程（Main Thread）**

​	一个程序在执行时是一个**进程**。进程的执行从**`main()`**函数开始，`main()`函数所运行的线程称为**主线程**。当主线程执行完毕，整个进程也会结束。因此，主线程是进程中的起始线程，并负责整个进程的生命周期。

2. **子线程（Child Thread）**

​	在程序的主线程中，我们可以通过创建**子线程**来实现并发执行。子线程是从主线程派生出来的独立线程，能够并行或异步地执行特定任务。虽然**子线程和主线程在执行时是平级的**，但**主线程**结束后会强制终止所有子线程，从而结束整个进程。

​	子线程的创建是通过调用`std::thread`类来完成的。`std::thread`接收一个**可调用对象**作为参数（如函数指针、Lambda 表达式等）。

3. **子线程创建**

​	创建子线程的基本方式是使用C++标准库中的`std::thread`类，并传入可调用对象。示例如下：

```cpp
#include <thread>
#include <iostream>

int main(){
    std::thread myThread([](){
        std::cout << "hello thread" << std::endl;
        /*
            ......
        */
       for(int i = 0 ;i < 1000000; ++i){
            std::cout << i <<std::endl;
       }
    });
    // myThread.join();// 主线程需要等待子线程
    myThread.detach();// 主线程不需要等待子线程
    return 0;
}
```

4. **子线程的生命周期管理**

​	当创建了子线程后，需要合理管理其生命周期，避免出现**资源泄漏**或**意外的线程终止**。

​	(1) **`join()`函数：**

​	在某些情况下，**主线程必须等待子线程执行完毕后才能继续**。`std::thread::join()`可以实现这一点。调用`join()`函数后，主线程会**挂起**，直到子线程执行完毕后才能继续执行主线程后续的代码。

```cpp
std::thread t(foo);
t.join();  // 等待子线程完成
```

​	(2) **`detach()`函数：**

​	有时候，主线程不需要等待子线程结束，子线程可以与主线程“分离”，独立执行。使用`std::thread::detach()`函数可以将子线程“分离”，此时子线程由**C++运行时库负责管理**。当主线程结束时，子线程不会被强制终止，而是由系统自动清理。使用`detach()`时需要注意，子线程在执行时的资源释放和异常处理将由C++的运行时自动管理。因此，主线程退出后，即使子线程仍在运行，程序也不会因为子线程的资源未释放而导致未定义行为。

```cpp
std::thread t(foo);
t.detach();  // 子线程与主线程分离，独立执行
```

### 7.3 线程传递参数

------

在使用`std::thread`创建子线程时，参数的传递方式有三种：

1. **值传递（Pass by Value）**：
   - 直接传递参数的副本。
   - 适用于基本数据类型（如`int`、`char`等）。
   - 示例：`std::thread t(func, value);`
2. **引用传递（Pass by Reference）**：
   - 传递参数的引用，需要使用`std::ref`封装。
   - 适用于需要在子线程中修改参数的情况，或传递大型对象以避免复制开销。
   - 示例：`std::thread t(func, std::ref(obj));`
3. **指针传递（Pass by Pointer）**：
   - 传递参数的地址。
   - 需确保指针在子线程使用期间有效，避免悬空指针。
   - 示例：`std::thread t(func, &obj);`

**代码演示**

```cpp
#include <thread>
#include <iostream>

void test(int i, const int& refI, const int* pI){
    std::cout << "i: " << i << ", refI: " << refI << ", *pI: " << *pI << std::endl;
}

int main(){
    int i = 100;
    std::thread myThread(test, i, i, &i);// 下例图像为地址变化
    myThread.join();  // 等待子线程完成
    // myThread.detach();
    return 0;
}
```

![image-20250109215757283](https://gitee.com/salinoia/image/raw/master/image-20250109215757283.png)

![image-20250109215821325](https://gitee.com/salinoia/image/raw/master/image-20250109215821325.png)

------

**传递参数的注意事项**

**(1) 使用`detach`时的指针传递**

- **问题**：
  - 当使用`detach()`将子线程与主线程分离时，主线程可能在子线程使用指针之前结束，导致指针指向的内存被回收，产生悬空指针。例如上例直接调用`myThread.detach();`：

![image-20250109220725778](https://gitee.com/salinoia/image/raw/master/image-20250109220725778.png)

- **建议**：
  - **避免**在`detach()`模式下传递指针。
  - 使用值传递或引用传递（通过`std::ref`）来确保子线程有独立的参数副本或有效的引用。
- **代码演示**：

```cpp
#include <thread>
#include <pthread.h>
#include <chrono>
#include <iostream>

class Test{
public:
    explicit Test(int i){
        std::cout << std::this_thread::get_id() << std::endl; // 线程是依附于进程存在的，所以不同的进程可以有相同的线程id
    }
};

void test(const Test& t){
    std::cout << "child thread id is: " << std::this_thread::get_id() << std::endl;
}

int main(){
    std::cout << "main thread id is: " << std::this_thread::get_id() << std::endl;
    int i = 100;
    Test t(i);
    std::thread myThread(test, t);// 线程传参的时候一定要避免隐式类型转化
    myThread.detach();  // 子线程与主线程分离
    // 主线程可能提前结束，子线程仍在运行

    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    return 0;
}
```

**(2) 使用`detach`时避免隐式类型转换**

- **问题**：
  - 隐式类型转换可能导致子线程在参数转换完成前，主线程已结束，造成未定义行为。
- **建议**：
  - 明确传递参数类型，避免隐式转换。
  - 使用`static_cast`或`reinterpret_cast`等显式转换确保类型安全。
- **代码演示**：

```cpp
#include <thread>
#include <iostream>

void test(double d){
    std::cout << "d: " << d << std::endl;
}

int main(){
    int i = 100;
    // 隐式转换将int转换为double，可能导致子线程执行前主线程结束
    std::thread myThread(test, i);
    myThread.detach();
    return 0;
}
```

**传递参数的不同情景**

**(1) 参数传递方式选择**

- **普通类型（基本数据类型）**：
  - **推荐**：值传递。
  - **原因**：开销小，简单安全。
- **类类型或大型对象**：
  - **推荐**：**引用传递，使用`std::ref`封装**。
  - **原因**：**避免复制开销**，允许子线程修改原对象。

**(2) 生命周期管理**

- **`join()`**：
  - 主线程等待子线程完成，确保子线程在主线程之前结束。
  - 适用于子线程的任务必须在主线程继续之前完成的场景。
- **`detach()`**：
  - 子线程与主线程分离，独立执行。
  - 适用于子线程任务独立于主线程，不需要主线程等待的场景。
  - **注意**：避免在`detach()`模式下传递指针或依赖主线程的资源。

**(3) 使用`std::ref`的必要性**

- **问题**：
  - 直接传递引用参数会导致`std::thread`尝试复制参数，使用普通的引用传递会调用一次复制构造函数，而不是传递引用，导致函数无法对引用对象进行修改。
- **解决方案**：
  - 使用`std::ref`封装引用，确保子线程接收到的是引用而非副本。
- **代码演示**：

```cpp
#include <thread>
#include <iostream>
#include <functional>  // 包含std::ref

void modify(int& x){// 普通类型传值，类类型传引用
    x += 10;
}

int main(){
    int i = 100;
    std::thread myThread(modify, std::ref(i));
    myThread.join();
    std::cout << "i after thread: " << i << std::endl;  // 输出110
    return 0;
}
```

### 7.4 数据共享与数据保护

多个线程的执行顺序与具体执行方法和处理器的调度机制有关，从开发者的角度来看是没有规律的，追求更多可以进一步学习协程；

**数据保护问题**

- 数据保护问题总共有三种情况：

  - 至少两个线程对共享数同时进行**读操作**，完全不会出现数据安全问题；

  - 至少两个线程对共享数据同时进行**写操作**，会出现数据安全问题，需要数据保护；

  - 至少两个线程对共享数据**有的进行读，有的进行写**，会出现数据安全问题，需要进行数据保护。

1.**汇编层面的数据操作**

在汇编语言中，对数据的操作通常分为以下几个步骤：

- **从内存加载数据到寄存器**：
   - 例如，`mov eax, [i]` 表示将内存中变量 `i` 的值加载到寄存器 `eax` 中。

- **在寄存器中进行运算**：
   - 例如，`inc eax` 表示将寄存器 `eax` 中的值加 1。

- **将结果写回内存**：
   - 例如，`mov [i], eax` 表示将寄存器 `eax` 中的值写回内存中的变量 `i`。

------

2.**同时写**

假设有两个线程（线程 A 和线程 B）同时对同一个变量 `i` 进行自增操作（`i++`），以下是可能发生的执行顺序：

- **线程 A**：
   - 将 `i` 的值加载到寄存器 `eax` 中：`mov eax, [i]`。
   - 对 `eax` 进行自增操作：`inc eax`。
   - 将 `eax` 的值写回内存：`mov [i], eax`。

- **线程 B**：
   - 将 `i` 的值加载到寄存器 `ebx` 中：`mov ebx, [i]`。
   - 对 `ebx` 进行自增操作：`inc ebx`。
   - 将 `ebx` 的值写回内存：`mov [i], ebx`。

如果线程 A 和线程 B 的执行顺序是交错的，可能会出现以下情况：

- 线程 A 和线程 B 都从内存中读取了 `i` 的初始值（例如 `i = 0`）。
- 线程 A 和线程 B 分别对寄存器中的值进行自增操作（`eax = 1`，`ebx = 1`）。
- 线程 A 和线程 B 分别将结果写回内存，最终 `i` 的值是 `1`，而不是预期的 `2`。

这就是典型的**数据竞争**问题。

**代码示例**：

```cpp
#include <thread>
#include <iostream>

unsigned g_num = 0;

void test(){
    for(unsigned i = 0; i < 10000; ++i){
        ++g_num;
    }
}

int main(){
    std::thread myThread(test);
    for(unsigned i = 0; i < 10000; ++i){
        ++g_num;
    }
    myThread.join();
    
    std::cout << g_num << std::endl;
    return 0;
}
```

输出结果：

![image-20250110102715096](https://gitee.com/salinoia/image/raw/master/image-20250110102715096.png)

------

3.**一写一读**

如果有一个线程在读数据，另一个线程在写数据，也会出现问题。例如：

1. **线程 A**（读操作）：
   - 将 `i` 的值加载到寄存器 `eax` 中：`mov eax, [i]`。

2. **线程 B**（写操作）：
   - 将 `i` 的值加载到寄存器 `ebx` 中：`mov ebx, [i]`。
   - 对 `ebx` 进行自增操作：`inc ebx`。
   - 将 `ebx` 的值写回内存：`mov [i], ebx`。

如果线程 A 和线程 B 的执行顺序是交错的，可能会出现以下情况：

- 线程 A 读取了 `i` 的值（例如 `i = 0`）。
- 线程 B 对 `i` 进行了自增操作（`i = 1`）。
- 线程 A 读取的值是 `0`，但实际上 `i` 的值已经被线程 B 修改为 `1`。

这种情况下，线程 A 读取到的数据是**过时的（Stale）**，这会导致程序逻辑错误。

------

解决方法：**互斥锁或原子操作**

**互斥锁**：

```cpp
#include <thread>
#include <iostream>
#include <mutex>

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
        但实际上会引入更多的锁操作，导致性能进一步下降，当然，若涉及到更为复杂的操作，就应该考虑将锁加上循环体内部
    */
    auto start = std::chrono::high_resolution_clock::now();
    for(unsigned i = 0; i < 10000; ++i) {
        std::lock_guard<std::mutex> lg(MyMutex);
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
```

**原子操作**：

```cpp
#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> counter(0);  // 原子计数器

void increment() {
    for (int i = 0; i < 10000; ++i) {
        counter.fetch_add(1, std::memory_order_relaxed);  // 原子自增
    }
}

int main() {
    std::thread t1(increment);
    std::thread t2(increment);

    t1.join();
    t2.join();

    std::cout << "Final counter value: " << counter << std::endl;  // 输出 20000
    return 0;
}
```

- **互斥锁**：
  - 适用于保护多个数据或复杂操作。
  - 使用频率更高，是解决数据竞争的主要工具。
- **原子操作**：
  - 适用于单个数据的简单操作（如计数器、标志位、自增、自减）。
  - 性能优于互斥锁，但无法直接保护多个数据的逻辑一致性。

### 7.5 死锁

死锁通常发生在以下四个条件同时满足时：

1. **互斥条件**：资源一次只能被一个线程占用。
2. **占有并等待**：线程持有至少一个资源，并等待获取其他被占用的资源。
3. **非抢占条件**：已分配给线程的资源不能被其他线程强行抢占，必须由线程自行释放。
4. **循环等待条件**：存在一个线程循环链，每个线程都在等待下一个线程所占用的资源。

```cpp
#include <iostream>
#include <thread>
#include <mutex>

unsigned counter = 0;
std::mutex mutex1; // 第一个互斥量
std::mutex mutex2; // 第二个互斥量

void thread_func1() {
    for (unsigned i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock1(mutex1); // 先锁 mutex1
        /*
            ...
            some code 
        */
        std::lock_guard<std::mutex> lock2(mutex2); // 再锁 mutex2
        ++counter; // 操作共享资源
    }
}

void thread_func2() {
    for (unsigned i = 0; i < 100000; ++i) {
        std::lock_guard<std::mutex> lock2(mutex2); // 先锁 mutex2
        /*
            ...
            some code 
        */
        std::lock_guard<std::mutex> lock1(mutex1); // 再锁 mutex1
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
```

解决方法：

1. 很简单，锁的顺序一致就可以了；但是实际开发中想要保证是一件很难的事情，因为往往一段程序的加锁是相隔很远的；
2. 可以引入`std::lock`和`std::adopt_lock`

```cpp
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
```

## 8.异常处理

### 8.1 异常介绍

- **异常的定义**：**异常是指程序在执行过程中遇到的特殊情况或错误**。与编译期错误不同，异常通常发生在程序运行时。
- **异常处理的关键字**：
  - **throw**：用于抛出异常，当程序遇到问题时会使用 `throw` 关键字抛出一个异常对象。
  - **try**：用于包围可能抛出异常的代码块。`try` 块中的代码会被监视，如果有异常发生，程序将跳转到相应的 `catch` 块进行处理。
  - **catch**：用于捕获 `try` 块中抛出的异常，并进行相应的处理。`catch` 块指定了捕获异常的类型。
- **异常处理的基本结构**：
   典型的异常处理结构是将可能出现异常的代码放在 `try` 块中，然后通过一个或多个 `catch` 块来捕获并处理不同类型的异常。
- **异常处理的作用**：
   异常处理机制使得程序在遇到错误时可以通过跳转到相应的处理代码块来避免程序崩溃，同时也可以在 `catch` 块中做一些必要的资源清理、错误日志记录等操作。

常见异常处理的场景：

- **算术异常**（Arithmetic Exception）

  ```cpp
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
  ```

- **文件相关异常**（File-related Exception）

  ```cpp
  #include <iostream>
  #include <fstream>
  #include <stdexcept>
  
  void outputFileContent(const std::string& filename){
      std::ifstream ifs(filename);
      std::string fileLineContent;
      if(ifs.is_open()){
          while(std::getline(ifs, fileLineContent)){ // 按行读取文件内容
              std::cout << fileLineContent << std::endl;
          }
          if(ifs.bad()){
              throw std::runtime_error("file is corrupted");
          }
          ifs.close();
      }
      else{
          if(ifs.fail()){
              throw std::runtime_error("file not exists");
          }
          else if(ifs.bad()){
              throw std::runtime_error("ifs is corrupted");
          }
      }
  }
  
  int main(){
      std::string str;
      std::cin >> str;
      try{
          outputFileContent(str);
      }
      catch(const std::runtime_error& e){
          std::cout << "Error: " << e.what() << std::endl; // 捕获输入流错误的异常
      }
      catch(...){
          std::cout << "Unknown error occurred." << std::endl; // 捕获其他未知异常
      }
      return 0;    
  }
  ```

- **内存分配异常**（Memory Allocation Exception）

  ```cpp
  #include <iostream>
  #include <fstream>
  #include <memory>
  #include <vector>
  
  int main(){
      std::vector<std::shared_ptr<int>> unique_int_vec;
      try{
          while(true){
              // 如果内存耗尽，优先抛出 std::bad_alloc 异常，并输出 std::bad_alloc。
  			// 如果系统触发 OOM Killer，则直接终止程序并显示 "Killed"
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
  ```

- **越界异常**（Out-of-bounds Exeception）

  ```cpp
  #include <iostream>
  #include <vector>
  #include <stdexcept> // For std::out_of_range
  
  template<typename T>
  class Test {
  public:
      const T& operator[](size_t index) const;
      void add(const T& value); // 添加一个元素到data
  private:
      std::vector<T> data;
  };
  
  template<typename T>
  const T& Test<T>::operator[](size_t index) const {
      if (index >= data.size()) {
          throw std::out_of_range("Accessing elements beyond valid range in a template class.");
      }
      return data[index];
  }
  
  template<typename T>
  void Test<T>::add(const T& value) {
      data.push_back(value);
  }
  
  int main() {
      try {
          Test<int> test;
  
          // 添加一些元素到容器
          test.add(10);
          test.add(20);
  
          // 正常访问
          std::cout << "Element at index 0: " << test[0] << std::endl;
  
          // 越界访问，触发异常
          std::cout << "Element at index 2: " << test[2] << std::endl;
      }
      catch (const std::out_of_range& e) {
          std::cout << "Exception caught: " << e.what() << std::endl;
      }
      /*
          Element at index 0: 10
          Element at index 2: Exception caught: Accessing elements beyond valid range in a template class.
      */
      return 0;
  }
  ```
  

### 8.2 标准异常

定义在头文件`<exception>`中，C++标准库的异常类从 **`std::exception`** 派生出多个子类，按照功能划分为以下几类：

**1. 内存相关异常**

- **`std::bad_alloc`**
  - 继承自 `std::exception`。
  - **用途**：在使用 `new` 分配内存失败时抛出。
  - **场景**：内存不足或分配失败。

##### **2. 类型相关异常**

- **`std::bad_cast`**：
  - **用途**：在使用 `dynamic_cast` 类型转换失败时抛出。
  - **场景**：需要强制转换派生类和基类之间的指针或引用。
- **`std::bad_typeid`**：
  - **用途**：当使用 `typeid` 操作符的对象为 `nullptr` 时抛出。
  - **场景**：用于 RTTI（运行时类型识别）场景。

**3. 容器和边界相关异常**

- **`std::out_of_range`**：
  - **用途**：当容器访问超出范围时抛出，例如 `std::vector::at`。
  - **场景**：边界检查更严格时使用。
- **`std::length_error`**：
  - **用途**：当容器的大小超出最大长度限制时抛出。
  - **场景**：容器大小限制检查。

**4. 逻辑错误相关异常**

- **`std::logic_error`**
  - 继承自 `std::exception`，是逻辑错误的基类。
  - 派生类
    - **`std::domain_error`**：表示无效的数学域，例如平方根的负数。
    - **`std::invalid_argument`**：传递的参数无效。
    - **`std::length_error`**：容器长度无效。
    - **`std::out_of_range`**：访问超出范围。

**5. 运行时错误相关异常**

- **`std::runtime_error`**
  - 继承自 `std::exception`，是运行时错误的基类。
  - 派生类
    - **`std::overflow_error`**：表示算术运算溢出。
    - **`std::underflow_error`**：表示算术运算下溢。
    - **`std::range_error`**：表示计算结果超出范围。

**常用的标准异常**

1. **`std::bad_alloc`**：

   - **场景**：内存分配失败时自动抛出。

   - 示例代码

     ```cpp
     try {
         int* arr = new int[100000000000000];
     } catch (const std::bad_alloc& e) {
         std::cout << e.what() << std::endl;
     }
     ```

2. **`std::out_of_range`**：

   - **场景**：使用容器的 `at` 方法访问越界时抛出。

   - 示例代码

     ```cpp
     try {
         std::vector<int> vec = {1, 2, 3};
         std::cout << vec.at(5) << std::endl; // 越界
     } catch (const std::out_of_range& e) {
         std::cout << e.what() << std::endl;
     }
     ```

3. **`std::runtime_error`**：

   - **场景**：用于表示程序运行中出现的不可预测错误。

   - 示例代码

     ```cpp
     try {
         throw std::runtime_error("Custom runtime error");
     } catch (const std::runtime_error& e) {
         std::cout << e.what() << std::endl;
     }
     ```

4. **`...`（通用异常捕获）**：

   - **场景**：捕获所有未被明确捕获的异常。

   - 示例代码

     ```cpp
     try {
         throw 42; // 任意类型的异常
     } catch (...) {
         std::cout << "Caught an unknown exception" << std::endl;
     }
     ```

**较少使用但特定场景需要的标准异常**

1. **`std::domain_error`**：

   - **场景**：数学计算输入参数非法。

   - 示例代码

     ```cpp
     try {
         throw std::domain_error("Invalid domain");
     } catch (const std::domain_error& e) {
         std::cout << e.what() << std::endl;
     }
     ```

2. **`std::overflow_error` 和 `std::underflow_error`**：

   - **场景**：算术溢出或下溢。

   - 示例代码

     ```cpp
     try {
         throw std::overflow_error("Overflow occurred");
     } catch (const std::overflow_error& e) {
         std::cout << e.what() << std::endl;
     }
     ```

------

**自定义异常（简要）**

虽然标准异常覆盖了绝大多数场景，但有时需要自定义异常。自定义异常通常继承自 `std::exception` 或其他标准异常类：

**示例代码**：

```cpp
class CustomException : public std::exception {
public:
    const char* what() const noexcept override {
        return "Custom exception occurred";
    }
};

int main() {
    try {
        throw CustomException();
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
```

------

## 9.万能引用与完美转发

### 9.1 万能引用

**定义**：

万能引用指的是一种 **既可以绑定到左值，也可以绑定到右值的引用**，但它的行为取决于使用的上下文。在 **模板类型推导** 过程中，编译器会根据传入参数的实际类型，将万能引用解析为 **左值引用 (`T&`)** 或 **右值引用 (`T&&`)**。

**关键点**：

1. **不是新类型**：

   - 万能引用并不是一种新的引用类型。
   - 一个引用 **要么是左值引用 (`T&`)，要么是右值引用 (`T&&`)**，万能引用的特殊性在于它的上下文敏感性。

2. **语法**：

   - **模板形式的万能引用**：

     ```cpp
     template<typename T>
     void func(T&& parm);
     ```

     - 只有当 `T` 是由编译器推导得出的类型时，`T&&` 才是万能引用。
     - 如果 `T` 是显式指定或固定的，`T&&` 就是普通的右值引用。

   - **auto 形式的万能引用**：

     ```cpp
     auto&& var = var2;
     ```

     - 这里的 `auto&&` 是万能引用，因为 `auto` 的类型是从 `var2` 的实际类型推导而来的。

3. **非万能引用的情况**：

   - `const T&& parm`：普通 **右值引用**，因为 `T` 被 `const` 修饰。

   - `std::vector<T>&& parm`：普通 **右值引用**，因为类型绑定到容器类。

   - 如果 `T` 与模板类绑定（如 `MyVector<T>`），则 `T&&` 在成员函数中**也不是万能引用**，除非成员函数本身也是模板。

     ```cpp
     template<typename T>
     class MyVector{
     public:
         template<typename T>
         void push_back(T&& elem);// 并非万能引用
     
         template<typename T2>
         void push_back(T2&& elem);// 万能引用
     };
     ```

     

**代码示例**：

- **模板形式的万能引用**：

  ```cpp
  template<typename T>
  void func(T&& parm) {
      if constexpr (std::is_lvalue_reference<T>::value) {
          std::cout << "左值引用" << std::endl;
      } else {
          std::cout << "右值引用" << std::endl;
      }
  }
  
  int x = 10;
  func(x);       // 左值引用
  func(10);      // 右值引用
  ```

- **auto 形式的万能引用**：

  ```cpp
  int x = 10;
  auto&& ref = x;     // ref 是左值引用
  auto&& ref2 = 10;   // ref2 是右值引用
  // const auto&& elem = x; // 报错，此时固定为右值 
  ```

------

**万能引用的使用场景**

万能引用的主要作用是实现 **完美转发**。完美转发确保模板函数在将参数传递给另一个函数时，可以保留参数的左值/右值特性。

- **完美转发示例**：

  ```cpp
  template<typename T, typename... Args>
  std::shared_ptr<T> create(Args&&... args) {
      return std::make_shared<T>(std::forward<Args>(args)...);
  }
  
  create<std::vector<int>>(10, 0);  // 完美转发参数
  ```

------

**引用折叠**

引用折叠是指当多个引用限定符（`&` 或 `&&`）在类型推导过程中组合在一起时，它们会被折叠成一个单一的引用类型。C++11 的引用折叠规则简化并标准化了这种行为。

**折叠规则**：

当 `&` 和 `&&` 组合时：

- `T& &` 折叠为 `T&`。
- `T& &&` 折叠为 `T&`。
- `T&& &` 折叠为 `T&`。
- `T&& &&` 折叠为 `T&&`。

简而言之：

- **只要有一个 `&`，最终就是 `T&`（左值引用）。**
- **只有 `&& &&` 会折叠为 `T&&`（右值引用）。**

**示例**：

- **代码示例，展示引用折叠**：

  ```cpp
  template<typename T>
  void func(T&& parm) {
      T& lref = parm;   // 始终有效：折叠为左值引用
  }
  
  int x = 10;
  func(x);       // T = int&，所以 T&& = int&&& = int&
  func(10);      // T = int，所以 T&& = int&&
  ```

- **显式引用折叠**：

  ```cpp
  template<typename T>
  void wrapper(T&& parm) {
      func(std::forward<T>(parm));  // 使用引用折叠实现完美转发
  }
  ```

**万能引用**：

- 万能引用依赖模板类型推导（或 `auto`），能够根据上下文分别表现为左值引用或右值引用。
- 主要用于 **通用函数模板** 和 **完美转发**。

**引用折叠**：

- 引用折叠是 C++11 的一项基础规则，用于解决多重引用修饰符的组合问题。
- 是实现万能引用和完美转发的关键技术。

### 9.2 完美转发

------

**1. 完美转发的定义**

完美转发是 C++11 引入的一种技术，用于在函数中将传递的参数 **原封不动** 地传递给另一个函数。这种 **"原封不动"** 不仅包括参数的值，还包括：

- 参数的 **类型**（例如左值、右值）。
- 参数的 **限定符**（如 `const`、`volatile`）。
- 参数是否为 **引用**。

**关键点**：

- 完美转发主要用于模板函数，确保参数的属性在传递过程中不发生变化。
- 实现完美转发的核心工具是 **万能引用**（`T&&`）和 `std::forward`。

------

**2. 问题：传统参数传递的问题**

在 C++ 中，使用普通的值传递、引用传递、或者指针传递，往往会丢失参数的一些属性。例如，无法区分左值和右值，或者限定符丢失。

**示例代码：传统方法的问题**

```cpp
#include <iostream>

void print(int& x) {
    std::cout << "lvalue reference" << std::endl;
}

void print(int&& x) {
    std::cout << "rvalue reference" << std::endl;
}

template<typename T>
void forwardToPrint(T&& arg) {
    // print(arg); // 无法完美传递，arg 被视为左值引用
    print(std::move(arg)); // std::move(arg) 均被视为右值引用
}

int main() {
    int x = 10;
    forwardToPrint(x);   // 输出：lvalue reference（正确）
    forwardToPrint(10);  // 输出：lvalue reference（错误，应该是 rvalue reference）
}
```

**原因**：

- 在模板函数中，`arg` 是一个万能引用（`T&&`），但在 `print(arg)` 的调用中，`arg` 是一个 **左值引用**，因为它有名字（命名规则：有名字的变量总是左值）。
- 这导致了右值（如 `10`）被错误地传递为左值。
- 如果此时将`print(arg)`修改为`print(std::move(arg))`，就全部变成右值转发

------

**3. 解决方法：使用 `std::forward` 实现完美转发**

`std::forward` 是一个模板工具，用于根据模板参数推导，保留参数的原始属性（左值、右值以及限定符）。通过 `std::forward`，可以将参数准确地传递给另一个函数。

**示例代码：使用 `std::forward` 实现完美转发**

```cpp
#include <iostream>
#include <utility> // std::forward

void print(int& x) {
    std::cout << "lvalue reference" << std::endl;
}

void print(int&& x) {
    std::cout << "rvalue reference" << std::endl;
}

template<typename T>
void forwardToPrint(T&& arg) {
    print(std::forward<T>(arg)); // 完美转发
}

int main() {
    int x = 10;
    forwardToPrint(x);   // 输出：lvalue reference
    forwardToPrint(10);  // 输出：rvalue reference
}
```

**解析**：

- `std::forward<T>(arg)`
  - 如果 `T` 是左值引用类型（`T&`），`std::forward<T>` 会将参数作为左值传递。
  - 如果 `T` 是右值引用类型（`T&&`），`std::forward<T>` 会将参数作为右值传递。
- `std::forward` 可以看作是 **条件转移（conditional move）**，确保参数的属性在传递过程中不会丢失。

------

以下代码展示了完美转发在构造函数中的应用：

```cpp
#include <iostream>
#include <string>
#include <utility

class MyClass {
public:
    template<typename T>
    MyClass(T&& name) : name_(std::forward<T>(name)) {} // 完美转发到成员变量

    void printName() const {
        std::cout << "Name: " << name_ << std::endl;
    }

private:
    std::string name_;
};

int main() {
    std::string str = "Hello";
    MyClass obj1(str);        // 左值传递
    MyClass obj2("World");    // 右值传递

    obj1.printName(); // 输出：Name: Hello
    obj2.printName(); // 输出：Name: World
}
```

**关键点**：

- 构造函数中 `T&&` 是万能引用。
- 使用 `std::forward<T>` 将 `name` 的属性（左值或右值）完整地传递到成员变量 `name_` 中。
- 通过完美转发避免了不必要的拷贝操作，提高了性能。


# LiteSTL
## Introduction
This project aims to accurately reproduce the Standard Template Library (STL) in C++, while also considering the addition of alternative templates with different underlying implementations (e.g., `map` containers based on balanced binary trees or skip lists). The implementations in this project adhere to the following principles:

1. **Full Replication of All Public Members**: This includes reproducing all member functions and their overloads, ensuring that code using the STL can switch to this project's `namespace` without any modifications and produce identical results.

2. **Error Handling Mimicking STL**: The error handling will be replicated such that the runtime errors encountered in the original STL code will be the same when switching to this project's `namespace`.

3. **Consistent Logic and Time Complexity**: The code logic will be modeled after the STL, striving to maintain the same time complexity as the original implementations. However, the reproduced code will inevitably differ from the STL, such as not including the same complex wrappers, meaning that runtime performance may not exactly match the STL.

Within the constraints of these principles, the project may introduce minor modifications to the containers, such as adding frequently used methods or additional error messages to help users identify the cause of errors.

Furthermore, the STL implementation referenced in this project is based on the library files provided by Visual Studio, which may differ from the underlying implementations of other compilers.

## 介绍
本项目旨在高精度复现C++中的标准模板库(STL)，同时也会考虑添加一些底层实现不同的模板（例如基于平衡二叉树或跳表的`map`容器），本项目中所有的实现会基于以下原则：

1. **完全复现所有共有成员**：复现所有的公有方法（包括他们的所有重载），从而使得将使用了STL的代码中相应的名字空间替换为本项目的名字空间`namespace`后无需进行任何修改，且运行结果一致。

2. **仿照STL处理报错**：使得名字空间`namespace`替换之前代码在运行时会反馈的错误在替换后仍会反馈相同错误

3. **与STL相同的代码逻辑**：尽可能使得复现与STL中的实现的时间复杂度一致。然而，本项目复现的代码一定和STL实现有差异，例如不会像STL中一样进行复杂的包装，因此运行时间不会和STL实现完全一致。

在满足上述原则的前提下，本项目可能会对容器进行少量修改，例如增加一些使用频率较高的方法，或是增加一些报错信息方便用户甄别报错原因。

另外，本项目所参考的STL实现源于Visual Studio提供的库文件，可能会与其他编译器的底层实现有所区别。

## Progress
### vector
The latest version is 0.5. 

In this version, all public functions (including overloads) in vector has already been implemented, but have not yet been tested for stability and efficiency. 

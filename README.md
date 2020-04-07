# FgyTinySTL
* 为了学习c++泛型语法以及数据结构算法，参考一些教程实现自己的TinySTL<br>

## 学习资料
* 侯捷 《STL源码剖析》
* [zouxiaohang/TinySTL](https://github.com/zouxiaohang/TinySTL)
* SGI STL源码（有需要自己下载）
* 各种博客<br>

## 实现环境
* win10（后期转到Ubuntu）
* IDE：CLion
* 环境：MinGW

## 实现过程
### 1.1 空间配置器部分
* 包含文件：Alloc.h、Allocator.h、Construct.h、Alloc.cpp<br>（代码内有注释）
实现了两级空间配置器:[alloc.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Alloc.h); 实现细节:[implement_detail](https://github.com/fgy1995/FgyTinySTL/tree/master/implement_detail)<br>
### 1.2 测试结果
* 使用allocator类对内置类型，内置类型的数组，自定义类，自定义类型的数组进行空间配置以及construct。结果都可以进行正常地配置和construct。

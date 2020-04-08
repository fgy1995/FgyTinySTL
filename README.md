# FgyTinySTL
* 为了学习c++泛型语法以及数据结构算法，参考一些教程实现自己的TinySTL<br>

## 学习资料
* 侯捷 《STL源码剖析》
* Stanley B.Lippman 《C++ Primer》（第五版）参考书，哪里语法忘了参考下
* [zouxiaohang/TinySTL](https://github.com/zouxiaohang/TinySTL)
* SGI STL源码（有需要自己下载）
* 各种博客<br>

## 实现环境
* win10（前期家里电脑带不动虚拟机，后期回学校转到Ubuntu）
* IDE：CLion（Ubuntu用Sublime写的）
* 环境：MinGW

## 实现过程
### 1.1 空间配置器部分
* 包含文件：Alloc.h、Allocator.h、Construct.h、Alloc.cpp（代码内有注释）<br>
实现了两级空间配置器:[alloc.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Alloc.h); 实现细节:[implement_detail/alloc.cpp](https://github.com/fgy1995/FgyTinySTL/tree/master/implement_detail); 对外接口:[Allocator.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Allocator.h); 构造器:[Construct.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Construct.h)<br>
### 1.2 空间配置的流程(自己画的)
![](https://github.com/fgy1995/FgyTinySTL/blob/master/picture/%E5%88%86%E7%BA%A7%E7%A9%BA%E9%97%B4%E9%85%8D%E7%BD%AE%E6%B5%81%E7%A8%8B.jpg)
### 1.3 测试结果
* 使用allocator类对内置类型，内置类型的数组，自定义类，自定义类型的数组进行空间配置以及construct。结果都可以进行正常地配置和construct。

### 2.1 迭代器与traits技法
* 包含文件：Iterator.h、TypeTraits.h（代码内有注释）<br>
实现了迭代器萃取类型、迭代器的定义: [Iterator.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Iterator.h); 实现了类型的萃取: [TypeTraits.h](https://github.com/fgy1995/FgyTinySTL/blob/master/TypeTraits.h)<br>
### 2.2 测试结果
* 测试了原生指针的使用，对于容器迭代器的测试，等到容器实现后再测试。

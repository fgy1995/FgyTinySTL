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

## 经验总结
* 把看书和写代码遇到的一些问题总结在[myexperience](https://github.com/fgy1995/FgyTinySTL/tree/master/myexperience)<br>

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
* 测试了原生指针的使用，对于容器迭代器的测试，等到容器实现后再测试。<br>
### 3.1 vector
* 包含文件Vector.h、Vector_impl.h、UninitializedFunctions.h、Algorithm.h（代码内有注释）<br>
vector的声明 [Vector.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Vector.h);<br>
vector的实现 [implement_detail/Vector_impl.h](https://github.com/fgy1995/FgyTinySTL/blob/master/implement_detail/Vector_impl.h);<br>
[Alorithm.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Algorithm.h)，因为vector需要使用一些算法，而目前还没有实现自己的算法，因此先使用SGI STL算法库algorithm.h; <br>
[UninitializedFunctions.h](https://github.com/fgy1995/FgyTinySTL/blob/master/UninitializedFunctions.h)描述了3个全局函数，用来初始化为配置的空间。<br>
### 3.2 vector测试结果
* 使用自定义的vector存储内置数据类型和类类型，并使用各种内置操作均能实现正确结果。<br>
* 这次没运用自己设计的迭代器，vector的内置迭代器是原生指针。因此对于iterator.h的测试在下一节。<br>
### 4.1 list
* 包含文件List.h、List_impl.h、Functional.h<br>
list的声明 [List.h](https://github.com/fgy1995/FgyTinySTL/blob/master/List.h);<br>
list的实现 [implement_detail/List_impl.h](https://github.com/fgy1995/FgyTinySTL/blob/master/implement_detail/List_impl.h);<br>
一些功能函数，例如排序使用的函数对象 [Functional.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Functional.h);<br>
### 4.2 list的测试结果
* 分别使用内置类型和自定义类类型对list进行测试，各种操作均能返回正确结果。<br>
* 迭代器的设计，list的迭代器里面用一个指针用来执行链表的节点，因此list只需要使用两个头尾迭代器就可以遍历整个链表。SGI STL源码使用双向循环链表进行存储，只需要一个迭代器就可以遍历整个链表。我的设计使用非循环的双向链表，所以需要两个迭代器，要注意区别；但是它们的本质都是一样的。<br>

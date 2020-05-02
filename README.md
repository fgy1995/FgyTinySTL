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
### 3.2 vector的测试
* 使用自定义的vector存储内置数据类型和类类型，并使用各种内置操作均能实现正确结果。<br>
* 这次没运用自己设计的迭代器，vector的内置迭代器是原生指针。因此对于iterator.h的测试在下一节。<br>
### 4.1 list
* 包含文件List.h、List_impl.h、Functional.h<br>
list的声明 [List.h](https://github.com/fgy1995/FgyTinySTL/blob/master/List.h);<br>
list的实现 [implement_detail/List_impl.h](https://github.com/fgy1995/FgyTinySTL/blob/master/implement_detail/List_impl.h);<br>
一些功能函数，例如排序使用的函数对象 [Functional.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Functional.h);<br>
### 4.2 list的测试
* 分别使用内置类型和自定义类类型对list进行测试，各种操作均能返回正确结果。<br>
* 迭代器的设计，list的迭代器里面用一个指针用来执行链表的节点，因此list只需要使用两个头尾迭代器就可以遍历整个链表。SGI STL源码使用双向循环链表进行存储，只需要一个迭代器就可以遍历整个链表。我的设计使用非循环的双向链表，所以需要两个迭代器，要注意区别；但是它们的本质都是一样的。<br>
### 4.3 list的函数调用关系
![](https://github.com/fgy1995/FgyTinySTL/blob/master/picture/List%E5%87%BD%E6%95%B0%E8%B0%83%E7%94%A8%E5%85%B3%E7%B3%BB.jpg)
<br>
### 5.1 deque
* 包含文件Deque.h、Deque_impl.h<br>
deque的声明[List.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Deque.h);<br>
deuqe的实现[List_impl.h](https://github.com/fgy1995/FgyTinySTL/blob/master/implement_detail/Deque_impl.h);<br>
deque实现了双向队列，它可以从双向分别添加删除元素；deque维护了一个分段的连续空间，所谓分段连续是指通过维护一个map（不是STL的map容器）作为控制器，将不同的分段区间维护到一起。仔细考虑会发现，deque其实是维护了一个一维的指针数组，数组中每一个元素都是一个指针，指向一段连续的内存空间，所有指针指向的内存空间大小都是相同的。在进一步思考，在逻辑上deque其实就是维护了一个二维数组，只不过内部的数据不是连续的。<br>
deque的迭代器是随机访问迭代器，由于deque是分段连续，所以迭代器的随机访问操作要仔细设计。<br>
### 5.2 deque的测试
* 对deque及其迭代器进行各种操作的测试，均能得到正确结果。<br>
### 6.1 stack、queue ---容器适配器（adapter）
* 包含文件Stack.h、Queue.h<br>
stack的声明及实现[Stack.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Stack.h);<br>
queue的声明及实现[Queue.h](https://github.com/fgy1995/FgyTinySTL/blob/master/Queue.h);<br>
* stack和queue都是适配器他们的内部封装了其他的容器，调用容器的操作从而实现了它们的操作；stack和queque内部都默认封装了deque容器，如果想使用其他合适的容器可以在声明时候指定，eg. stack<int,list<int>> st, queue<int,list<int>>等。<br>
### 6.2 测试结果
* 对stack和queue的各种操作均能显示正确结果，此外要注意，stack和queue并没有迭代器，因为它们不需要遍历所有元素，只需要访问头尾元素。<br>
### 7.1 slist
* 包含文件SList.h<br>
slist的声明及实现[SList.h](https://github.com/fgy1995/FgyTinySTL/blob/master/SList.h);<br>
slist是一个单链表，它并不在STL标准中，但是它一样有着广泛的使用，虽然它的功能可以由list代替，但是它占用的空间比list小，所以在一些场景中，slist要比list好一些。<br>
另外slist中节点类和迭代器类的实现均采用了继承结构，通过研究这种结构可以为之后rbtree结构打下基础。其实在SGI STL源码中各种类的设计都是采用了继承结构，只是《STL源码剖析》中为了讲解说明，没有这么说。
### 7.2 slist测试结果
* 对slist进行各种操作均能正确实现。<br>
### 8.1 priority_queue
* 包含文件Priority_queue.h, HeapAlgorithm.h,Utility.h<br>
优先级队列内部使用vector作为容器，使用堆的算法对vector进行处理，使其可以成为一个堆。并能够执行优先级队列的算法。

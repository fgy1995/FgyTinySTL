//
// Created by Fgy on 2020/4/10.
//

#ifndef FGYSTL_VECTOR_H
#define FGYSTL_VECTOR_H

#include <algorithm>
#include <assert.h>

#include "Allocator.h"
//#include "Algorithm.h"
#include "Iterator.h"
//#include "ReverseIterator.h" 反向迭代器还没有学习到，学习到后在实现，下面的有关反向迭代器的操作也暂时不加上去
#include "UninitializedFunctions.h"
#include "TypeTraits.h"//看了别人的代码不知道为什么用了SGI STL标准的type_traits，我先用自己写的看看有什么差别
#include <type_traits>//运用了is_Integral<>::type 这个类型，我不知道有什么用，对重载似乎也没有帮助

namespace FgyTinySTL{
    /*
     * 对vector进行定义
     * */
    template<class T, class Alloc=allocator<T>>//使用自己写的allocator作为空间配置器
    class vector{
    public://先定义嵌套类型，这部分的内容要多于STL源码剖析上的内容，因为STL一直在更新前进
        typedef T                       value_type;
        typedef value_type *            iterator;//vector的迭代器就是一个原生指针，因为vector其本质就是一段在内存上连续分配的空间
        typedef const value_type*       const_iterator;
        //typedef reverse_iterator_t<T*>  reverse_iterator; 定义了反向迭代器，目前没有实现
        //typedef reverse_iterator_t<const T*>  const_reverse_iterator; const版本的反向迭代器
        typedef value_type *            pointer;
        typedef value_type &            reference;
        typedef const value_type &      const_reference;
        typedef size_t                  size_type;
        typedef ptrdiff_t               difference_type;
    private://定义vector最基本的数据成员，这里用的是private权限，SGI STL标准用的是protected权限，因为它需要被别人继承，本项目不考虑对于vector的继承问题
        iterator start;//目前空间的头
        iterator finish;//目前空间的尾
        iterator end_of_storage;//目前可以空间的尾
        typedef Alloc dataAllocator;//SGI STL的源码这里用的simple_alloc接口，即 typedef simple_alloc<value_type,Alloc> dataallocator;
                                    //本项目提供的接口和simple_alloc类似
    public:
        //声明构造、拷贝、赋值、析构函数
        vector():start(0),finish(0),end_of_storage(0){}
        explicit vector(const size_type n);//显示构造函数，防止发生隐式的类型转换，explicit用于只有一个参数的构造函数，可以再经验上记录一下
        vector(const size_type n, const value_type& value);//构造n个value
        template<class InputIterator>
        vector(InputIterator first, InputIterator last);//用first到last范围的元素初始化vector
        vector(const vector& v);//拷贝构造函数，注意要用const vector&
        vector(vector&& v);//拷贝构造函数的移动版本,右值引用
        vector& operator=(const vector& v);//重载拷贝赋值运算符
        vector& operator=(vector&& v);//重载拷贝运算符，移动版本,右值引用
        ~vector();//注意这里没用虚函数，因为本项目没有考虑继承结构

        //声明比较运算符
        bool operator==(const vector& v) const;//const函数，const成员函数不可访问非const成员函数，
                                               // const对象只能访问const成员函数，非const对象既可以访问const成员函数又可以访问非const成员函数
                                               //这是因为函数会传递const this指针
        bool operator!=(const vector& v) const;

        //与迭代器相关的操作
        iterator begin(){return start;}//返回首迭代器
        const_iterator begin() const{return start;}//返回const迭代器 const_iterator就是const T*, 因为const所以可以重载
        const_iterator cbegin() const{return start;}//返回const迭代器
        iterator end(){return finish;}//返回尾迭代器
        const_iterator end() const{return finish;}//返回const迭代器
        const_iterator cend() const{return finish;}//返回const迭代器
        //reverse_iterator rbegin(){return reverse_iterator(finish);} 反向迭代器
        //const_reverse_iterator rebegin() const{return const_reverse_iterator(finish);}
        //reverse_iterator rend(){return reverse_iterator(start);}
        //const_reverse_iterator rend() const{return const_reverse_iterator(finish);}

        //与容器大小相关操作
        difference_type size(){return finish-start;}//返回容器大小
        difference_type capacity(){return end_of_storage-start;}//返回容器的容量
        bool empty()const{return start==finish;}//判断容器是否为空
        void resize(size_type n, value_type val=value_type());//这个value_type(),并不是调用返回类型指针的函数。这个value_type()是建立一个临时对象，作为默认值
                                                                //重设容器大小
        void reserve(size_type n);//改变容器容量
        void shrink_to_fit();//缩小容器空间

        //访问元素相关
        reference operator[](const difference_type i){return *(begin()+i);}//由于vector的迭代器是原生指针所以无需重载*
        const_reference operator[](const difference_type i) const{return *(begin()+i);}
        reference front(){return *begin();}
        reference back(){return *(end()-1);}
        pointer data(){return start;}//返回容器首元素指针

        //修改容器相关的操作
        void clear();//销毁容器中的所有对象，但是不回收空间
        void swap(vector& v);//交换容器元素
        void push_back(const value_type& value);//尾后插入元素
        void pop_back();//尾后删除元素
        iterator insert(iterator position, const value_type& val);//在position前插入元素
        void insert(iterator position, const size_type& n, const value_type& val);//在position前插入n个元素
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);//在position前插入[first,last)范围的元素,这个范围可以是别的容器元素
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);

        //空间配置器相关
        Alloc get_allocator(){return dataAllocator();}

    private:
        //定义不对外开放的，与构造、插入等操作的
        void destroyAndDeallocateAll();//用于销毁容器
        void allocateAndFillN(const size_type n, const value_type& value);//分配空间并初始化n个value
        template<class InputIterator>
        void allocateAndCopy(InputIterator first, InputIterator last);//分配空间并拷贝该范围的元素

        template<class InputIterator>
        void vector_aux(InputIterator first, InputIterator last, std::false_type);//这块别人的代码用的std::false_type。一会实现的时候看看原因
                                                                                //但是我写的typetraits和源码一样。
        template<class Integer>
        void vector_aux(Integer n, const value_type& value, std::true_type);
        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
        template<class Integer>
        void insert_aux(iterator position, Integer n, const value_type& value, std::true_type);
        template<class InputIterator>
        void reallocateAndCopy(iterator position, InputIterator first, InputIterator last);
        void reallocateAndFillN(iterator position, const size_type& n, const value_type& val);
        size_type getNewCapacity(size_type len) const;

    public:
        //定义与其他vector的比较操作
        template<class T1, class Alloc1>
        friend bool operator==(const vector<T1,Alloc1>& v1, const vector<T1,Alloc1>& v2);
        template<class T1, class Alloc1>
        friend bool operator!=(const vector<T1,Alloc1>& v1, const vector<T1,Alloc1>& v2);
    };
}

#include "implement_detail/Vector_impl.h"
#endif //FGYSTL_VECTOR_H

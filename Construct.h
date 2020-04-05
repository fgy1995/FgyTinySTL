//
// Created by Fgy on 2020/4/4.
//

#ifndef FGYSTL_CONSTRUCT_H
#define FGYSTL_CONSTRUCT_H

#include <new>
#include "TypeTraits.h"

namespace FgyTinySTL{
    template<class T1,class  T2>
    inline void construct(T1* ptr, T2& value){//全局创建construct函数，复杂在为初始化的空间ptr初始化资源。
        new(ptr) T1(value);//new函数调用了T1的构造函数。
    }

    template<class T>
    inline void destroy(T* ptr){//创建全局的destroy函数，这是第一种形式，直接传入指针，那么就可以直接调用析构函数
        ptr->~T();
    }

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _true_type){ }//POD类型调用这个析构内置类型，_true_type在这里充当哑元，用于区分函数

    template<class ForwardIterator>
    inline void _destroy(ForwardIterator first, ForwardIterator last, _false_type){
        for(; first<last; ++first)
            destroy(&*first);//对于迭代器指向的每一个元素都使用destroy函数进行析构
    }

    template<class ForwardIterator>
    inline void destroy(ForwardIterator first, ForwardIterator last){//destroy的第二种形式，输入左闭右开区间迭代器，对对象进行析构
        /*使用了traits技术来判断ForwardIterator的类型，POD类型表示他是标量类型，例如内置类型int等，用户自定义的类型不是POD类型。
         * 1.POD类型和非POD类型的区别在于，他们的析构函数，POD类型不需要定义析构函数，系统进行销毁，而非POD类型则通常需要自己定义析构函数，
         * 由于POD类型和非POD类型在销毁时候的差别，所以如果对于每一个POD类型都调用析构函数的话会浪费时间，因为系统自动销毁。所以利用了traits
         * 技术来判断ForwardIterator的类型，调用不同的函数。
         * 2._type_traits<T> 这是一个结构体，结构体定义了判断类型的变量。其中_true_type,_false_type也都是定义的结构体。
         * 3.typename的作用，把他后面的声明为类型，而不是变量，这样可以有效防止发生错误。
         *
         * ####注意：这块写的和SGI STL标准有一些不同，这里做了一些缩减
         * SGI STL标准在这里首先使用value_type(ForwardIterator)判断迭代器的类型，之后调用_destroy(fisrt,last,value_type(ForwardIterator))
         * 在_destroy()函数中，使用traits技术来判断该迭代器类型是否具有trivial_destructor,所谓的trivial_destructor是指不复杂的析构函数，
         * 一般指内置类型，因为内置类型的析构由系统解决。之后再调用_destroy_aux（）来执行析构，也是根据_true_type和false_type来决定的
         * SGISTL标准写的有些繁琐了，所以可以直接使用is_POD_type来判断是否有trivial_destructor。
         *
         * */
        typedef typename _type_traits<ForwardIterator>::is_POD_type is_POD_type;
        _destroy(first,last,is_POD_type());//将根据is_POD_type()的类型来调用不同的函数，
    }

    //此外，还要写两个特化版本，针对迭代器char*和wchar*，traits技法无法判断这两个类型
    //函数调用的时候一定是调用最匹配的版本，所以如果匹配则会在以下两个函数中进行匹配
    inline void destroy(char* first, char* last) {}
    inline void destroy(wchar_t* first, wchar_t* last) {}


}

#endif //FGYSTL_CONSTRUCT_H

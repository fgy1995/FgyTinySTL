//
// Created by Fgy on 2020/4/10.
//

#ifndef FGYSTL_UNINITIALIZEDFUNCTIONS_H
#define FGYSTL_UNINITIALIZEDFUNCTIONS_H

//#include "Algorithm.h" 暂时没有定义，先用STL标准的Algorithm代替，
#include <algorithm>
#include "Construct.h"
#include "Iterator.h"
#include "TypeTraits.h"
/*
 * 这个文件复杂创建uninitialized函数(都是全局函数)，用于对未初始化的内存进行按块的初始化
 * 运用高级的函数，algorithm中的函数，目前algorithm还没有实现，用的STL标准的Algorithm库
 * */
namespace FgyTinySTL{
    /*
     * uninialized_copy的实现
     * 输入一个范围[first,last)，给定一个未初始化空间的起始位置result,将该范围的元素以此复制从result开始的位置，并返回尾后迭代器
     * */
    /*下面三个为函数声明，定义在下面*/
    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*);

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type);

    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type);

    /*下面为函数定义*/
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result){
        return _uninitialized_copy(first,last,result,value_type(result));//value_type()判断result迭代器所指的类型
    }
    template<class InputIterator, class ForwardIterator, class T>
    inline ForwardIterator _uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*){
        typedef typename _type_traits<T>::is_POD_type is_POD;//判断迭代器所指的类型T是否为POD类型
        return _uninitialized_copy_aux(first,last,result,is_POD());
    };
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type){
        memcpy(result,first,(last-first)*sizeof(*first));
        return result+(last-first);
    }
    template<class InputIterator, class ForwardIterator>
    inline ForwardIterator _uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type){
        ForwardIterator cur=result;
        for(;first!=last;++first,++cur)
            construct(&(*cur),*first);
        return cur;
    }
    /*
     * 定义uninitialized_fill
     * 给定范围[first,last)，数值x，为该范围的所有元素赋值x
     * */
    /*下面为函数声明，函数定义在后面*/
    template<class ForwardIterator, class T, class T1>
    inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*);

    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _true_type);

    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _false_type);

    /*下面为函数定义*/
    template<class ForwardIterator, class T>
    inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x){
        _uninitialized_fill(first,last,x,value_type(first));
    }
    template<class ForwardIterator, class T, class T1>
    inline void _uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*){
        typedef typename _type_traits<T1>::is_POD_type is_POD;
        _uninitialized_fill_aux(first,last,x,is_POD());
    }
    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _true_type){
        std::fill(first,last,x);
    }
    template<class ForwardIterator, class T>
    inline void _uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& x, _false_type){
        for(;first!=last;++first)
            construct(&(*first),x);
    }

    /*
     * 定义uninitialized_fill_n
     * 给定first指定欲初始化空间的起始位置，n表示欲初始化空间的大小，x表示初始化值，返回尾后迭代器
     * */
    /*下面为函数声明，定义在后面*/
    template<class ForwardIterator, class Size, class T, class T1>
    inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*);
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type);
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type);

    /*下面为函数定义*/
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x){
        return _uninitialized_fill_n(first,n,x,value_type(first));
    }
    template<class ForwardIterator, class Size, class T, class T1>
    inline ForwardIterator _uninitialized_fill_n(ForwardIterator first, Size n, const T& x, T1*){
        typedef typename _type_traits<T1>::is_POD_type is_POD;
        return _uninitialized_fill_n_aux(first,n,x,is_POD());
    }
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _true_type){
        return std::fill_n(first,n,x);
    }
    template<class ForwardIterator, class Size, class T>
    inline ForwardIterator _uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, _false_type){
        ForwardIterator cur=first;
        for(;n!=0;++cur,--n)
            construct(&(*cur),x);
        return cur;
    }
}

#endif //FGYSTL_UNINITIALIZEDFUNCTIONS_H

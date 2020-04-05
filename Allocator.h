//
// Created by Fgy on 2020/4/5.
//

#ifndef FGYSTL_ALLOCATOR_H
#define FGYSTL_ALLOCATOR_H


#include "Alloc.h"
#include "Construct.h"

#include <cassert>
#include <new>

namespace FgyTinySTL{
    /*
     * allocator类提供了一个使用alloc类的接口，同时Allocator.h也符合STL的标准。
     * 但是注意：在SGI STL虽然也提供了类似的接口defalloc.h，但是它没使用过，SGI STL使用simple_alloc提供alloc的接口
     * 本设计中因为没有涉及复杂的一级配置，又为了方便使用，所以自己设计一个allocator类作为alloc的接口。
     * */
    template<class T>
    class allocator{
    public:
        //定义一些变量别名，可能在traits技术有用，目前还没学到那里
        typedef T value_type;
        typedef T* pointer;
        typedef const T* const_pointer;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
    public:
        static T* allocate();
        static T* allocate(size_t n);
        static void deallocate(T* ptr);
        static void deallocate(T* ptr, size_t n);

        static void construct(T* ptr);
        static void construct(T* ptr, const T& value);
        static void destroy(T* ptr);
        static void destroy(T* first, T* last);
    };
    template<class T>
    T* allocator<T>::allocate() {
        return static_cast<T*>(alloc::allocate(sizeof(T)));
    }
    template<class T>
    T* allocator<T>::allocate(size_t n) {
        if(n==0) return 0;
        return static_cast<T*>(alloc::allocate(sizeof(T)*n));
    }

    template<class T>
    void allocator<T>::deallocate(T *ptr) {
        alloc::deallocate(static_cast<void*>(ptr),sizeof(T));
    }
    template<class T>
    void allocator<T>::deallocate(T *ptr, size_t n) {
        if(n==0) return;
        alloc::deallocate(static_cast<void*>(ptr), sizeof(T)*n);
    }

    template<class T>
    void allocator<T>::construct(T *ptr) {
        new(ptr) T();//调用默认构造函数
    }
    template<class T>
    void allocator<T>::construct(T *ptr, const T &value) {
        new(ptr) T(value);//调用拷贝构造函数
    }

    template<class T>
    void allocator<T>::destroy(T *ptr) {
        ptr->~T();
    }
    template<class T>
    void allocator<T>::destroy(T *first, T *last) {
        for(;first!=last;++first)
            first->~T();
    }
}

#endif //FGYSTL_ALLOCATOR_H

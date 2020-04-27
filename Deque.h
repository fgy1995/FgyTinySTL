//
// Created by Fgy on 2020/4/24.
//

#ifndef FGYSTL_DEQUE_H
#define FGYSTL_DEQUE_H

#include "Allocator.h"
#include "Iterator.h"
#include <iostream>
//#include "ReverseIterator.h"

namespace FgyTinySTL{
    template<class T, class Alloc=allocator<T>>
    class deque;
    namespace Detail{
        /*
         * deque的迭代器类设计，它是随机访问迭代器，需要提供随机访问迭代器的各种操作
         * */
        template<class T>//SGI STL源码中的模板还有REF,PTR，以及空间大小,为了方便没有实现那么多
        class dq_iterator:public iterator<random_access_iterator_tag,T>{
        private:
            template<class T1, class Alloc>
            friend class FgyTinySTL::deque;//声明deque为友员
        private:
            //注意这里的数据成员设计与SGI STL源码中一些区别，SGI STL源码中使用T* cur,first,last,来标记一个迭代器的位置
            //这里使用mapIndex和cur标记迭代器位置，本质一样就是表现形式有所差别
            typedef FgyTinySTL::deque<T>* cntrPtr;
            size_t mapIndex;//mapIndex用于指明deque中map的偏移量，
            T* cur;//标记该迭代器在缓存空间中的位置
            cntrPtr container;//标记该迭代器属于哪一个容器
        private:
            T* getBuckTail(size_t mapIndex)const;//获得缓存空间的尾
            T* getBuckHead(size_t mapIndex)const;//获得缓存空间的头
            size_t getBuckSize()const;//获得缓存空间的大小
        public:
            dq_iterator():mapIndex(-1),cur(0),container(0){}
            dq_iterator(size_t index, T* ptr, cntrPtr container):mapIndex(index),cur(ptr),container(container){}
            dq_iterator(const dq_iterator& it):mapIndex(it.mapIndex),cur(it.cur),container(it.container){}
            dq_iterator& operator=(const dq_iterator& it);
            void swap(dq_iterator& it);
            T& operator*(){return *cur;}
            const T& operator*() const{return *cur;}
            T* operator->(){return &(operator*());}
            const T* operator->() const{return &(operator*());}//调用const版本
            dq_iterator& operator++();
            dq_iterator operator++(int);//后置版本
            dq_iterator& operator--();
            dq_iterator operator--(int);//后置版本
            bool operator==(const dq_iterator& it)const;
            bool operator!=(const dq_iterator& it)const;

        public:
            /*
             * 定义迭代器的随机访问操作
             * */
            template<class T1>
            friend dq_iterator<T1> operator+(const dq_iterator<T1>& it, typename dq_iterator<T1>::difference_type n);
            template<class T1>
            friend dq_iterator<T1> operator+(typename dq_iterator<T1>::difference_type n, const dq_iterator<T1>& it);
            template<class T1>
            friend dq_iterator<T1> operator-(const dq_iterator<T1>& it, typename dq_iterator<T1>::difference_type n);
            template<class T1>
            friend dq_iterator<T1> operator-(typename dq_iterator<T1>::difference_type n, const dq_iterator<T1>& it);
            template<class T1>
            friend typename dq_iterator<T1>::difference_type operator-(const dq_iterator<T1>& it1, const dq_iterator<T1>& it2);
            template<class T1>
            friend void swap(dq_iterator<T1>& lhs, dq_iterator<T1>& rhs);
        };
    }//end of detail

    /*
     * 声明deque
     * */
    template<class T, class Alloc>
    class deque{
    private:
        template<class T1>//把迭代器声明为友员
        friend class FgyTinySTL::Detail::dq_iterator;
    public:
        /*
         * 定义类型别名
         * */
        typedef T value_type;
        typedef Detail::dq_iterator<T> iterator;
        typedef Detail::dq_iterator<const T> const_iterator;
        typedef T& reference;
        typedef const T& const_reference;
        typedef size_t size_type;
        typedef ptrdiff_t difference_type;
        typedef Alloc allocator_type;
    private:
        typedef Alloc dataAllocator;//分配资源
        enum class EBucksSize{BUCKSIZE=64};//使用枚举可以为类型数据成员赋初值，这个枚举表示每个缓存空间最多存64个元素；SGI STL源码中每个缓存区的默认值为512bytes
    private:
        //定义deque的数据成员，用于遍历整个容器
        iterator start,finish;//头尾迭代器
        size_t mapSize;//map的容量
        T** map;//map其实就是一个存储指针的意为数组的指针，数组指针；进一步分析map，其实感觉整个deque就像在维护一个动态变化的二维数组，而动态变化的维护依赖于map；
    public:
        /*
         * 构造函数相关
         * */
        deque():mapSize(0),map(0){};
        explicit deque(size_type n, const value_type& val=value_type());
        template<class InputIterator>
        deque(InputIterator first, InputIterator last);
        deque(const deque& x);//拷贝构造
        ~deque();
        deque& operator=(const deque& x);

    public:
        /*
         * 迭代器相关操作
         * */
        iterator begin(){ return start;}
        iterator end(){ return finish;}
        iterator begin()const{ return start;}
        iterator end()const{ return finish;}

        /*
         * 容器容量相关操作
         * */
        size_type size()const{return end()-begin();}
        bool empty() const{return begin()==end();}

        /*
         * 取容器的值相关操作
         * */
        reference operator[](size_type n) {return *(begin()+n);}
        reference front() {return *begin();}
        reference back() {return *(end()-1);}
        const_reference operator[](size_type n)const {return *(begin()+n);};
        const_reference front()const { return *begin();}
        const_reference back()const {return *(end()-1);}

        /*
         * 首尾插入删除操作（主要功能）
         * */
        void push_back(const value_type& val);
        void push_front(const value_type& val);
        void pop_back();
        void pop_front();
        void swap(deque& x);
        void clear();

    private:
        /*
         * 定义一些辅助函数
         * */
        T* getANewBuck();
        T** getANewMap(const size_t size);
        size_t getNewMapSize(const size_t size);
        size_t getBuckSize()const;
        void init();
        bool back_full()const;
        bool front_full()const;
        void deque_aux(size_t n, const value_type& val, std::true_type);
        template<class InputIterator>
        void deque_aux(InputIterator first, InputIterator last, std::false_type);
        void reallocateAndCopy();

    public:
        /*
         * 定义比较操作
         * */
        template<class T1>
        friend bool operator==(const deque<T1>& lhs, const deque<T1>& rhs);
        template<class T1>
        friend bool operator!=(const deque<T1>& lhs, const deque<T1>& rhs);
        template<class T1>
        friend void swap(deque<T1>& lhs, deque<T1>& rhs);
    };//end of deque
}// end of FgyTinySTL

#include "implement_detail/Deque_impl.h"
#endif //FGYSTL_DEQUE_H















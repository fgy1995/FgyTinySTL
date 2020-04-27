//
// Created by Fgy on 2020/4/27.
//

#ifndef FGYSTL_STACK_H
#define FGYSTL_STACK_H

#include "Deque.h"

/*
 * stack是adpter（适配器），它的内部封装了别的容器，所以实现起来比较简单
 * stack内部默认的容器是deque<T>,当然可以使用别的容器，list，vector均可以，可以在声明的时候指明
 * 以下提供stack的基本操作，
 * */
namespace FgyTinySTL{
    template<class T, class Sequence=FgyTinySTL::deque<T>>
    class stack{
    private:
        /*
         * 定义友员
         * */
        template<class T1, class Sequence1>
        friend bool operator==(const stack<T1,Sequence1>& lhs, const stack<T1,Sequence1>& rhs);
        template<class T1, class Sequence1>
        friend bool operator==(const stack<T1,Sequence1>& lhs, const stack<T1,Sequence1>& rhs);

    public:
        /*
         * 定义相关类型别名
         * */
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        typedef Sequence container_type;

    private:
        /*
         * 定义底层容器
         * */
        container_type c;
    public:
        explicit stack(){c=container_type();}
        bool empty() const {return c.empty();}
        size_type size() const {return c.size();}
        reference top() {return c.back();}
        const_reference top() const{return c.back();}
        void push(const value_type& val) {c.push_back(val);}
        void pop() {c.pop_front();}
    };
    template<class T, class Sequence>
    bool operator==(const stack<T,Sequence>& lhs, const stack<T,Sequence>& rhs){
        return lhs.c==rhs.c;
    }
    template<class T, class Sequence>
    bool operator!=(const stack<T,Sequence>& lhs, const stack<T,Sequence>& rhs){
        return lhs.c!=rhs.c;
    }

}//end of FgyTinySTL

#endif //FGYSTL_STACK_H




















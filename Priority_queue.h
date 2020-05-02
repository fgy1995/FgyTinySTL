//
// Created by Fgy on 2020/4/28.
//

#ifndef FGYSTL_PRIORITY_QUEUE_H
#define FGYSTL_PRIORITY_QUEUE_H

#include "HeapAlgorithm.h"
#include "Functional.h"
#include "Vector.h"

/*
 * 定义优先级队列，优先级队列内部封装了堆，使用vector作为默认容器
 * */

namespace FgyTinySTL{
    template<class T, class Sequence=FgyTinySTL::vector<T>, class Compare=FgyTinySTL::less<typename Sequence::value_type>>
    class priority_queue{
    public:
        typedef T                   value_type;
        typedef Sequence            container_type;
        typedef typename Sequence::reference            reference;
        typedef typename Sequence::const_reference      const_reference;
        typedef typename Sequence::size_type            size_type;
    private:
        container_type c;
        Compare compare;
    public:
        explicit priority_queue(const Compare& comp=Compare(), const container_type & ctnr=container_type()):c(ctnr),compare(comp){}
        template<class InputIterator>
        priority_queue(InputIterator first, InputIterator last,
                       const Compare& comp=Compare(), const const container_type& ctnr=container_type())
                       :c(ctnr),compare(comp){
            c.insert(c.end(),first,last);
            FgyTinySTL::make_heap(c.begin(),c.end());
        }
        bool empty() const {return c.empty();}
        size_type size() const {return c.size();}
        reference top() {return c.front();}
        void push(const value_type& val){
            c.push_back(val);
            FgyTinySTL::push_heap(c.begin(),c.end(),compare);
        }
        void pop(){
            FgyTinySTL::pop_heap(c.begin(),c.end(),compare);
            c.pop_back();
        }
        void swap(priority_queue& x){
            FgyTinySTL::swap(c,x.c);
            FgyTinySTL::swap(compare,x.compare);
        }

    public:
        /*
         * 定义友员函数
         * */
        template<class T1, class Sequence1, class Compare1>
        friend void swap(priority_queue<T1,Sequence1,Compare1>& lhs, priority_queue<T1,Sequence1,Compare1>& rhs);
    };
    template<class T1, class Sequence1, class Compare1>
    void swap(priority_queue<T1,Sequence1,Compare1>& lhs, priority_queue<T1,Sequence1,Compare1>& rhs){
        lhs.swap(rhs);
    }
}//end of FgyTinySTL

#endif //FGYSTL_PRIORITY_QUEUE_H

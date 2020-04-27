//
// Created by Fgy on 2020/4/27.
//

#ifndef FGYSTL_QUEUE_H
#define FGYSTL_QUEUE_H

#include "List.h"
#include "Deque.h"

namespace FgyTinySTL{
    template<class T, class Sequence=FgyTinySTL::deque<T>>
    class queue{
    private:
        /*
         * 定义友员
         * */
        template<class T1, class Sequence1>
        friend bool operator==(const queue<T1,Sequence>& lhs, const queue<T1,Sequence1>& rhs);
        template<class T1, class Sequence1>
        friend bool operator<(const queue<T1,Sequence>& lhs, const queue<T1,Sequence1>& rhs);

    public:
        /*
         * 定义类型别名
         * */
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
        typedef typename Sequence::const_reference const_reference;
        typedef Sequence container_type;
    private:
        container_type c;
    public:
        queue(){}
        bool empty() const {return c.empty();}
        size_type size() const {return c.size();}
        reference front() { return c.front();}
        const_reference front() const {return c.front();}
        reference back() {return c.back();}
        const_reference back() const {return c.back();}
        void push(const value_type& val){c.push_back(val);}
        void pop(){c.pop_front();}
    };
    template<class T, class Sequence>
    bool operator==(const queue<T,Sequence>& lhs, const queue<T,Sequence>& rhs){
        return lhs.c==rhs.c;
    }
    template<class T, class Sequence>
    bool operator<(const queue<T,Sequence>& lhs, const queue<T,Sequence>& rhs){
        return lhs.c<rhs.c;
    }
}//end of FgyTinySTL

#endif //FGYSTL_QUEUE_H

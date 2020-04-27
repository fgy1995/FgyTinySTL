//
// Created by Fgy on 2020/4/27.
//

#ifndef FGYSTL_SLIST_H
#define FGYSTL_SLIST_H

#include "Allocator.h"
#include "Iterator.h"
#include <iostream>
/*
 * slist是单链表，list是双向链表，他们各有各自的好处
 * 由于slist的一部分操作和list挺像的，这里就不一一实现了，只实现一些基本功能。
 * 此外，slist的迭代器和节点类型使用了继承的体系结构，可以通过这个来复习一下继承的基础知识，并为后面的rbtree打基础
 * */
namespace FgyTinySTL{
    namespace Detail{
        struct slist_node_base{
            slist_node_base* next;
        };
        template<class T>
        struct slist_node:public slist_node_base{
            T data;
        };
        /*
         * 定义两个全局函数作用于slist的节点
         * */
        //在某一节点后，插入节点
        //注意，slist_node也可以使用这个函数，因为父节点指针可以指向子节点，并且函数没有使用子节点的内容
        //就算使用了子节点的内容也是可以的，因为有动态绑定的机制。
        //但是要注意返回的节点是slist_node_base,使用的时候要注意把类型强转回来
        inline slist_node_base* slist_make_link(slist_node_base* prev, slist_node_base* new_node){
            new_node->next=prev->next;
            prev->next=new_node;
            return new_node;
        }
        //统计某一个节点后的链表长度
        inline size_t slist_size(slist_node_base* node){
            size_t result=0;
            for(; node!=0; node=node->next)
                ++result;
            return result;
        }
        /*
         * 定义slist的迭代器，同样也采用了继承结构
         * */
        struct slist_iterator_base{
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;
            typedef FgyTinySTL::forward_iterator_tag iterator_category;

            slist_node_base* node;//迭代器指向的节点

            slist_iterator_base(slist_node_base* x):node(x){}
            void incr(){node=node->next;}
            bool operator==(const slist_iterator_base& x)const{ return node==x.node;}
            bool operator!=(const slist_iterator_base& x)const{ return node!=x.node;}
        };
        template<class T>
        struct slist_iterator:public slist_iterator_base{
            typedef slist_iterator<T> iterator;
            typedef slist_iterator<T> self;
            typedef T value_type;
            typedef T* pointer;
            typedef T& reference;
            typedef slist_node<T> list_node;

            slist_iterator(list_node* x):slist_iterator_base(x){}
            slist_iterator():slist_iterator_base(0){}
            slist_iterator(const iterator& x):slist_iterator_base(x.node){}

            reference operator*()const { return ((list_node*)node)->data;}
            pointer operator->()const { return &(operator*());}
            self& operator++(){
                incr();
                return *this;
            }
            self operator++(int){
                self tmp=*this;
                incr();
                return tmp;
            }
            //没有--操作，因为slist的迭代器是单向迭代器。
        };
    }//end of Detail

    template<class T, class Alloc=allocator<Detail::slist_node<T>>>
    class slist{
    public:
        typedef T           value_type;
        typedef T*          pointer;
        typedef const T*    const_pointer;
        typedef T&          reference;
        typedef const T&    const_reference;
        typedef size_t      size_type;
        typedef ptrdiff_t   difference_type;

        typedef Detail::slist_iterator<T>           iterator;
        typedef Detail::slist_iterator<const T>     const_iterator;

    private:
        typedef Detail::slist_node<T>       list_node;
        typedef Detail::slist_node_base     list_node_base;
        typedef Detail::slist_iterator_base iterator_base;
        typedef Alloc                       nodeAllocator;

        static list_node* create_node(const value_type& val){
            list_node* node=nodeAllocator::allocate();
            construct(&(node->data),val);
            node->next=0;
            return node;
        }
        static void destroy_node(list_node* node){
            destroy(&node->data);
            nodeAllocator::deallocate(node);
        }

    private:
        list_node_base head;
    public:
        slist(){head.next=0;}
        ~slist(){clear();}

    public:
        iterator begin(){return iterator((list_node*)head.next);}
        iterator end(){return iterator(0);}
        size_type size()const {return Detail::slist_size(head.next);}
        bool empty()const {return head.next==0;}
        void clear(){
            list_node* node=(list_node*)head.next;
            while(node){
                list_node* tmp=node;
                node=(list_node*)node->next;
                head.next=node;
                destroy_node(tmp);
            }
            nodeAllocator::deallocate((list_node*)&head);
        }
        void swap(slist& l){
            list_node_base* tmp=l.head.next;
            l.head.next=head.next;
            head.next=tmp;
        }
        reference front() { return ((list_node*)head.next)->data; }
        void push_front(const value_type& val){
            list_node* tmp=create_node(val);
            Detail::slist_make_link(&head,tmp);
        }
        void pop_front(){
            list_node* tmp=head.next;
            head.next=tmp->next;
            destroy_node(tmp);
        }
    };


}//end of FgyTinySTL

#endif //FGYSTL_SLIST_H


















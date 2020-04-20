//
// Created by Fgy on 2020/4/16.
//

#ifndef FGYSTL_LIST_H
#define FGYSTL_LIST_H

#include "Allocator.h"
#include "Iterator.h"
//#include "ReverseIterator.h" 反向迭代器暂时不实现，之后补充
#include "UninitializedFunctions.h"
#include "Functional.h"
#include <type_traits>

namespace FgyTinySTL{
    namespace Detail{//提前声明，因为Alloc要使用node作为默认模板参数
        template<class T>
        struct node;
    }
    template<class T, class Alloc=allocator<Detail::node<T>>>
    class list;//因为嵌套名称空间需要使用list，所以提前声明
    namespace Detail{
        template<class T>
        struct node{//定义list的节点node
            T data;
            node<T>* prev;
            node<T>* next;
            list<T>* container;//该节点属于哪一个容器
            node(const T& d, node* p, node* n, list<T>* c):data(d),prev(p),next(n),container(c){}
            bool operator==(const node& n){
                return data==n.data&&prev==n.prev&&next==n.next&&container==n.container;
            }
        };

        //因为list的迭代器不是原生指针，所以另写一个迭代器类，迭代器类继承父名称空间中的iterator，
        //listIterator继承了iterator中需要萃取的属性
        template<class T>
        struct listIterator: public FgyTinySTL::iterator<bidirectional_iterator_tag,T>{
            template<class T1, class Alloc>
            friend class list;//把list声明为友元类，list可以访问迭代器类的各个属性

            /*声明一个指针，指向list中的节点，用于访问list*/
            typedef node<T>* nodePtr;
            nodePtr p;
            /*声明迭代器类的各种方法*/
            explicit listIterator(nodePtr ptr= nullptr):p(ptr){}
            listIterator& operator++();//前置++,为什么返回引用，因为迭代器发生了变化，并且迭代器指向的是下一节点
            listIterator operator++(int);//后置++，为什么不返回引用，因为迭代器已经指向了下一节点，然而返回的却是当前节点
            listIterator& operator--();//前置--
            listIterator operator--(int);//后置--
            T& operator*() const{return (*p).data;}//解引用返回的是节点存储的元素内容
            T* operator->() const{return &(operator*());}//->返回的是所存元素的指针（地址）
            template<class T1>
            friend bool operator==(const listIterator<T1>& lhs, const listIterator<T1>& rhs);
            template<class T1>
            friend bool operator!=(const listIterator<T1>& lhs, const listIterator<T1>& rhs);
        };
    }//detail namespace end

    /*
     * list的定义及成员的声明
     * */
    template<class T, class Alloc>//声明中已经指出默认模板实参了，定义就不用指出
    class list{
        template<class T1>
        friend struct Detail::listIterator;//把listIterator声明为友员，方便访问listIterator访问
    private://类型别名
        typedef Alloc nodeAllocator;//SGI STL使用的是simple_alloc<list_node,Alloc>接口，这个的好处是可以直接声明要创建的节点以及使用哪种配置器
                                    //我写的这个方法没有使用simple_alloc接口，因此需要在模板中给出配置器及其分配的类型，因此上面的名称空间写的复杂
        typedef Detail::node<T>* nodePtr;
    public://类型别名，用于萃取机制
        typedef T                               value_type;
        typedef Detail::listIterator<T>         iterator;
        typedef Detail::listIterator<const T>   const_iterator;
        //typedef reverse_iterator_t<iterator>  reverse_iterator;    反向迭代器以后实现
        typedef T&                              reference;
        typedef size_t                          size_type;
    private:
        iterator head;//SGI STL使用循环双向链表来表示list，因此只需要一个迭代器就能遍历所有元素
        iterator tail;//本设计使用非循环的双向链表来表示list，可以head，tail表示首尾迭代器，这么写比较方便，但是注意这两种表示法本质没有区别
    public:
        /*
         * 构造函数相关操作
         * */
        list();//默认构造函数
        explicit list(size_type n, const value_type& val=value_type());//显示构造函数，防止隐示转换
        template<class InputIterator>
        list(InputIterator first, InputIterator last);//
        list(const list& l);//拷贝构造函数
        list& operator=(const list& l);//重载赋值运算符
        ~list();

        /*
         * 容器大小相关操作
         * */
        bool empty() const {return tail==head;}
        size_type size() const;
        reference front(){return (head.p)->data;}
        reference back(){return ((tail.p)->prev)->data;}

        /*
         * 头尾插入删除操作,
         * 两种实现方法：1.自己写一遍插入删除操作，便于构造函数使用；2.调用insert和erase操作
         * */
        void push_front(const value_type& val);
        void pop_front();
        void push_back(const value_type& val);
        void pop_back();

        /*
         * 返回迭代器相关操作
         * */
        iterator begin();
        iterator end();
        const_iterator begin() const;//利用const进行重载
        const_iterator end() const;
        //reverse_iterator rbegin(); 反向迭代器之后实现
        //reverse_iterator rend();

        /*
         * 修改容器内容相关操作
         * */
        iterator insert(iterator position, const value_type& val);//指定位置前插入一个元素，返回该元素的迭代器
        void insert(iterator position, size_type n, const value_type& val);//指定位置前插入n个元素
        template<class InputIterator>
        void insert(iterator position, InputIterator first, InputIterator last);
        iterator erase(iterator position);
        iterator erase(iterator first, iterator last);
        void swap(list& x);
        void clear();
        void splice(iterator position, list& x);//将x接入position之前，x不能与*this相同
        void splice(iterator position, list& x, iterator i);//将i所指元素接入到position之前，i和position可在同一list
        void splice(iterator position, list& x, iterator first, iterator last);//将[first,last)接入到position之前，可在同一list，但是position不在其中
        void remove(const value_type& val);//删除指定元素
        void unique();//移除数值相同的连续元素
        template<class Predicate>
        void remove_if(Predicate pred);//删除满足谓词条件的元素
        template<class BinaryPredicate>
        void unique(BinaryPredicate binary_pred);//自定义谓词版本
        void merge(list& x);//合并list，两个list必须有序
        template<class Compare>
        void merge(list& x, Compare comp);//自定义顺序合并list
        void sort();//algorithm中的排序针对随机访问迭代器，因此list的排序算法只能自己实现，使用快速排序完成排序操作,默认小到大
        template<class Compare>
        void sort(Compare comp);//自定义排序顺序
        void reverse();//反转list
    private:
        /*
         * 声明一些辅助函数，用于成员函数调用
         * */
        void ctor_aux(size_type n, const value_type& val, std::true_type);//构造函数的辅助函数
        void ctor_aux(size_type n, const value_type& val, std::false_type);
        template<class InputIterator>
        void ctor_aux(InputIterator first, InputIterator last, std::false_type);//构造函数的辅助函数
        nodePtr newNode(const T& val=T());//创建一个list的节点，值为val nodePtr在listIterator中定义
        void deleteNode(nodePtr p);//删除一个节点
        void insert_aux(iterator position, size_type n, const T& val, std::true_type);//插入操作的辅助函数
        template<class InputIterator>
        void insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type);
        const_iterator changeIteratorToConstIterator(iterator& it) const;

    public:
        /*
         * 声明一些友员函数，
         * */
        template<class T1>
        friend void swap(list<T1>& lhs, list<T1>& rhs);//交换两个list
        template<class T1>
        friend bool operator==(const list<T1>& lhs, const list<T1>& rhs);//重载==，用于判断两个list是否相等
        template<class T1>
        friend bool operator!=(const list<T1>& lhs, const list<T1>& rhs);
    };
}

#include "implement_detail/List_impl.h"
#endif //FGYSTL_LIST_H

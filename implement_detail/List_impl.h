//
// Created by Fgy on 2020/4/16.
//

#ifndef FGYSTL_LIST_IMPL_H
#define FGYSTL_LIST_IMPL_H

#include "../List.h"
#include <iostream>

namespace FgyTinySTL{
    namespace Detail{
        template<class T>
        listIterator<T>& listIterator<T>::operator++() {
            p=p->next;
            return *this;
        }
        template<class T>
        listIterator<T> listIterator<T>::operator++(int) {
            auto res=*this;
            ++(*this);
            return res;
        }
        template<class T>
        listIterator<T>& listIterator<T>::operator--(){
            p=p->prev;
            return *this;
        }
        template<class T>
        listIterator<T> listIterator<T>::operator--(int) {
            auto res=*this;
            --(*this);
            return res;
        }
        template<class T1>
        bool operator==(const listIterator<T1>& lhs, const listIterator<T1>& rhs){
            return lhs.p==rhs.p;
        }
        template<class T1>
        bool operator!=(const listIterator<T1>& lhs, const listIterator<T1>& rhs){
            return !(lhs.p==rhs.p);
        }
    }

    /*
     * 构造函数相关
     * */
    template<class T, class Alloc>
    list<T,Alloc>::list(){
        head.p=newNode();
        tail.p=head.p;
    }

    template<class T, class Alloc>
    list<T,Alloc>::list(size_type n, const value_type& val) {
        ctor_aux(n,val,std::is_integral<value_type>());
    }
    template<class T, class Alloc>
    template<class InputIterator>
    list<T,Alloc>::list(InputIterator first, InputIterator last){
        ctor_aux(first,last,std::is_integral<InputIterator>());
    }
    template<class T, class Alloc>
    list<T,Alloc>::list(const list &l) {
        head.p=newNode();
        tail.p=head.p;
        for(auto node=l.head.p; node!=l.tail.p; node=node->next)
            push_back(node->data);
    }
    template<class T, class Alloc>
    list<T,Alloc>& list<T,Alloc>::operator=(const list &l) {
        if(this!=&l){
            list(l).swap(*this);
        }
        return *this;
    }
    template<class T, class Alloc>
    list<T,Alloc>::~list() {
        if(head.p==tail.p&&tail.p== nullptr)
            return;
        for(;head!=tail;){
            auto tmp=head++;
            nodeAllocator::destroy(tmp.p);
            nodeAllocator::deallocate(tmp.p);
        }
        nodeAllocator::destroy(tail.p);
        nodeAllocator::deallocate(tail.p);
    }

    /*
     * 容器大小相关操作
     * */
    template< class T, class Alloc>
    typename list<T,Alloc>::size_type list<T,Alloc>::size() const {
        size_type counts=0;
        for(auto it=head; it!=tail; ++it)
            ++counts;
        return counts;
    }

    /*
     * 头尾插入删除操作
     * */
    template<class T, class Alloc>
    void list<T,Alloc>::push_front(const value_type &val) {
        auto node=newNode(val);
        head.p->prev=node;
        node->next=head.p;
        head.p=node;
    }
    template<class T, class Alloc>
    void list<T,Alloc>::pop_front() {
        auto oldNode=head.p;
        head.p=oldNode->next;
        head.p->prev= nullptr;//一定指向非空，否则指向野指针
        deleteNode(oldNode);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::push_back(const value_type &val) {
        auto node=newNode();//新建一个结点作为尾后结点
        tail.p->data=val;
        tail.p->next=node;
        node->prev=tail.p;
        tail.p=node;
    }
    template<class T, class Alloc>
    void list<T,Alloc>::pop_back() {
        auto newTail=tail.p->prev;
        newTail->next= nullptr;
        deleteNode(tail.p);
        tail.p=newTail;//这块没有显示的删除尾后结点的前一个元素，而是直接把这个元素当做尾后结点，删除之前的尾后结点，降低开销。
    }

    /*
     * 返回迭代器相关操作
     * */
    template<class T, class Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::begin(){
        return head;
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::end(){
        return tail;
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::begin() const {
        auto tmp=(list* const)this;
        return changeIteratorToConstIterator(tmp->head);
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::end() const {
        auto tmp=(list* const)this;
        return changeIteratorToConstIterator(tmp->tail);
    }

    /*
     * 修改容器内容相关操作
     * */
    template<class T, class Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::insert(iterator position, const value_type &val) {//返回的是插入元素的迭代器
        if(position==begin()){
            push_front(val);
            return begin();
        }
        else if(position==end()){//因为push_back是新建一个新的空结点作为尾后结点，原尾后结点被置为新值。所以返回ret
            auto ret=position;
            push_back(val);
            return ret;
        }
        auto node=newNode(val);
        auto prev=position.p->prev;
        node->next=position.p;
        node->prev=prev;
        prev->next=node;
        (position.p)->prev=node;
        return iterator(node);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::insert(iterator position, size_type n, const value_type& val) {
        insert_aux(position,n,val,typename std::is_integral<value_type >::type());
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void list<T,Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        insert_aux(position,first,last, typename std::is_integral<InputIterator>::type());
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::erase(iterator position) {
        if(position==head){
            pop_front();
            return head;
        }
        else{
            auto prev=position.p->prev;
            prev->next=position.p->next;
            (position.p)->next->prev=prev;
            deleteNode(position.p);
            return iterator(prev->next);
        }
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::iterator list<T,Alloc>::erase(iterator first, iterator last) {
        iterator res;
        for(;first!=last;){
            auto tmp=first++;
            res=erase(tmp);
        }
        return res;
    }
    template<class T, class Alloc>
    void list<T,Alloc>::clear() {
        erase(begin(),end());//clear只删除所有的数据节点，尾后结点不删除，尾后结点可以用于标识容器的存在，
    }
    template<class T, class Alloc>
    void list<T,Alloc>::remove(const value_type &val) {
        for(auto it=begin(); it!=end();){
            if(*it==val)
                it=erase(it);
            else
                ++it;
        }
    }
    template<class T, class Alloc>
    template<class Predicate>
    void list<T,Alloc>::remove_if(Predicate pred) {//删除满足谓词条件的元素
        for(auto it=begin(); it!=end();){
            if(pred(*it))
                it=erase(it);
            else
                ++it;
        }
    }
    template<class T, class Alloc>
    void list<T,Alloc>::unique() {//利用迭代器的写法
        iterator first=begin();
        iterator last=end();
        if(first==last) return;
        iterator next=first;
        while(++next!=last){
            if(*next==*first)
                erase(next);
            else
                first=next;
            next=first;
        }
    }
    template<class T, class Alloc>
    template<class BinaryPredicate>
    void list<T,Alloc>::unique(BinaryPredicate binary_pred) {//利用用户定义的unique谓词对链表进行操作，利用指针自己写，不用erase函数
        nodePtr curNode=head.p;
        while(curNode!=tail.p){
            nodePtr nextNode=curNode->next;
            if(binary_pred(curNode->data,nextNode->data)){
                if(nextNode==tail.p){//nextNode为尾后结点，删除这个元素，cur
                    curNode->next= nullptr;
                    tail.p=curNode;//nextNode的前一个节点作为新的尾后结点
                }
                else{
                    curNode->next=nextNode->next;
                    nextNode->next->prev=curNode;
                }
                deleteNode(nextNode);
            }
            else{
                curNode=nextNode;
            }
        }
    }
    template<class T, class Alloc>
    void list<T,Alloc>::swap(list &x) {//交换两个list中给的内容，其实就是把两个list的头尾指针交换
        auto tmp=x.head.p;
        x.head.p=head.p;
        head.p=tmp;

        tmp=x.tail.p;
        x.tail.p=tail.p;
        tail.p=tmp;
    }
    /*
     * splice是list的一个核心的函数，可以便捷地将不同的list进行拼接操作
     * */
    template<class T, class Alloc>
    void list<T,Alloc>::splice(iterator position, list &x) {
        this->insert(position,x.begin(),x.end());//把x的元素插入到position位置前
        x.head.p=x.tail.p;//x list就只剩一个尾后迭代器，用于标识x list还存在，但是其已经没有元素了
    }
    template<class T, class Alloc>
    void list<T,Alloc>::splice(iterator position, list &x, iterator first, iterator last) {//将x list中的first到last插入到position前
        if(first.p==last.p) return;
        auto tailNode=last.p->prev;//tialNode为待插入元素的最后一个节点
        //下面这个判断语句用于将x中的[first.p,tailNode]提取出来
        if(x.head.p==first.p){//first为x的头结点
            x.head.p=last.p;
            x.head.p->prev= nullptr;
        }
        else{//该范围元素在x链表中间
            (first.p)->prev->next=last.p;
            (last.p)->prev=(first.p)->prev;
        }
        //下面的判断语句用于将该范围的元素插入到position之前
        if(position.p==head.p){
            first.p->prev= nullptr;
            tailNode->next=head.p;
            (head.p)->prev=tailNode;
            head.p=first.p;
        }
        else{
            (position.p)->prev->next=first.p;
            (first.p)->prev=(position.p)->prev;
            tailNode->next=position.p;
            (position.p)->prev=tailNode;
        }
    }
    template<class T, class Alloc>
    void list<T,Alloc>::splice(iterator position, list &x, iterator i) {//将x中的迭代器i指向的元素插入到position之前
        auto next=i;
        this->splice(position,x,i,++next);//直接调用上一个迭代器范围的splice的函数
    }
    /*
     * merge()函数用于将两个排序好的链表合并到this中，
     * 注意：
     * 首先，两个链表有序；
     * 其次，链表中的元素可以比较大小，类类型需要重载各种比较运算符
     * */
    template<class T, class Alloc>
    void list<T,Alloc>::merge(list &x) {
        auto first1=begin();
        auto first2=x.begin();
        while(first1!=end()&&first2!=x.end()){
            if(*first1<=*first2){
                ++first1;
            }
            else{
                auto tmp=first2++;
                this->splice(first1,x,tmp);
            }
        }
        if(first2!=x.end())
            this->splice(first1,x,first2,x.end());
    }
    template<class T, class Alloc>
    template<class Compare>
    void list<T,Alloc>::merge(list &x, Compare comp) {//使用用户定义的比较方法进行合并，comp为可调用对象
        auto first1=begin();
        auto first2=x.begin();
        while(first1!=end()&&first2!=x.end()){
            if(comp(*first2,*first1)){
                auto tmp=first2++;
                this->splice(first1,x,tmp);
            }
            else{
                ++first1;
            }
        }
        if(first2!=x.end()){
            this->splice(first1,x,first2,x.end());
        }
    }
    template<class T, class Alloc>
    void list<T,Alloc>::sort() {//使用快速排序进行排序
        sort(FgyTinySTL::less<T>());
        std::cout<<"sort completed"<<std::endl;
    }
    template<class T, class Alloc>
    template<class Compare>
    void list<T,Alloc>::sort(Compare comp) {
        if(empty()||head.p->next==tail.p)//空链表和只有一个节点的情况直接返回
            return;
        list carry;//中转节点，用于将原链表的每一个头结点取出来
        list counter[64];//定义存储链表的数组，每一个数组节点最多存pow(2,i)个节点，这个排序虽然书上说是用快排，但是我觉得更像归并排序
        int fill=0;//用于指示到哪个节点了
        while(!empty()){
            carry.splice(carry.begin(),*this,begin());//把原链表的头结点存入carry的头部
            int i=0;
            while(i<fill&&!counter[i].empty()){
                counter[i].merge(carry,comp);//把carry链表并入counter[i];此时carry为空
                carry.swap(counter[i++]);//交换carry和counter[i]的内容
            }
            carry.swap(counter[i]);
            if(i==fill)
                ++fill;
        }
        for(int i=1; i!=fill; ++i){
            counter[i].merge(counter[i-1],comp);
        }
        std::cout<<counter[fill-1].size()<<std::endl;
        swap(counter[fill-1]);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::reverse() {
        if(empty()||(head.p)->next==tail.p)
            return;
        auto curNode=head.p;
        head.p=(tail.p)->prev;//直接把最后两个结点变为首尾节点，直接使用头插法就可以了
        (head.p)->prev= nullptr;
        do{//这就是一个头插法的过程
            auto nextNode=curNode->next;
            curNode->next=(head.p)->next;
            (head.p)->next->prev=curNode;
            (head.p)->next=curNode;
            curNode->prev=head.p;
            curNode=nextNode;
        }while(curNode!=head.p);
    }


    /*
     * 辅助函数
     * */
    template<class T, class Alloc>
    typename list<T,Alloc>::nodePtr list<T,Alloc>::newNode(const T& val) {
        nodePtr res=nodeAllocator::allocate();
        nodeAllocator::construct(res,Detail::node<T>(val, nullptr, nullptr, this));
        return res;
    }
    template<class T, class Alloc>
    void list<T,Alloc>::deleteNode(nodePtr p) {
        p->prev=p->next= nullptr;
        nodeAllocator::destroy(p);
        nodeAllocator::deallocate(p);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::ctor_aux(size_type n, const value_type &val, std::true_type) {
        head.p=newNode();
        tail.p=head.p;
        while(n--)
            push_back(val);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::ctor_aux(size_type n, const value_type &val, std::false_type) {
        head.p=newNode();
        tail.p=head.p;
        while(n--)
            push_back(val);
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void list<T,Alloc>::ctor_aux(InputIterator first, InputIterator last, std::false_type) {
        head.p=newNode();
        tail.p=head.p;
        for(;first!=last;++first)
            push_back(*first);
    }
    template<class T, class Alloc>
    typename list<T,Alloc>::const_iterator list<T,Alloc>::changeIteratorToConstIterator(iterator &it) const {//把iterator变成const iterator
        using nodeP=Detail::node<const T>*;
        auto temp=(list<const T>* const)this;
        auto ptr=it.p;
        Detail::node<const T> node(ptr->data,(nodeP)(ptr->prev),(nodeP)(ptr->next),temp);
        return const_iterator(&node);
    }
    template<class T, class Alloc>
    void list<T,Alloc>::insert_aux(iterator position, size_type n, const T &val, std::true_type) {
        for(auto i=n; i!=0; --i){
            position=insert(position,val);
        }
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void list<T,Alloc>::insert_aux(iterator position, InputIterator first, InputIterator last, std::false_type) {
        for(--last; first!=last; --last){
            position=insert(position,*last);
        }
        insert(position,*last);
    }

    /*
     * 一些友员函数
     * */
    template<class T1>
    void swap(list<T1>& lhs, list<T1>& rhs){
        lhs.swap(rhs);
    }
    template<class T1>
    bool operator==(const list<T1>& lhs, const list<T1>& rhs){
        auto node1=lhs.head.p;
        auto node2=rhs.head.p;
        for(; node1!=lhs.tail.p && node2!=rhs.tail.p; node1=node1->next,node2=node2->next){
            if(node1->data!=node2->data)
                break;
        }
        if(node1==lhs.tail.p && node2==rhs.tail.p)
            return true;
        return false;
    }
    template<class T1>
    bool operator!=(const list<T1>& lhs, const list<T1>& rhs){
        return !(lhs==rhs);
    }

}

#endif //FGYSTL_LIST_IMPL_H

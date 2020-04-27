//
// Created by Fgy on 2020/4/25.
//

#ifndef FGYSTL_DEQUE_IMPL_H
#define FGYSTL_DEQUE_IMPL_H

#include "../Deque.h"

namespace FgyTinySTL{
    namespace Detail{
        /*
         * 私有成员函数，获得缓冲区头尾相关操作
         * */
        template<class T>
        T* dq_iterator<T>::getBuckHead(size_t mapIndex) const {//获取迭代器所在缓存区的头指针
            return container->map[mapIndex];
        }
        template<class T>
        T* dq_iterator<T>::getBuckTail(size_t mapIndex) const {//获取迭代器所在缓存区的尾指针，指向容器最后一个元素，而不是最后元素的后一个,和STL源码有所不同，源码指向最后元素的后一个
            return container->map[mapIndex]+(container->getBuckSize()-1);
        }
        template<class T>
        size_t dq_iterator<T>::getBuckSize() const {//返回缓存空间的容量，调用了deque的函数，友员
            return container->getBuckSize();
        }
        /*
         *赋值运算符及交换操作
         * */
        template<class T>
        dq_iterator<T>& dq_iterator<T>::operator=(const dq_iterator &it) {
            if(this!=&it){
                mapIndex=it.mapIndex;
                cur=it.cur;
                container=it.container;
            }
            return *this;
        }
        template<class T>
        void dq_iterator<T>::swap(dq_iterator& it) {
            auto tmp1=mapIndex;
            mapIndex=it.mapIndex;
            it.mapIndex=tmp1;

            auto tmp2=cur;
            cur=it.cur;
            it.cur=tmp2;

            auto tmp3=container;
            container=it.container;
            it.container=tmp3;
        }

        /*
         * ++,--等操作
         * */
        template<class T>
        dq_iterator<T>& dq_iterator<T>::operator++() {
            if(cur!=getBuckTail(mapIndex)){//当前迭代器不在缓存空间的最后一位
                ++cur;
            }
            else if(mapIndex+1<container->mapSize){//当前迭代器在缓存空间最后一位,并且map没越界
                ++mapIndex;
                cur=getBuckHead(mapIndex);
            }
            else{//+1后越界了
                mapIndex=container->mapSize;
                cur=container->map[mapIndex];
            }
            return *this;
        }
        template<class T>
        dq_iterator<T> dq_iterator<T>::operator++(int) {
            auto tmp=*this;
            ++(*this);
            return tmp;
        }
        template<class T>
        dq_iterator<T>& dq_iterator<T>::operator--(){
            if(cur!=getBuckHead(mapIndex)){//当前迭代器不指向缓存空间的头
                --cur;
            }
            else if(mapIndex-1>=0){//当前迭代器指向缓存空间的头，并且map没越界
                --mapIndex;
                cur=getBuckTail(mapIndex);
            }
            else{//越界
                mapIndex=0;
                cur=container->map[mapIndex];
            }
            return *this;
        }
        template<class T>
        dq_iterator<T> dq_iterator<T>::operator--(int){
            auto tmp=*this;
            --(*this);
            return tmp;
        }
        /*
         * 迭代器比较操作
         * */
        template<class T>
        bool dq_iterator<T>::operator==(const dq_iterator& it) const{
            return (mapIndex==it.mapIndex)&&(cur==it.cur)&&(container==it.container);
        }
        template<class T>
        bool dq_iterator<T>::operator!=(const dq_iterator& it) const{
            return !(*this==it);
        }
        /*
         * 随机访问相关操作，
         * */
        template<class T1>
        dq_iterator<T1> operator+(const dq_iterator<T1>& it, typename dq_iterator<T1>::difference_type n){//n>=0
            dq_iterator<T1> res(it);
            auto left=res.getBuckTail(res.mapIndex)-res.cur;
            if(n<=left){//向前前进n后仍在当前缓存空间
                res.cur+=n;
            }
            else{
                n=n-left;//前进剩余空间
                res.mapIndex+=(n/res.getBuckSize()+1);//mapIndex前进
                auto p=res.getBuckHead(res.mapIndex);
                auto x=n%it.getBuckSize()-1;
                res.cur=p+x;
            }
            return res;
        }
        template<class T1>
        dq_iterator<T1> operator+(typename dq_iterator<T1>::difference_type n, const dq_iterator<T1>& it){
            return (it+n);
        }
        template<class T1>
        dq_iterator<T1> operator-(const dq_iterator<T1>& it, typename dq_iterator<T1>::difference_type n){//n>=0
            dq_iterator<T1> res(it);
            auto left=res.cur-res.getBuckHead(res.mapIndex);
            if(n<=left){//向后退仍在当前缓存空间
                res.cur-=n;
            }
            else{
                n=n-left;
                res.mapIndex-=(n/res.getBuckSize()+1);
                res.cur=res.getBuckTail(res.mapIndex)-(n%res.getBuckSize()-1);
            }
            return res;
        }
        template<class T1>
        dq_iterator<T1> operator-(typename dq_iterator<T1>::difference_type n, const dq_iterator<T1>& it){
            return (it-n);
        }
        template<class T1>
        typename dq_iterator<T1>::difference_type operator-(const dq_iterator<T1>& it1, const dq_iterator<T1>& it2){
            if(it1.container==it2.container&&it1.container==0)
                return 0;
            auto diff_size=typename dq_iterator<T1>::difference_type((it1.mapIndex-it2.mapIndex-1)*it1.getBuckSize());
            auto it1_left=typename dq_iterator<T1>::difference_type(it1.cur-it1.getBuckHead(it1.mapIndex));
            auto it2_left=typename dq_iterator<T1>::difference_type(it2.getBuckTail(it2.mapIndex)-it2.cur+1);
            return diff_size+it1_left+it2_left;
        }
        template<class T1>
        void swap(dq_iterator<T1>& lhs, dq_iterator<T1>& rhs){
            lhs.swap(rhs);
        }
    }//end of Detail
    /*
     * 构造函数相关操作
     * */
    template<class T, class Alloc>
    deque<T,Alloc>::deque(size_type n, const value_type& val){
        deque();
        deque_aux(n,val,typename std::is_integral<value_type>::type());
    }
    template<class T, class Alloc>
    template<class InputIterator>
    deque<T,Alloc>::deque(InputIterator first, InputIterator last) {
        deque();
        deque_aux(first,last, typename std::is_integral<InputIterator>::type());
    }
    template<class T, class Alloc>
    deque<T,Alloc>::deque(const deque& x){
        mapSize=x.mapSize;
        map=getANewMap(mapSize);//分配好了map的每一个空间
        for(int i=0; x.start.mapIndex+i!=x.mapSize; ++i)//为容器中的每一个元素赋值
            for(int j=0; j!=getBuckSize(); ++j)
                map[x.start.mapIndex+i][j]=x.map[x.start.mapIndex+i][j];
        start.mapIndex=x.start.mapIndex;
        finish.mapIndex=x.finish.mapIndex;
        start.cur=map[start.mapIndex]+(x.start.cur-x.map[x.start.mapIndex]);
        finish.cur=map[finish.mapIndex]+(x.finish.cur-x.map[x.finish.mapIndex]);
        start.container=finish.container=this;
    }
    template<class T, class Alloc>
    deque<T,Alloc>::~deque() {
        for(int i=0; i!=mapSize; ++i){
            for(auto p=map[i]+0; !p&&p!=map[i]+getBuckSize(); ++p)
                dataAllocator::destroy(p);
            if(!map[i])
                dataAllocator::deallocate(map[i],getBuckSize());
        }
        delete[] map;//map就是一个数组
    }
    template<class T, class Alloc>
    deque<T,Alloc>& deque<T,Alloc>::operator=(const deque& x) {
        if(*this!=x)
            deque(x);
        return *this;
    }

    /*
     * 首尾插入删除操作
     * */
    template<class T, class Alloc>
    void deque<T,Alloc>::push_back(const value_type &val) {
        if(empty()){
            init();
        }
        else if(back_full()){
            reallocateAndCopy();
        }
        FgyTinySTL::construct(finish.cur,val);
        ++finish;
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::push_front(const value_type &val) {
        if(empty()){
            init();
        }
        else if(front_full()){
            reallocateAndCopy();
        }
        --start;
        FgyTinySTL::construct(start.cur,val);
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::pop_front() {
        dataAllocator::destroy(start.cur);
        ++start;
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::pop_back() {
        --finish;
        dataAllocator::destroy(finish.cur);
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::swap(deque& x) {
        auto tmp1=mapSize;
        mapSize=x.mapSize;
        x.mapSize=tmp1;

        auto tmp2=map;
        map=x.map;
        x.map=tmp2;

        start.swap(x.start);
        finish.swap(x.finish);
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::clear() {
        for(auto i=0; i!=mapSize; ++i)
            for(auto p=map[i]+0; !p&&p!=map[i]+getBuckSize(); ++p)
                dataAllocator::destroy(p);
        mapSize=0;
        start.mapIndex=finish.mapIndex=mapSize/2;
        start.cur=finish.cur=map[mapSize/2];
    }

    /*
     * 友员的比较函数
     * */
    template<class T>
    bool operator==(const deque<T>& lhs, const deque<T>& rhs){
        auto it1=lhs.begin(),it2=rhs.begin();
        for(; it1!=lhs.end()&&it2!=rhs.end(); ++it1,++it2)
            if(*it1==*it2)
                return false;
        if(it1==lhs.end()&&it2==rhs.end())
            return true;
        return false;
    }
    template<class T>
    bool operator!=(const deque<T>& lhs, const deque<T>& rhs){
        return !(lhs==rhs);
    }
    template<class T>
    void swap(deque<T>& lhs, deque<T>& rhs){
        lhs.swap(rhs);
    }

    /*
     * 私有的辅助函数
     * */
    template<class T, class Alloc>
    void deque<T,Alloc>::deque_aux(size_t n, const value_type &val, std::true_type) {
        int i=0;
        //一边添加一半
        for(; i<n/2; ++i)
            (*this).push_back(val);
        for(; i<n; ++i)
            (*this).push_front(val);
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void deque<T,Alloc>::deque_aux(InputIterator first, InputIterator last, std::false_type) {
        auto mid=(last-first)/2;
        for(auto it=first+mid; it!=first-1; --it)
            (*this).push_front(*it);
        for(auto it=first+mid+1; it!=last; ++it)
            (*this).push_back(*it);
    }
    template<class T, class Alloc>
    T** deque<T,Alloc>::getANewMap(const size_t size) {
        T** map1=new T*[size];
        for(int i=0; i!=size; ++i)
            map1[i]=getANewBuck();
        return map1;
    }
    template<class T, class Alloc>
    T* deque<T,Alloc>::getANewBuck() {
        return dataAllocator::allocate(getBuckSize());
    }
    template<class T, class Alloc>
    size_t deque<T,Alloc>::getBuckSize() const {
        return (size_t)EBucksSize::BUCKSIZE;//枚举类型
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::init() {
        mapSize=2;
        map=getANewMap(mapSize);
        start.container=finish.container=this;
        start.mapIndex=finish.mapIndex=mapSize-1;//注意本程序做了简化，实际要更复杂一些
        start.cur=finish.cur=map[mapSize-1];
    }
    template<class T, class Alloc>
    void deque<T,Alloc>::reallocateAndCopy() {
        auto newMapSize=getNewMapSize(mapSize);
        T** newMap=getANewMap(newMapSize);
        size_t startIndex=newMapSize/4;
        for(int i=0; start.mapIndex+i!=mapSize; ++i)
            for(int j=0; j!=getBuckSize(); ++j)
                newMap[startIndex+i][j]=map[start.mapIndex+i][j];//把原先的元素放到新的map的中间，便于头尾插入删除
        size_t n=start.cur-map[start.mapIndex];
        auto size=this->size();
        clear();
        mapSize=newMapSize;
        map=newMap;
        start=Detail::dq_iterator<T>(startIndex,newMap[startIndex]+n,this);
        finish=start+size;
    }
    template<class T, class Alloc>
    size_t deque<T,Alloc>::getNewMapSize(const size_t size) {
        return (size==0?2:size*2);
    }
    template<class T, class Alloc>
    bool deque<T,Alloc>::back_full() const {
        return map[mapSize-1]&&map[mapSize]==end().cur;
    }
    template<class T, class Alloc>
    bool deque<T,Alloc>::front_full() const {
        return map[0]&&map[0]==begin().cur;
    }


}//end of FgyTinySTL

#endif //FGYSTL_DEQUE_IMPL_H



















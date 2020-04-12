//
// Created by Fgy on 2020/4/11.
//

#ifndef FGYSTL_VECTOR_IMPL_H
#define FGYSTL_VECTOR_IMPL_H


namespace FgyTinySTL{
    /*
     * 对vector的成员函数进行定义
     * */

    template<class T, class Alloc>
    vector<T,Alloc>::~vector() {//析构函数
        destroyAndDeallocateAll();
    }
    template<class T, class Alloc>
    vector<T,Alloc>::vector(const size_type n) {//构建n个初始化值的构造函数
        allocateAndFillN(n,value_type());//value_type()产生一个当前容器存储对象的一个临时对象
    }
    template<class T, class Alloc>
    vector<T,Alloc>::vector(const size_type n, const value_type &value) {//构建n个初始值为value的构造函数
        allocateAndFillN(n,value);//也可以调用vector_aux(n,value,typename std::is_integral<>::type())
    }
    template<class T, class Alloc>
    template<class InputIterator>
    vector<T,Alloc>::vector(InputIterator first, InputIterator last) {//迭代器范围的构造函数
        vector_aux(first,last, typename std::is_integral<InputIterator>::type());
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::vector_aux(InputIterator first, InputIterator last, std::false_type) {
        allocateAndCopy(first,last);
    }
    template<class T, class Alloc>
    template<class Integer>
    void vector<T,Alloc>::vector_aux(Integer n, const value_type &value, std::true_type) {
        allocateAndFillN(n,value);
    }

    template<class T, class Alloc>
    vector<T,Alloc>::vector(const vector& v){//拷贝构造函数,为啥拷贝构造函数可以访问类的私有成员？类的访问限定符针对的是类，而不是某个对象
                                             //因此，只要在类内作用域，那么就可以访问类的私有成员。从使用的角度出发，类的访问限定符是对外界的权限
                                             //而类作为类设计者实现的东西，是不用考虑这些权限的
        allocateAndCopy(v.begin(),v.end());
    }
    template<class T, class Alloc>
    vector<T,Alloc>::vector(vector&& v) {//移动构造函数
        start=v.start;
        finish=v.finish;
        end_of_storage=v.end_of_storage;
        v.start=v.finish=v.end_of_storage=0;
    }
    template<class T, class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator=(const vector& v) {
        if(this!=v){
            allocateAndCopy(v.begin(),v.end());
        }
        return *this;
    }
    template<class T, class Alloc>
    vector<T,Alloc>& vector<T,Alloc>::operator=(vector&& v) {
        if(this!=v){
            destroyAndDeallocateAll();
            start=v.start;
            finish=v.finish;
            end_of_storage=v.end_of_storage;
            v.start=v.finish=v.end_of_storage=0;
        }
        return *this;
    }

    /*
     * 和容器容量相关的操作
     * */
    template<class T, class Alloc>
    void vector<T,Alloc>::resize(size_type n, value_type val) {//重新设定容器大小
        if(n<size()){
            dataAllocator::destroy(start+n,finish);
            finish=start+n;
        }
        else if(n>size()&&n<=capacity()){
            finish=FgyTinySTL::uninitialized_fill_n(finish,n-size(),val);
        }
        else if(n>capacity()){
            size_type lengthOfInsert=n-size();
            T* newStart=dataAllocator::allocate(getNewCapacity(lengthOfInsert));
            T* newFinish=FgyTinySTL::uninitialized_copy(begin(),end(),newStart);
            newFinish=FgyTinySTL::uninitialized_fill_n(newFinish,lengthOfInsert,val);
            destroyAndDeallocateAll();
            start=newStart;
            finish=newFinish;
            end_of_storage=start+n;
        }
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::reserve(size_type n) {//改变容器的capacity
        if(n<=capacity())
            return;
        T* newStart=dataAllocator::allocate(n);
        T* newFinish=FgyTinySTL::uninitialized_copy(begin(),end(),newStart);
        destroyAndDeallocateAll();
        start=newStart;
        finish=newFinish;
        end_of_storage=start+n;
    }

    /*
     * 逻辑比较相关操作
     * */
    template<class T, class Alloc>
    bool vector<T,Alloc>::operator==(const vector& v) const {
        if(size()!=v.size())
            return false;
        else
        {
            auto it1=start;
            auto it2=v.start;
            for(;it1!=finish&&it2!=v.finish;++it1,++it2)
                if(*it1!=*it2)
                    return false;
        }
        return true;
    }
    template<class T, class Alloc>
    bool vector<T,Alloc>::operator!=(const vector& v) const {
        return !(*this==v);
    }
    template<class T1, class Alloc1>
    bool operator==(const vector<T1,Alloc1>& v1,const vector<T1,Alloc1>& v2){
        return v1==v2;
    }
    template<class T1, class Alloc1>
    bool operator!=(const vector<T1,Alloc1>& v1,const vector<T1,Alloc1>& v2){
        return !(v1==v2);
    }

    /*
     * 修改容器相关操作
     * */
    template<class T, class Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator position) {
        return erase(position,position+1);//[position,position+1)
    }
    template<class T, class Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::erase(iterator first, iterator last) {//SGI STL源码用的algotithm函数，这里直接对迭代器进行操作
        difference_type lenOfTail=end()-last;
        difference_type lenOfRemoved=last-first;
        finish=finish-lenOfRemoved;
        for(; lenOfTail!=0;--lenOfTail){
            auto tmp=last-lenOfRemoved;
            *tmp=*(last++);
        }
        return first;
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::reallocateAndFillN(iterator position,const size_type &n, const value_type &val) {
        difference_type newCapacity=getNewCapacity(n);
        T* newStart=dataAllocator::allocate(newCapacity);
        T* newEndOfStorage=newStart+newCapacity;
        T* newFinish=FgyTinySTL::uninitialized_copy(begin(),position,newStart);
        newFinish=FgyTinySTL::uninitialized_fill_n(newFinish,n,val);
        newFinish=FgyTinySTL::uninitialized_copy(position,end(),newFinish);

        destroyAndDeallocateAll();
        start=newStart;
        finish=newFinish;
        end_of_storage=newEndOfStorage;
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::reallocateAndCopy(iterator position, InputIterator first,
                                            InputIterator last) {
        difference_type newCapacity=getNewCapacity(last-first);
        T* newStart=dataAllocator::allocate(newCapacity);
        T* newEndOfStorage=newStart+newCapacity;
        T* newFinish=FgyTinySTL::uninitialized_copy(begin(),position,newStart);
        newFinish=FgyTinySTL::uninitialized_copy(first,last,newFinish);
        newFinish=FgyTinySTL::uninitialized_copy(position,end(),newFinish);
        destroyAndDeallocateAll();
        start=newStart;
        finish=newFinish;
        end_of_storage=newEndOfStorage;
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::insert_aux(iterator position, InputIterator first,
                                     InputIterator last, std::false_type) {
        difference_type locationLeft=end_of_storage-finish;//储备空间
        difference_type locationNeed=last-first;//要插入的元素的个数
        if(locationLeft>=locationNeed){//储备空间足够
            if(finish-position>locationNeed)//插入点后的元素个数大于新增元素的个数
            {
                FgyTinySTL::uninitialized_copy(finish-locationNeed,finish,finish);
                std::copy_backward(position,position+locationNeed,finish);
                std::copy(first,last,position);
                finish+=locationNeed;
            } else//插入点后的元素小数大于待插入元素
            {
                iterator tmp=FgyTinySTL::uninitialized_copy(first+(finish-position),last,finish);
                FgyTinySTL::uninitialized_copy(position,finish,tmp);
                std::copy(first,first+(finish-position),position);
                finish+=locationNeed;
            }
        } else//储备空间不够
        {
            reallocateAndCopy(position,first,last);
        }
    }
    template<class T, class Alloc>
    template<class Integer>
    void vector<T,Alloc>::insert_aux(iterator position, Integer n,
                                     const value_type &value, std::true_type) {
        assert(n!=0);
        difference_type locationLeft=end_of_storage-finish;
        difference_type locationNeed=n;
        if(locationLeft>=locationNeed){
            auto tmpptr=end()-1;
            for(;tmpptr>=position; --tmpptr){
                FgyTinySTL::construct(tmpptr+locationNeed,*tmpptr);
            }
            FgyTinySTL::uninitialized_fill_n(position,n,value);
            finish+=locationNeed;
        } else
        {
            reallocateAndFillN(position,n,value);
        }
    }

    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::insert(iterator position, InputIterator first, InputIterator last) {
        insert_aux(position,first,last,typename std::is_integral<InputIterator>::type());
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::insert(iterator position, const size_type &n, const value_type &val) {
        insert_aux(position,n,val, typename std::is_integral<size_type>::type());
    }
    template<class T, class Alloc>
    typename vector<T,Alloc>::iterator vector<T,Alloc>::insert(iterator position, const value_type &val) {
        const auto index=position-begin();
        insert(position,1,val);
        return begin()+index;
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::push_back(const value_type &value) {
        insert(end(),value);
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::pop_back() {
        --finish;
        dataAllocator::destroy(finish);
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::clear() {
        dataAllocator::destroy(start,finish);
        finish=start;
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::swap(vector& v) {
        if(this!=&v) {
            T *tmp;
            tmp = start;
            start = v.start;
            v.start = tmp;
            tmp = finish;
            finish = v.finish;
            v.finish = tmp;
            tmp = end_of_storage;
            end_of_storage = v.end_of_storage;
            v.end_of_storage = tmp;
        }
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::shrink_to_fit() {
        T* t=dataAllocator::allocate(size());
        finish=FgyTinySTL::uninitialized_copy(begin(),end(),t);
        dataAllocator::deallocate(start,capacity());
        start=t;
        end_of_storage=finish;
    }

    template<class T, class Alloc>
    void vector<T,Alloc>::destroyAndDeallocateAll() {//析构元素并销毁空间
        if(capacity()!=0)
        {
            dataAllocator::destroy(start,finish);
            dataAllocator::deallocate(start,capacity());
        }
    }
    template<class T, class Alloc>
    void vector<T,Alloc>::allocateAndFillN(const size_type n, const value_type &value) {//分配空间并初始化为n个value
        //dataAllocator Allocator<T>
        start=dataAllocator::allocate(n);
        FgyTinySTL::uninitialized_fill_n(start,n,value);
        finish=end_of_storage=start+n;
    }
    template<class T, class Alloc>
    template<class InputIterator>
    void vector<T,Alloc>::allocateAndCopy(InputIterator first, InputIterator last) {//分配空间，并由迭代器范围初始化元素
        start=dataAllocator::allocate(last-first);//两个迭代器相减的操作应该是迭代器已经定义好的了
        finish=FgyTinySTL::uninitialized_copy(first,last,start);
        end_of_storage=finish;
    }

    template<class T, class Alloc>
    typename vector<T,Alloc>::size_type vector<T,Alloc>::getNewCapacity(size_type len) const {
        size_type oldCapacity=end_of_storage-start;
        size_type res=std::max(oldCapacity,len);
        size_type newCapacity=(oldCapacity!=0)?oldCapacity+res:len;
        return newCapacity;
    }
}

#endif //FGYSTL_VECTOR_IMPL_H

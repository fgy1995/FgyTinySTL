//
// Created by Fgy on 2020/4/28.
//

#ifndef FGYSTL_HEAPALGORITHM_H
#define FGYSTL_HEAPALGORITHM_H

#include "Utility.h"
#include "Functional.h"
#include "Iterator.h"
/*
 * 定义建堆用的一些方法，及堆排序
 * */
namespace FgyTinySTL{
   /*
    * make_heap算法，分别有上溯和下溯算法，上溯从下向上开始建堆，下溯从上到下开始建堆
    * */

   template<class RandomAccessIterator, class Compare>
   static void up(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp){
       //迭代器的范围为[first,last],堆的头部为head，注意这里堆的头部前面并没有一个空位，所以要注意父子关系的运算
       //这个函数把为元素置于正确位置
       if(first!=last){
           int index=last-first;
           auto parentIndex=(index-1)/2;
           for(auto cur=last; parentIndex>=0&&cur!=head; parentIndex=(index-1)/2){
               auto parent=head+parentIndex;//获取parent迭代器
               if(comp(*parent,*cur))
                   FgyTinySTL::swap(*cur,*parent);
               cur=parent;
               index=cur-head;
           }
       }
   }

   template<class RandomAccessIterator, class Compare>
   static void down(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator head, Compare comp){
       //下溯算法，把堆顶置于正确位置
       if(first!=last){
           int index=first-head;
           auto leftChildIndex=2*index+1;
           for(auto cur=first; leftChildIndex<(last-head+1)&&cur<last; leftChildIndex=2*index+1){
               auto child=head+leftChildIndex;
               if(child+1<=last&&*(child+1)>*child)//找到左右孩子最大值
                   child=child+1;
               if(comp(*cur,*child))
                   FgyTinySTL::swap(*cur,*child);
               cur=child;
               index=cur-head;
           }
       }
   }

   //建堆算法
   template<class RandomAccessIterator>
   void make_heap(RandomAccessIterator first, RandomAccessIterator last){
       FgyTinySTL::make_heap(first,last,typename FgyTinySTL::less<typename FgyTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
   }
   template<class RandomAccessIterator, class Compare>
   void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
       const auto range=last-first;
       for(auto cur=first+range/2-1; cur>=first; --cur){
           FgyTinySTL::down(cur,last-1,first,comp);
           if(cur==first) return;
       }
   }

   //插入算法
   template<class RandomAccessIterator>
   void push_heap(RandomAccessIterator first, RandomAccessIterator last){
       FgyTinySTL::push_heap(first,last, typename FgyTinySTL::less<typename FgyTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
   }
   template<class RandomAccessIterator, class Compare>
   void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
       FgyTinySTL::up(first,last-1,first,comp);
   }
   //弹出元素算法
   template<class RandomAccessIterator>
   void pop_heap(RandomAccessIterator first, RandomAccessIterator last){
       FgyTinySTL::pop_heap(first,last, typename FgyTinySTL::less<typename FgyTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
   }
   template<class RandomAccessIterator, class Compare>
   void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
       FgyTinySTL::swap(*first,*(last-1));
       if(last-first>=2)
           FgyTinySTL::down(first,last-2,comp);
   }

   //堆排序算法
   template<class RandomAccessIterator>
   void sort_heap(RandomAccessIterator first, RandomAccessIterator last){
       FgyTinySTL::sort_heap(first,last, typename FgyTinySTL::less<typename FgyTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
   }
   template<class RandomAccessIterator, class Compare>
   void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
       for(auto cur=last; cur!=first; --cur)
           FgyTinySTL::pop_heap(first,cur,comp);
   }

   //判断一组数据是否为堆
   template<class RandomAccessIterator>
   bool is_heap(RandomAccessIterator first, RandomAccessIterator last){
       return FgyTinySTL::is_heap(first,last, typename FgyTinySTL::less<typename FgyTinySTL::iterator_traits<RandomAccessIterator>::value_type>());
   }
   template<class RandomAccessIterator, class Compare>
   bool is_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp){
       const auto range=last-first;
       auto index=range/2-1;
       for(auto cur=first+range/2-1; cur>=first; --cur,--index){
           if(*(first+(2*index+1))>*cur||((first+(2*index+2))<=last&&*(first+(2*index+2))>*cur))
               return false;
           if(cur==first)
               break;
       }
       return true;
   }


}//end of FgyTinySTL

#endif //FGYSTL_HEAPALGORITHM_H



















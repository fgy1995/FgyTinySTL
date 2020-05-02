//
// Created by Fgy on 2020/4/28.
//

#ifndef FGYSTL_UTILITY_H
#define FGYSTL_UTILITY_H
/*
 * 定义一些常用的全局方法，也可以叫做工具方法
 * */

namespace FgyTinySTL{
    //数据交换方法
    template<class T>
    void swap(T& a,T& b){
        T tmp=a;
        a=b;
        b=tmp;
    }
}


#endif //FGYSTL_UTILITY_H

//
// Created by Fgy on 2020/4/18.
//

#ifndef FGYSTL_FUNCTIONAL_H
#define FGYSTL_FUNCTIONAL_H

namespace FgyTinySTL{
    /*
     * 定义判断小于的函数对象，也就是一个类重载了()运算符
     * */
    template<class T>
    struct less{
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;

        result_type operator()(const first_argument_type& x, const second_argument_type& y){
            return x<y;
        }
    };
    /*
     * 定义判断等于的函数对象
     * */
    template<class T>
    struct equal_to{
        typedef T first_argument_type;
        typedef T second_argument_type;
        typedef bool result_type;
        result_type operator()(const first_argument_type& x, const second_argument_type& y){
            return x==y;
        }
    };
}

#endif //FGYSTL_FUNCTIONAL_H

//
// Created by Fgy on 2020/4/8.
//

#ifndef FGYSTL_TYPETRAITS_H
#define FGYSTL_TYPETRAITS_H

/*
 * type_traits用来判断某个类型是否具有trivial defalt_ctor, trivial copy_ctor, trivial assignment operator等
 * 内置类型以及原生指针都是具有以上类型，而自定义的类其数据成员的情况来判断是否具有上面的类型。
 * 这我的实现，自定义类都不具有上面的类型，而有的编译器，具有可以推断出一个类是否具有上面的类型。
 * */
namespace FgyTinySTL{

    /*
     * 这块的定义在STL源码剖析的第三章并没有介绍，在别人的代码中我看到的。
     * 并且参考SGI STL源码我觉得这段代码的作用是针对一些编译器可能会自动生成type_traits
     * 使用这段代码的作用可能是如果编译器自动生成type_traits，那么就用编译器生成的。
     * 到底有什么作用，之后会继续跟踪。
     * */
    namespace{
        template<bool, class Ta, class Tb>//bool是非类型模板参数
        struct IfThenElse;
        template<class Ta, class Tb>
        struct IfThenElse<true, Ta, Tb>{
            using result=Ta;
        };
        template<class Ta, class Tb>
        struct IfThenElse<false, Ta, Tb>{
            using result=Tb;
        };
    }
    struct _true_type{};
    struct _false_type{};
    /*
     * 萃取传入T类型的类型特性
     * */
    template<class T>
    struct _type_traits{
        typedef _false_type has_trivial_default_constructor;
        typedef _false_type has_trivial_copy_constructor;
        typedef _false_type has_trivial_assignment_operator;
        typedef _false_type has_trivial_destructor;
        typedef _false_type is_POD_type;// 全称plain old data，plain代表它是一个普通类型，old代表它可以与c兼容，（这是一个c++11概念，可以去博客上看看）
                                        // 可以使用比如memcpy()这类c中最原始函数进行操作。C++11中把POD分为了两个基本概念的集合
                                        // 即：平凡的（trival）和标准布局的（standard layout）。
    };
    /*
     * type_traits的特化版本，可以把内置类型和原生指针的的类型特性萃取出来。
     * */
    template<>//把类型全部特化的话，那么模板后的<>就不用把模板类型写出来了
    struct _type_traits<bool>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<char>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned char>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<signed char>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<wchar_t>{//宽字节，char 1字节，wchar_t 2字节
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<short>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned short>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<int>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned int>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<long>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned long>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<long long>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned long long>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<float>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<double>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<long double>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<class T>
    struct _type_traits<T*>{//原生指针的特化版本
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<class T>
    struct _type_traits<const T*>{//const原生指针的特化版本
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    /*
     * 下面定义char*的各种动作
     * */
    template<>
    struct _type_traits<char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<unsigned char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<signed char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<const char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<const unsigned char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
    template<>
    struct _type_traits<const signed char*>{
        typedef _true_type has_trivial_default_constructor;
        typedef _true_type has_trivial_copy_constructor;
        typedef _true_type has_trivial_assignment_operator;
        typedef _true_type has_trivial_destructor;
        typedef _true_type is_POD_type;
    };
}
#endif //FGYSTL_TYPETRAITS_H

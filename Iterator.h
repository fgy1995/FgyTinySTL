//
// Created by Fgy on 2020/4/8.
//

#ifndef FGYSTL_ITERATOR_H
#define FGYSTL_ITERATOR_H

/*
 * 这部分尽可能地实现了STL迭代器的内容，但是对比SGI STL的Iterator实现还是有一定的不同
 * SGI STL关系迭代器头文件的实现主要在stl_iterator.h和stl_iterator_base.h这两个文件中
 * */

namespace FgyTinySTL{
    /*
     * 定义迭代器标签，用于激活重载函数
     * 使用了继承的结果，确保可以把继承类赋值给父类，这与类的多态不一样，类的多态是指基类的指针或引用指向继承类的时候，可以调用继承类的虚函数，动态绑定
     * */
    struct input_iterator_tag{};
    struct output_iterator_tag{};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    /*
     * 这部分由于定义迭代器类型，input、output等，这部分在STL源码剖析的第三章并没有提，应该在具体介绍容器的时候会使用
     * 我是看别人的代码有这块内容，我自己考虑了一下也给加上了
     * */
    template<class T, class Distance>
    struct input_iterator{
        typedef input_iterator_tag iterator_category;
        typedef T                  value_type;
        typedef Distance           difference_type;
        typedef T*                 pointer;
        typedef T&                 reference;
    };
    struct output_iterator{
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;
    };
    template<class T, class Distance>
    struct forward_iterator{
        typedef forward_iterator_tag iterator_category;
        typedef T                    value_type;
        typedef Distance             difference_type;
        typedef T*                   pointer;
        typedef T&                   reference;
    };
    template<class T, class Distance>
    struct bidirectional_iterator{
        typedef bidirectional_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef Distance                   difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
    };
    template<class T, class Distance>
    struct random_access_iterator{
        typedef random_access_iterator_tag iterator_category;
        typedef T                          value_type;
        typedef Distance                   difference_type;
        typedef T*                         pointer;
        typedef T&                         reference;
    };
    /*
     * 定义迭代器类，自己开发的迭代器最好继承这个类，确保不会丢失iterator的相应类型
     * */
    template<class Category, class T, class Distance=ptrdiff_t, class Pointer=T*, class Reference=T&>
    struct iterator{
        typedef Category    iterator_category;
        typedef T           value_type;
        typedef Distance    difference_type;
        typedef Pointer     pointer;
        typedef Reference   reference;
    };
    /*
     * 定义迭代器萃取器，可以从迭代器推断出相应类型
     * 定义迭代器萃取器的好处是，可以使用模板的偏特化，这样对于原生指针也可以获得迭代器的相应类型
     * */
    template<class Iterator>
    struct iterator_traits {
        typedef typename Iterator::iterator_category    iterator_category;
        typedef typename Iterator::value_type           value_type;
        typedef typename Iterator::differnce_type       difference_type;
        typedef typename Iterator::pointer              pointer;
        typedef typename Iterator::reference            reference;
    };
    /*
     * 定义迭代器萃取器的原生指针的偏特化版本
     * */
    template<class T>
    struct iterator_traits<T*>{
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef T*                              pointer;
        typedef T&                              reference;
    };
    /*
     * 定义迭代器萃取的const原生指针的偏特化版本
     * */
    template<class T>
    struct iterator_traits<const T*>{
        typedef random_access_iterator_tag      iterator_category;
        typedef T                               value_type;
        typedef ptrdiff_t                       difference_type;
        typedef const T*                        pointer;
        typedef const T&                        reference;
    };

    /*
     * 定义一个返回迭代器类型的函数
     * */
    template<class Iterator>
    inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator& ){
        typedef typename iterator_traits<Iterator>::iterator_category category;
        return category();//建立一个category()的临时对象返回，category()返回的就是上面写的5种迭代器tag的类型
    }

    /*
     * 定义函数返回迭代器所指类型的指针
     * */
    template<class Iterator>
    inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
    }

    template<class Iterator>
    inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&){
        return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
    }
}

#endif //FGYSTL_ITERATOR_H
//
// Created by Fgy on 2020/4/4.
//

/*
 * STL标准规定STL必须有一个名为Allocator的头文件，虽然SGI STL有这样一个文件，但是实际上SGI使用的是alloc头文件进行空间配置
 * SGI的alloc将空间配置分为两个，一个是第一级空间配置器，一个是第二级空间配置器，并且alloc提供一个simple_alloc的类作为访问这两个的配置的接口
 * 通过simple_alloc访问可以不用指明调用哪一级配置器，会根据资源的大小自动调用；
 * 第一级空间配置器：使用malloc进行空间分配，它分配的资源大于128bytes；使用free回收空间；并在空间不足时，使用oom_malloc,oom_remalloc这两个函数，
 * 继续申请空间，这两个函数使用了一个用户定义的函数_malloc_alloc_oom_handler不断获得内存资源。
 * 因为第一级空间配置器使用了c语言的malloc，因此它必须模拟new出现异常后的处理，new出现异常后，会根据set_new_handler（）设定的异常处理函数。
 *
 * 第二级配置器：二级空间配置器处理资源小于等于bytes。它维护一个内存池（memory pool）并使用一个名为自由链表的数组来配置空间，因为第二级空间配置器
 * 涉及到了多线程的知识，目前还不考虑，所以不把二级配置器定义为模板，但是一样可以使用。
 *
 * 另外为了方便，此版本没写simple_alloc类作为接口。直接使用二级配置器
 * */

#ifndef FGYSTL_ALLOC_H
#define FGYSTL_ALLOC_H
#include <new>
#include <cstdlib>


namespace FgyTinySTL{
    class _malloc_alloc {//定义第一级空间配置器，但不使用它，因为用户自定义的空间处理函数我不会写,函数的实现放在detail文件夹
    private:
        //下面的函数用来处理内存不足的情况
        static void *oom_malloc(size_t);
        static void *oom_realloc(void*, size_t);
        static void (* _malloc_alloc_oom_handler)();//函数指针，用来保存用户指定的内存空间处理函数。

    public:

        static void *allocate(size_t n){//配置空间。返回的是void*，需要根据不同类型而自动进行强转
            void *result=malloc(n);
            if(result==0) result=oom_malloc(n);//空间不足，调用oom_malloc()处理。
            return result;
        }

        static void deallocate(void* ptr,size_t /*n*/){//回收空间。ptr为allocate分配的空间
            free(ptr);
        }

        static void *reallocate(void* ptr, size_t /*old_size*/, size_t new_size){//重新配置空间
            void *result=realloc(ptr,new_size);
            if(result==0) result=oom_realloc(ptr,new_size);
            return result;
        }

        /*
         * 下面这是一个函数，从里向外看，set_malloc_handler后面有直接的括号说明set_malloc_handler是函数，它的参数是一个函数指针 void (*f)(),
         * 它的返回值也是一个函数指针void (*f)()。用后置返回类型表示能清晰些 static set_malloc_handler(void (*f) ())->void (*f)() {}
         * */
        static void (* set_malloc_handler(void (*f) ()))(){//设置用户自定义的_malloc_alloc_oom_handler函数
            void (*old)() =_malloc_alloc_oom_handler;
            _malloc_alloc_oom_handler=f;
            return (old);
        }
    };
    typedef _malloc_alloc malloc_alloc;//为一级配置器起个别名

    /*二级配置器的实现，注意不考虑线程所以没定义，
     * 二级配置器的核心在于维护自由链表。
     * */
    class alloc{
    private:
        enum EAlign{ALIGN=8};//小型区块的上调边界
        enum EMaxBytes{MAXBYTES=128};//小型区块最大值，超过它由一级配置器分配
        enum EFreeLists{NFREELISTS=(EMaxBytes::MAXBYTES/EAlign::ALIGN)};//自由链表的个数
        enum ENObjs{NOBJS=20};//每次增加的节点数
    private:
        union obj{//建立一个联合体，既可以当做链表节点又可以当做为用户分配的内存。
            union obj* next_node;
            char client_data[1];
        };
        static obj* volatile free_list[EFreeLists::NFREELISTS];
    private:
        static char *start_free;//内存池开始位置
        static char *end_free;//内存池结束位置
        static size_t heap_size;//内存池大小
    private:
        static size_t ROUND_UP(size_t bytes){//将bytes上调至8倍数
            return ((bytes+EAlign::ALIGN-1)&~(EAlign::ALIGN-1));
        }

        static size_t FREELIST_INDEX(size_t bytes){//获得该bytes在数组上的位置，进而可以访问该bytes的自由链表
            return (((bytes)+EAlign::ALIGN-1)/EAlign::ALIGN-1);
        }

        static void *refill(size_t bytes);//空间不够了重新填充空间，返回一个大小为bytes的空间，并可以加入到freelist中
        static char *chunk_alloc(size_t bytes, size_t& nobjs);//注意nobjs一定要用引用的形式，因为nobjs的值可能会变
    public:
        static void *allocate(size_t bytes);
        static void deallocate(void* ptr, size_t bytes);
        static void *reallocate(void* ptr, size_t old_sz, size_t new_sz);
    };

}

#endif //FGYSTL_ALLOC_H

//
// Created by Fgy on 2020/4/4.
//
#include "../Alloc.h"

namespace FgyTinySTL{
    /*下面是一级配置器的定义*/
    void (* _malloc_alloc::_malloc_alloc_oom_handler)() =0;
    void* _malloc_alloc::oom_malloc(size_t n) {
        void (* my_malloc_handler)();
        void *result;
        for(;;)
        {
            my_malloc_handler=_malloc_alloc_oom_handler;
            if(my_malloc_handler==0) {exit(0);}
            (*my_malloc_handler)();//调用用户自定义函数
            result=malloc(n);
            if(result) return (result);
        }
    }
    void* _malloc_alloc::oom_realloc(void* ptr, size_t n) {
        void (* my_malloc_handler)();
        void *result;
        for(;;)
        {
            my_malloc_handler=_malloc_alloc_oom_handler;
            if(my_malloc_handler==0) {exit(0);}
            (*my_malloc_handler)();
            result=realloc(ptr,n);
            if(result) return (result);
        }
    }

    /*下面写二级配置器的实现*/
    char *alloc::start_free=0;
    char *alloc::end_free=0;
    size_t alloc::heap_size=0;
    alloc::obj* volatile alloc::free_list[alloc::EFreeLists::NFREELISTS]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    void* alloc::allocate(size_t bytes) {//从自由链表中分配空间
        if(bytes>(size_t)EMaxBytes::MAXBYTES)//大于区块的最大值
            return malloc(bytes);////注意：SGISTL标准是调用第一级配置器，这里我为了方便直接使用malloc申请空间了
        size_t index=FREELIST_INDEX(bytes);
        obj* volatile result=free_list[index];
        if(result==0)//这个大小的区块已经没有了，调用refill()分配区块，refill返回一个bytes大小的区块，并为freelist配置新的区块
        {
            //第一次一定会调用refill为自由链表分配空间
            void *r=refill(ROUND_UP(bytes));
            return r;
        }
        free_list[index]=result->next_node;
        return result;
    }

    void alloc::deallocate(void *ptr, size_t bytes) {
        if(bytes>alloc::EMaxBytes::MAXBYTES)//大于区块最大值，直接用free释放
        {
            free(ptr);
            return;
        }
        size_t index=FREELIST_INDEX(bytes);
        obj* q= static_cast<obj*>(ptr);
        q->next_node=free_list[index];//把这个区块头插到自由链表中
        free_list[index]=q;
    }

    void *alloc::reallocate(void *ptr, size_t old_sz, size_t new_sz) {
        deallocate(ptr,old_sz);
        ptr=allocate(new_sz);
        return ptr;
    }
    
    void *alloc::refill(size_t bytes) {//当自由链表区块不足的时候，refill为其分配新的区块
        size_t nobjs=ENObjs::NOBJS;
        char *chunk=chunk_alloc(bytes,nobjs);
        if(nobjs==1)
            return chunk;
        obj* volatile* my_free_list=free_list+FREELIST_INDEX(bytes);
        obj* result=(obj*)chunk;
        *my_free_list=(obj*)(chunk+bytes);
        obj* next_obj=(obj*)(chunk+bytes);
        obj* cur_obj;
        for(int i=1;;++i)
        {
            cur_obj=next_obj;
            next_obj=(obj*)((char*)next_obj+bytes);
            if(nobjs-1==i)
            {
                cur_obj->next_node=0;
                break;
            } else{
                cur_obj->next_node=next_obj;
            }
        }
        return result;
    }
    char *alloc::chunk_alloc(size_t bytes, size_t& nobjs) {
        char *result;//分配内存的首地址
        size_t total_bytes=bytes*nobjs;//一共分配的bytes
        size_t left_bytes=end_free-start_free;//内存池剩余的bytes
        if(left_bytes>=total_bytes)//内存池剩余的bytes足够分配
        {
            result=start_free;
            start_free+=total_bytes;
            return result;
        }
        else if(left_bytes>bytes)//内存池剩余的不能满足所有的需求，但是最少能分配一个区块
        {
            nobjs=left_bytes/bytes;
            total_bytes=bytes*nobjs;
            result=start_free;
            start_free+=total_bytes;
            return result;
        }
        else //内存池剩余的连一个区块都分配不了了，这个时候需要把内存池里剩余的所有内存分配给其他区块，并申请新的空间，想尽一切方法，如果还不可以的话，只能抛出异常
        {
            size_t bytes_to_get=2*total_bytes+ROUND_UP(heap_size>>4);//重新向堆申请的资源你的大小
            if(left_bytes>0)//把内存池仅剩的一点资源给分配了
            {
                obj* volatile* my_free_list=free_list+FREELIST_INDEX(left_bytes);//my_free_list,是指定对于自由链表的头结点的地址，需要对它解引用才能获得他存的地址
                ((obj*)start_free)->next_node=*my_free_list;
                *my_free_list=(obj*)start_free;
            }
            start_free=(char*)malloc(bytes_to_get);
            if(start_free==0)//系统堆也没有空间了，试试能不能从别的自由链表中找到可用的区块
            {
                obj* volatile* my_free_list;
                obj* volatile p;
                for(int i=bytes; i<EMaxBytes::MAXBYTES; i+=EAlign::ALIGN)//遍历大于等于bytes的自由链表
                {
                    my_free_list=free_list+FREELIST_INDEX(i);
                    p=*my_free_list;
                    if(p!=0)//该自由链表上还有空间且空间一定大于bytes，取出一块出来
                    {
                        *my_free_list=p->next_node;
                        start_free=(char*)p;
                        end_free=start_free+i;
                        return chunk_alloc(bytes,nobjs);//递归调用自己，修改nobjs
                    }
                }
                end_free=0;//自由链表也没有节点了，放弃吧
                //其实SGI STL，在这还有一步，start_free=(char*)malloc_alloc::allocate(bytes_to_get);通过调用一级配置器来获得资源，因为一级配置器有用户自定义的资源出来函数，所以有机会获得一定的内存空间
            }
            heap_size+=bytes_to_get;
            end_free=start_free+bytes_to_get;
            return chunk_alloc(bytes,nobjs);//递归调用自己，修改nobjs
        }
    }
}

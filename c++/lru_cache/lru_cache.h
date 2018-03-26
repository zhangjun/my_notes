/****************************************************************
    File Name    : lru_cache.h
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-03-11 15-47
*****************************************************************/

#ifndef _LRU_CACHE_H
#define _LRU_CACHE_H

#include<unorderd_map>
#include<list>

class LRUCache {
    public:
        LRUCache(int capacity) : _capacity(capacity){}

        int Get(int key);
        void Set(int key, int value);
    private:
        std::unordered_map<int, std::list< std::pair<int, int> >::iterator> map_;
        std::list< std::pair<int, int> >list_;
        int capacity_;
};


#endif

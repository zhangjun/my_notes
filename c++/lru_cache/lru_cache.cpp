/****************************************************************
    File Name    : lru_cache.cpp
    Author       : Jun Zhang
	Mail         : zhangjun9@staff.weibo.com 
	Created Time : 2018-03-11 15-53
*****************************************************************/

#include "lru_cache.h"

int LRUCache::get(int key){
    if(map_.count(key) == 0)
        return -1;

    std::list< std::pair<int, int> >::iterator it = map_[key];
    int value = it -> second;
    list_.erase(it);
    list_.push_front(std::pair<int, int>(key, value));
    map_[key] = list_.begin();
    return value;
}

void LRUCache::set(int key, int value){
    if(map_.count(key) != 0){
        std::list<std::pair<int, int> >::iterator it = map_[key];
        list_.erase(it);
        list_.push_back(std::pair<int, int>(key, value));
        map_[key] = list_.begin();
    }
    else {
        if(map_.size() == capacity_){
            map_.erase(list_.back().first);
            list_.pop_back();
        }
        list_.push_front(std::pair<int, int>(key, value));
        map_[key] = list_.begin();
    }

}

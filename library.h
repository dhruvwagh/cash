#ifndef CASH_LIBRARY_H
#define CASH_LIBRARY_H
#include<iostream>
#include<map>
#include<memory>
#include<any>
#include<vector>
#include<unordered_map>

typedef std::unordered_map<int16_t,std::pair<int16_t,std::any>> c_map;
std::vector<std::pair<c_map*,int16_t>> key_list;
int16_t kit;

namespace Cache{
    int PoolId=0;
    static std::vector<c_map*> space;
}

struct Item{
    static std::any d;
    static int16_t Key;
    static int16_t age;
    static c_map* pool;
};

c_map* create_pool(){
    static c_map pool;
    pool.reserve(256);
    c_map* poolid = &pool;
    Cache::space.push_back(poolid);
    std::pair<c_map*,int16_t> pk_pair;
    pk_pair.first = poolid;
    pk_pair.second=0;
    key_list.push_back(pk_pair);
    return poolid;
}

void evict_LRU(c_map* in_pool_id){
    c_map a= *in_pool_id;
    int16_t max = 0, oldest_key;
    std::pair<int16_t,std::any> oldest;
    for (auto& i:a){
        oldest = i.second;
        if (oldest.first > max ){
            max = oldest.first;
            oldest_key = i.first;
        }
    }
    a.erase(oldest_key);
}

Item create_item(std::any val, c_map* pid){
    Item i;
    i.age = 0;
    i.d =val;
    i.Key =kit;
    i.pool = pid;
    kit+=1;
    return i;
}

void insert_key(Item i){
    c_map map = *i.pool;
    std::pair<int16_t , std::any> val;
    std::pair<int16_t , std::pair<int16_t ,std::any>> kv_pair;
    val.first=i.age;
    val.second=i.d;
    kv_pair.first = i.Key;
    kv_pair.second = val;
    map.insert(kv_pair);
}




#endif //CASH_LIBRARY_H
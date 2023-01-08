#ifndef CASH_LIBRARY_H
#define CASH_LIBRARY_H
#include<iostream>
#include<map>
#include<memory>
#include<any>
#include<vector>
#include<unordered_map>

typedef std::unordered_map<int16_t, std::pair<int16_t, std::any>> c_map;
std::vector<std::pair<c_map *, int16_t>> key_list;
int16_t kit;

namespace Cache {
    int PoolId = 0;
    static std::vector<c_map *> space;
}

//defining an item:

struct Item {
    static std::any d;
    static int16_t Key;
    static int16_t age;
    static c_map *pool;
};

c_map *create_pool() {
    static c_map pool;
    pool.reserve(256); // reserving memory space
    c_map *poolid = &pool; // pool(map) address
    Cache::space.push_back(poolid); // adding a pool to the memory space
    std::pair<c_map *, int16_t> pk_pair;
    pk_pair.first = poolid;
    pk_pair.second = 0;
    key_list.push_back(pk_pair);
    return poolid;
}

void evict_LRU(c_map* in_pool_id) {
    c_map a = *in_pool_id;
    int16_t max = 0, oldest_key;
    std::pair<int16_t, std::any> oldest;

    // searching for the oldest member ( find a better soln)

    for (auto &i: a) {
        oldest = i.second;
        if (oldest.first > max) {
            max = oldest.first;
            oldest_key = i.first;
        }
    }
    a.erase(oldest_key);
}

// creating an item to be assigned in a pool, the function initializes the parameters and returns the object of "Item" type

Item create_item(std::any val, c_map *pid) {
    Item i;
    i.age = 0;
    i.d = val;
    i.Key = kit;
    i.pool = pid;
    kit += 1;
    return i;
}

void increase_age(c_map *pool) {
    c_map map = *pool;
    for (auto i: map) {      // increasing the age
        i.second.first += 1;
    }
}
//checks if map is empty before, to be added, option to choose eviction policy.

void insert_key(Item i) {
    c_map map = *i.pool;
    std::pair<int16_t, std::any> val;
    std::pair<int16_t, std::pair<int16_t, std::any>> kv_pair;
    val.first = i.age;
    val.second = i.d;
    kv_pair.first = i.Key;
    kv_pair.second = val;
    if (map.size() >= 256) {
        evict_LRU(i.pool);
    }
    map.insert(kv_pair);
    increase_age(i.pool);
}

//finding an element

std::any find(c_map *pool, int16_t Key) {
    c_map map = *pool;
    auto f = map.find(Key);
    std::pair<int16_t, std::any> d_pair = f->second;
    increase_age(pool);
    return d_pair.second;
}

void change_value(int16_t key, std::any new_d, c_map *pool) {
    c_map map = *pool;
    map.at(key).second = new_d;
    increase_age(pool);
}

void flush(c_map *pool) {
    c_map map = *pool;
    map.clear();
}

int16_t get_size(c_map *pool) {
    c_map map = *pool;
    return map.size();
}

#endif //CASH_LIBRARY_H
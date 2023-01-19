#ifndef CASH_LIBRARY_H
#define CASH_LIBRARY_H

#include<iostream>
#include<map>
#include<memory>
#include<any>
#include<vector>
#include<unordered_map>
#include<list>
#include <algorithm>

using namespace std;


/*namespace Cache {
    int PoolId = 0;
    static std::vector<c_map *> space;
}
 */

//defining an item:

struct Item {
    std::any d;
    std::string Key;
    int16_t age;

    bool operator==(const Item &b) const {
        return (Key == b.Key);
    }
};
typedef std::unordered_map<std::string, Item *> c_map;
std::map<c_map *, std::list<Item>> pool_list;

c_map create_pool() {
    c_map pool;
    pool.reserve(256); // reserving memory space
    c_map *poolid = &pool; // pool(map) address
    std::pair<c_map *, std::list<Item>> pk_pair;
    std::list<Item> ls;
    pk_pair = std::make_pair(poolid, ls);
    pool_list.insert(pk_pair);
    return pool;
}

void evict_LRU(c_map* in_pool_id) {
    c_map a = *in_pool_id;
    auto it = pool_list.find(in_pool_id);
    std::list<Item> ls = it->second;
    ls.pop_back();
    //int16_t max = 0, oldest_key;
    /* searching for the oldest member ( find a better soln)
    auto oldest = std::max_element(a.begin(), a.end(),
                     [](auto x, auto y) { return x.second.first <  y.second.first; });
    a.erase(oldest->first);
     */
}

// creating an item to be assigned in a pool, the function initializes the parameters and returns the object of "Item" type

Item create_item(std::string Key) {
    Item i;
    i.Key = Key;
    i.d = 0;
    i.age = 0;
    return i;
}

/*void increase_age(c_map *pool) {
    c_map map = *pool;
    for (auto i: map) {      // increasing the age
        i.second.first += 1;
    }
}
 */
//checks if map is empty before, to be added, option to choose eviction policy.

void insert_item(Item i, c_map *poolid) {
    c_map map = *poolid;
    auto it = pool_list.find(poolid);
    std::list<Item> ls = it->second;
    //val.first = i.age;
    //val.second = i.d;
    if (map.size() >= 256) {
        evict_LRU(poolid);
    }
    ls.push_front(i);
    Item *n = &i;
    map.insert({i.Key, n});
    //increase_age(i.pool);
}

//finding an element
void update_pos(c_map *poolid, Item i) {
    auto it = pool_list.find(poolid);
    Item a = i;
    std::list<Item> ls = it->second;
    std::list<Item>::iterator it1 = std::find(ls.begin(), ls.end(), i);

}

Item access(c_map *pool, std::string Key) {
    /*c_map map = *pool;
    auto f = map.find(Key);
    Item i = *f->second;
    update_pos(pool, i);*/
    c_map map = *pool;
    auto it = pool_list.find(pool);
    auto it1 = map.find(Key);
    Item *i = it1->second;
    std::list<Item> ls = it->second;
    update_pos(pool, *i);
    return *i;
}

void change_value(string key, std::any new_d, c_map *pool) {
    c_map map = *pool;
    Item i = access(pool, key);
    i.d = new_d;
}

void flush(c_map *pool) {
    c_map map = *pool;
    auto it = pool_list.find(pool);
    list<Item> ls = it->second;
    ls.clear();
    map.clear();
}

int16_t get_size(c_map *pool) {
    c_map map = *pool;
    return map.size();
}

#endif //CASH_LIBRARY_H
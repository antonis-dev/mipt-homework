//
// Created by antonis on 6/28/22.
//

#ifndef INCLUDE_LRU_CACHE_H_
#define INCLUDE_LRU_CACHE_H_

#include <assert.h>
#include <functional>
#include <unordered_map>

#include <list>
#include <iostream>

namespace homework {

template <class ItemValue, class ItemId = int, bool with_value = true>
class lru_cache {
 public:
    template<class ItemValue_, class ItemId_>
    struct value_item {
        ItemId_ id;
        ItemValue_ value;
    };

    template<class ItemId_>
    struct no_value_item {
        ItemId_ id;
    };

    using Item = typename std::conditional<with_value, value_item<ItemValue, ItemId>, no_value_item<ItemId>>::type;

    using list_t = std::list<Item>;
    using lru_hash = std::unordered_map<ItemId, typename list_t::iterator>;
    using lru_iter = typename list_t::iterator;
    using const_iter_t = typename list_t::const_iterator;
    using data_cb_t = std::function<ItemValue(ItemId)>;

    lru_cache(size_t size, data_cb_t data_cb = default_data_cb()) : size_(size), data_cb_(data_cb) {
    }

    bool full() {
        return list_.size() >= size_;
    }

    const lru_iter tail() {
        assert(("cache is empty", list_.size() > 0));
        return list_.end();
    }

    const lru_iter front() {
        assert(("cache is empty", list_.size() > 0));
        return list_.begin();
    }

    bool contains(ItemId key) {
        return hash_.count(key) > 0;
    }

    bool lookup_update(ItemId key) {
        auto hit_it = hash_.find(key);
        if (hit_it == hash_.end()) {
            if (full()) {
                hash_.erase(list_.back().id);
                list_.pop_back();
            }

            list_.push_front(create_item(key));
            hash_[key] = list_.begin();

            return false;
        }

        auto el_it = hit_it->second;
        if (el_it != list_.begin())
            list_.splice(list_.begin(), list_, el_it, std::next(el_it));

        return true;
    }

    static data_cb_t default_data_cb() {
        return std::bind([](ItemId key){
            return ItemValue{};
        }, std::placeholders::_1);
    }

 private:
    Item create_item(ItemId id) {
        if constexpr(with_value)
            return Item{id, data_cb_(id)};
        else
            return Item{id};
    }

 protected:
    list_t list_;
    lru_hash hash_;
    size_t   size_;
    data_cb_t data_cb_;
};

template <class Item, class KeyT = int>
class lru_cache_ext : public lru_cache<Item, KeyT> {
 public:
    using _base = lru_cache<Item, KeyT>;
    using _base::list_;
    using _base::hash_;
    using _base::size_;
    using typename _base::data_cb_t;
    using _base::contains;

    lru_cache_ext(size_t size, typename _base::data_cb_t data_cb = _base::default_data_cb())
        : lru_cache<Item, KeyT>(size, data_cb) {
    }

    bool push_front(KeyT key) {
        if (contains(key))
            return true;

        list_.push_front(data_cb_(key));
        hash_[key] = list_.begin();
        if (list_.size() > size_) {
            hash_.erase(list_.back().id);
            list_.pop_back();
        }
    }

    bool erase(KeyT key) {
        auto hit_it = hash_.find(key);
        if (hit_it == hash_.end()) {
            return false;
        }

        list_.erase(hit_it->second);
        hash_.erase(hit_it);

        return true;
    }
};

}  // namespace homework

#endif  // INCLUDE_LRU_CACHE_H_

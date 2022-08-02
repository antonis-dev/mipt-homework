//
// Created by antonis on 6/28/22.
//

#ifndef INCLUDE_LRU_CACHE_H_
#define INCLUDE_LRU_CACHE_H_

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
    using lru_const_iter = typename list_t::const_iterator;
    using data_cb_t = std::function<ItemValue(ItemId)>;

    lru_cache(size_t capacity, data_cb_t data_cb = default_data_cb()) : capacity_(capacity), data_cb_(data_cb) {
    }

    bool full() const {
        return list_.size() >= capacity_;
    }

    size_t size() const {
        return list_.size();
    }

    const Item & tail() const {
        return list_.back();
    }

    const Item & front() const {
        return list_.front();
    }

    bool contains(ItemId key) const {
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

    void push_front(ItemId key) {
        if (contains(key))
            return;

        list_.push_front(create_item(key));
        hash_[key] = list_.begin();
        if (list_.size() > capacity_) {
            hash_.erase(list_.back().id);
            list_.pop_back();
        }
    }

    bool erase(ItemId key) {
        auto hit_it = hash_.find(key);
        if (hit_it == hash_.end()) {
            return false;
        }

        list_.erase(hit_it->second);
        hash_.erase(hit_it);

        return true;
    }

    void erase_tail() {
        hash_.erase(list_.back().id);
        list_.pop_back();
    }

    static data_cb_t default_data_cb() {
        return [](ItemId key){
            return ItemValue{};
        };
    }

 protected:
    Item create_item(ItemId id) const {
        if constexpr(with_value)
            return Item{id, data_cb_(id)};
        else
            return Item{id};
    }

 protected:
    list_t list_;
    lru_hash hash_;
    size_t   capacity_;
    data_cb_t data_cb_;
};

}  // namespace homework

#endif  // INCLUDE_LRU_CACHE_H_

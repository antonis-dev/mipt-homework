//
// Created by antonis on 6/29/22.
//

#ifndef INCLUDE_PREDICTING_CACHE_H_
#define INCLUDE_PREDICTING_CACHE_H_

#include <vector>
#include <list>
#include <functional>
#include <unordered_map>


namespace homework {

template <class ItemId = int>
class predicting_cache {
 public:
    using Item = std::pair<ItemId, std::size_t>;
    using history_t = std::vector<ItemId>;
    using list_t = std::list<ItemId>;
    using hash_t = std::unordered_map<ItemId, typename list_t::iterator>;
    using iter_t = typename list_t::iterator;
    using const_iter_t = typename list_t::const_iterator;

    predicting_cache(size_t size, history_t history) : size_(size), history_(history), cur_ind_(0) {
    }

    bool full() const {
        return list_.size() >= size_;
    }

    const_iter_t tail() const {
        return list_.end();
    }

    const_iter_t front() const {
        return list_.begin();
    }

    bool contains(ItemId key) const {
        return hash_.count(key) > 0;
    }

    bool lookup_update(ItemId key) {
        auto hit_it = hash_.find(key);
        if (hit_it == hash_.end()) {
            if (full()) {
                remove_worthless();
            }

            list_.push_front(key);
            hash_[key] = list_.begin();

            return false;
        }

        return true;
    }

 private:
    /**
     * @brief - it never be called for empty cache, so cur_ind_ will be at lease cap_
     */
    void remove_worthless() {
        ItemId latest = *list_.begin();
        for (std::size_t ind = cur_ind_; ind < history_.size(); ind++) {
            auto id = history_[ind];
            if (hash_.count(id)) {
                latest = id;
            }
        }

        list_.erase(hash_[latest]);
        hash_.erase(latest);
    }

 protected:
    list_t    list_;
    hash_t    hash_;
    size_t    size_;
    size_t    cur_ind_;
    history_t history_;
};

}  // namespace homework

#endif  // INCLUDE_PREDICTING_CACHE_H_

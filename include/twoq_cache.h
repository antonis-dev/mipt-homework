//
// Created by antonis on 6/24/22.
//

#ifndef INCLUDE_TWOQ_CACHE_H_
#define INCLUDE_TWOQ_CACHE_H_

#include "lru_cache.h"

namespace homework {

constexpr size_t MIN_CACHE_CAPACITY = 2;
constexpr size_t MIN_QUEUE_CAPACITY = 1;
constexpr double A1_IN_SHARE = 0.25;
constexpr double A1_OUT_SHARE = 0.5;

template <class ItemValue, class ItemId>
class twoq_cache {
 public:
    twoq_cache(size_t capacity)
        : cap_(capacity >= MIN_CACHE_CAPACITY ? capacity : MIN_CACHE_CAPACITY)
        , a1_in_cap_(cap_ * A1_IN_SHARE > 1 ? cap_ * A1_IN_SHARE : MIN_QUEUE_CAPACITY)
        , a1_out_cap_(cap_ * A1_OUT_SHARE)
        , am_cap_(cap_ - a1_in_cap_ > 0 ? cap_ - a1_in_cap_ : MIN_QUEUE_CAPACITY)
        , a1_in_(cap_)
        , a1_out_(a1_out_cap_)
        , am_(am_cap_) {
    }

    bool lookup_update(ItemId key) {
        if (am_.contains(key)) {
            return am_.lookup_update(key);
        } else if (a1_out_.contains(key)) {
            a1_out_.erase(key);
            reclaim_slot();
            return am_.lookup_update(key);
        } else {
            if (a1_in_.contains(key)) {
                return true;
            }

            reclaim_slot();
            a1_in_.push_front(key);

            return false;
        }
    }

    size_t size() const { return a1_in_.size() + am_.size(); }

 private:
    void reclaim_slot() {
        if (a1_in_.size() + am_.size() < cap_) {
            return;
        }

        if (a1_in_.size() > a1_in_cap_) {
            auto & tail_id = a1_in_.tail();
            a1_out_.push_front(tail_id.id);

            a1_in_.erase_tail();
            return;
        }

        am_.erase_tail();
    }

 private:
    size_t cap_;
    size_t a1_in_cap_;
    size_t a1_out_cap_;
    size_t am_cap_;

    lru_cache_ext<ItemValue, ItemId> a1_in_;
    lru_cache_ext<ItemValue, ItemId, false> a1_out_;
    lru_cache_ext<ItemValue, ItemId> am_;
};

}  // namespace homework

#endif  // INCLUDE_TWOQ_CACHE_H_

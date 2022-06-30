//
// Created by antonis on 6/24/22.
//

#include "twoq_cache.h"
#include "predicting_cache.h"
#include <iostream>

using homework::lru_cache;
using homework::lru_cache_ext;
using homework::predicting_cache;

int main() {
    std::vector<size_t> history {1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7};

    lru_cache<int, size_t> lru(4);
    predicting_cache<size_t> predicting(4, history);

    for (auto id : history) {
        std::cout << lru.lookup_update(id) << " ";
    }
    std::cout << std::endl;

    for (auto id : history) {
        std::cout << predicting.lookup_update(id) << " ";
    }
    std::cout << std::endl;
}
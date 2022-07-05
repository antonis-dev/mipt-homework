//
// Created by antonis on 6/24/22.
//

#include <iostream>
#include "twoq_cache.h"

using homework::twoq_cache;

std::vector<size_t>  in_ids;
size_t               cache_size;

void load_data() {
    size_t in_size;
    std::cin >> cache_size >> in_size;

    in_ids.resize(in_size);
    for (size_t ind = 0; ind < in_size; ind++)
        std::cin >> in_ids[ind];
}

void run_test() {
    load_data();

    twoq_cache<int, size_t> twoq(cache_size);
    size_t twoq_hit_count = 0;
    for (auto id : in_ids) {
        twoq.lookup_update(id) ? twoq_hit_count++ : twoq_hit_count;
    }

    std::cout << twoq_hit_count << std::endl;
}

int main() {
    run_test();

    return 0;
}
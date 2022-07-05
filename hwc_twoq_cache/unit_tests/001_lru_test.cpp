#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING

#include "catch.hpp"
#include "lru_cache.h"

using homework::lru_cache;

constexpr int ID_MULT = 4528;
size_t get_data_func(size_t id) {
    return id * ID_MULT;
}

SCENARIO("LRU cache test") {
    GIVEN("An empty lru cache") {
         constexpr size_t CACHE_SIZE = 10;
         lru_cache<size_t, size_t> lru_cache(CACHE_SIZE, std::bind(&get_data_func, std::placeholders::_1));

         THEN("The size start at 0 and fullness is false") {
             REQUIRE(lru_cache.size() == 0);
             REQUIRE(lru_cache.full() == false);
         }

         WHEN("new item send to cache") {
             size_t new_key = 6;
             auto search_res = lru_cache.lookup_update(new_key);
             THEN("The size changes and cache has new item") {
                 REQUIRE(lru_cache.size() == 1);
                 REQUIRE(lru_cache.front().id == new_key);
                 REQUIRE(lru_cache.tail().id == new_key);
                 REQUIRE(lru_cache.tail().value == new_key * ID_MULT);
                 REQUIRE(search_res == false);
             }
         }

         WHEN("size of lru_cache different elements send") {
             size_t hits = 0;
             for (size_t id = 0; id < CACHE_SIZE; id++) {
                 auto search_res = lru_cache.lookup_update(id);
                 if (search_res) {
                     hits = 0;
                 }
             }

            THEN("cache is full") {
                REQUIRE(lru_cache.size() == CACHE_SIZE);
                REQUIRE(lru_cache.full());
            }

            THEN("no hits") {
                REQUIRE(hits == 0);
            }

            THEN("same keys always hit") {
                for (size_t id = 0; id < CACHE_SIZE; id++) {
                    REQUIRE(lru_cache.lookup_update(id));
                    REQUIRE(lru_cache.front().id == id);
                    REQUIRE(lru_cache.front().value == id * ID_MULT);
                }
            }

            THEN("new element pushes oldest out") {
                REQUIRE(lru_cache.lookup_update(CACHE_SIZE) == false);
                REQUIRE(lru_cache.lookup_update(0) == false);
             }
         }
    }
}

TEST_CASE("Channel watch test") {
    REQUIRE(true);
}

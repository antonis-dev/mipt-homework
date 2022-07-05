


//
// Created by antonis on 6/24/22.
//

#include <iostream>
#include <fstream>
#include <filesystem>
#include <set>
#include "twoq_cache.h"
#include "predicting_cache.h"

using homework::lru_cache;
using homework::lru_cache_ext;
using homework::twoq_cache;
using homework::predicting_cache;
using String = std::string;
namespace fs = std::filesystem;

const String DATA_PATH = "test_data/";
const String IN_EXT = ".dat";
const String ANS_EXT = ".ans";
const String PERF_EXT = ".perf";
const size_t IDEAL_CACHE_LIMIT = 1000;

std::vector<size_t>  in_ids;
size_t               cache_size;
size_t               ans;

bool load_test_case(const String & test_name) {
    std::ifstream in_file(DATA_PATH + test_name + IN_EXT);
    if (!in_file)
        return false;

    size_t in_size;
    in_file >> cache_size >> in_size;

    in_ids.resize(in_size);
    for (size_t ind = 0; ind < in_size; ind++)
        in_file >> in_ids[ind];

    std::ifstream ans_file(DATA_PATH + test_name + ANS_EXT);
    if (!ans_file)
        return false;

    ans_file >> ans;

    return true;
}

bool load_perf_test(const String & test_name) {
    std::ifstream in_file(DATA_PATH + test_name + PERF_EXT);
    if (!in_file)
        return false;

    size_t in_size;
    in_file >> cache_size >> in_size;

    in_ids.resize(in_size);
    for (size_t ind = 0; ind < in_size; ind++)
        in_file >> in_ids[ind];

    return true;
}

String percetage_diffs(size_t base_hits, size_t other_hits) {
    if (base_hits == 0) {
        return "base cache has no hits";
    } else if (other_hits == 0) {
        return "no hits";
    } else if (other_hits >= base_hits) {
        auto res = static_cast<size_t>((static_cast<double>(other_hits) / base_hits - 1) * 100);
        return "+" + std::to_string(res) + String("%");
    } else {
        auto res = static_cast<size_t>((static_cast<double>(base_hits) / other_hits - 1) * 100);
        return "-" + std::to_string(res) + String("%");
    }
}

bool run_test(const String & test_name) {
    if (!load_test_case(test_name))
        return false;

    twoq_cache<int, size_t> twoq(cache_size);


    size_t twoq_hit_count = 0;
    for (auto id : in_ids) {
        auto success = twoq.lookup_update(id);
        success ? twoq_hit_count++ : twoq_hit_count;
        // std::cout << success << " ";
    }

    bool success = twoq_hit_count == ans;
    std::cout << "test case \"" << test_name << "\": " << (success ? "SUCCESS" : "FAIL") << std::endl;
    if (!success) {
        std::cout << "hits: " << twoq_hit_count << " answer: " << ans << std::endl;
    }
    return success;
}

bool performance_test(const String & test_name) {
    if (!load_perf_test(test_name))
        return false;

    std::cout << "performance test case \"" << test_name << "\": " << std::endl;

    size_t lru_hit_count = 0;
    lru_cache<int, size_t> lru(cache_size);
    for (auto id : in_ids) {
        auto success = lru.lookup_update(id);
        success ? lru_hit_count++ : lru_hit_count;
    }

    size_t predict_hit_count = 0;
    if (cache_size < IDEAL_CACHE_LIMIT) {
        predict_hit_count = 0;
        predicting_cache<size_t> predicting(cache_size, in_ids);
        for (auto id : in_ids) {
            auto success = predicting.lookup_update(id);
            success ? predict_hit_count++ : predict_hit_count;
        }
    }

    size_t twoq_hit_count = 0;
    twoq_cache<int, size_t> twoq(cache_size);
    for (auto id : in_ids) {
        auto success = twoq.lookup_update(id);
        success ? twoq_hit_count++ : twoq_hit_count;
    }

    std::cout << "\tLRU cache hits: " << lru_hit_count << "\t\t 100% " << std::endl;
    if (cache_size < IDEAL_CACHE_LIMIT)
        std::cout << "\tPredicting cache hits: " << predict_hit_count << "\t " <<
                  percetage_diffs(lru_hit_count, predict_hit_count) << std::endl;
    std::cout << "\t2Q cache hits: " << twoq_hit_count << "\t\t " <<
              percetage_diffs(lru_hit_count, twoq_hit_count) << std::endl;
    std::cout << std::endl;

    return true;
}

int main() {
    std::set<String> in_test_cases;
    std::set<String> ans_test_cases;
    std::set<String> perf_test_cases;

    std::string path("test_data");
    for (auto &file : fs::recursive_directory_iterator(path)) {
        if (file.path().extension() == ANS_EXT) {
            ans_test_cases.insert(file.path().stem());
        } else if (file.path().extension() == IN_EXT) {
            in_test_cases.insert(file.path().stem());
        } else if (file.path().extension() == PERF_EXT) {
            perf_test_cases.insert(file.path().stem());
        }
    }

    for (auto test_case : in_test_cases) {
        if (ans_test_cases.count(test_case) > 0) {
            if (!run_test(test_case)) {
                return -1;
            }
        }
    }

    std::cout << std::endl;

    for (auto test_case : perf_test_cases) {
        performance_test(test_case);
    }

    return 0;
}

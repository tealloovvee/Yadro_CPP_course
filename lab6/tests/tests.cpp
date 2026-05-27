#include <fstream>
#include <random>
#include <string>
#include <thread>

#include <gtest/gtest.h>

#include "BufferingWriter.h"
#include "Record.h"

namespace {
const std::string tempFile = "data.txt";
constexpr size_t N_THREADS = 8;
constexpr size_t N_NUMBERS = 32768;
} // namespace

TEST(RPNTest, Testing) {
    std::vector<size_t> sum(N_THREADS, 0);
    {
        BufferingWriter writer(tempFile);
        auto worker = [&writer](size_t seed, size_t& sum) {
            std::mt19937 gen(seed);
            std::uniform_int_distribution<> dis(1, 100);
            for (size_t i = 0; i < N_NUMBERS; ++i) {
                const size_t newVal = dis(gen);
                sum += newVal;
                const Record rec(std::to_string(newVal));
                writer.Write(rec);
            }
        };
        std::vector<std::thread> workers;
        workers.reserve(N_THREADS);
        for (size_t i = 0; i < N_THREADS; ++i) {
            workers.emplace_back(worker, i, std::ref(sum[i]));
        }
        for (size_t i = 0; i < N_THREADS; ++i) {
            workers[i].join();
        }
    }
    size_t counter = 0;
    size_t totalSum0 = 0;
    std::ifstream fin(tempFile);
    size_t n;
    while (fin >> n) {
        ++counter;
        totalSum0 += n;
    }
    size_t totalSum1 = 0;
    for (size_t i = 0; i < N_THREADS; ++i) {
        totalSum1 += sum[i];
    }
    EXPECT_EQ(counter, N_THREADS * N_NUMBERS);
    EXPECT_EQ(totalSum0, totalSum1);
    std::ignore = std::remove(tempFile.c_str());
}

#pragma once

#include <iostream>
#include <chrono>
#include <utility>

namespace perftest{

template <typename ...Args>
double benchmark(const auto test_func, Args&& ...args){
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    test_func(args...);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> buffer = t2 - t1;
    return buffer.count();
}

}
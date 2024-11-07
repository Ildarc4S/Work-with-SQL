#include "Timer.hpp"

Timer::Timer() : _start{std::chrono::high_resolution_clock::now()} {}

void Timer::reset() { _start = std::chrono::high_resolution_clock::now(); }

long long Timer::getMicroseconds() const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(end - _start).count();
}

double Timer::getSeconds() const {
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::duration<double>>(end - _start).count();
}
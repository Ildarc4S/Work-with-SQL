#pragma once

#include <chrono>

class Timer {
   public:
    Timer();

    void reset();
    long long getMicroseconds() const;
    double getSeconds() const;

   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> _start;
};
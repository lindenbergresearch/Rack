#pragma once

#include <chrono>

#define MIN_TIME 0.0
#define MAX_TIME 10e6

using namespace std::chrono;

namespace rack {
namespace timer {


/**
 * @brief Simple stopwatch for timeing statistics.
 */
struct Stopwatch {
    high_resolution_clock::time_point t0;
    high_resolution_clock::time_point t1, t2;
    unsigned int timerDivider;
    double sampleTime;
    double tau = 2.0;
    double elapsed = 0;


    /**
     * @brief Check for valid time value.
     * @param x
     * @return
     */
    static double check(double x) {
        if (isnan(x) || isinf(x)) return 0;
        if (x < MIN_TIME || x > MAX_TIME) return 0;
        return x; // fine
    }


    /**
     * @brief Constructs a stopwatch.
     * @param sampleTime A full time period.
     * @param timerDivider
     */
    explicit Stopwatch(double sampleTime, unsigned int timerDivider = 7) : timerDivider(timerDivider), sampleTime(sampleTime) {
        t0 = high_resolution_clock::now();
    }


    /**
     * @brief Start / Reset the stopwatch.
     * @return
     */
    double start() {
        t1 = high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = t1 - t0;
        return elapsed_seconds.count();
    }


    /**
     * @brief Returns the current value of the stopwatch with no side-effect.
     * @return
     */
    double current() const {
        std::chrono::duration<double> elapsed_seconds = high_resolution_clock::now() - t1;
        return elapsed_seconds.count();
    }


    /**
     * @brief Return the current lap-time and setup a smoothed elapsed time for statistics.
     * @return
     */
    double laptime() {
        t2 = high_resolution_clock::now();
        std::chrono::duration<double> elapsed_seconds = t2 - t1;
        auto _elapsed = elapsed_seconds.count();

        elapsed += (_elapsed - elapsed) * timerDivider * sampleTime / tau;

        return elapsed;
    }


    /**
     * @brief Returns the last lap-time as percentage based on a time period.
     * @return
     */
    double percent() const {
        return 100. * elapsed / sampleTime;
    }
};

}
}

#pragma once

#include <chrono>

using namespace std::chrono;


namespace rack {
namespace timer {

/**
 * @brief High resolution timer.
 */
static high_resolution_clock::time_point t0 = high_resolution_clock::now();


/**
 * @brief Returns the time since start in nanos.
 * @return
 */
static long nanoTime() {
    return duration_cast<nanoseconds>(high_resolution_clock::now() - t0).count();
}


/**
 * @brief
 * @param start
 * @return
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
        if (x < 0.0 || x > 10e3) return 0;
        return x; // fine
    }


    /**
     * @brief Constructs a stopwatch.
     * @param sampleTime A full time period.
     * @param timerDivider
     */
    explicit Stopwatch(double sampleTime = 0, unsigned int timerDivider = 7) : timerDivider(timerDivider), sampleTime(sampleTime) {
        t0 = high_resolution_clock::now();
        TRACE("Setup stopwatch-timer: divider=%d sampleTime=%.4f tau=%.3f", timerDivider, sampleTime, tau);
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
        return check(elapsed_seconds.count());
    }


    double currentLaptime() const {
        std::chrono::duration<double> elapsed_seconds = high_resolution_clock::now() - t1;
        auto _elapsed = elapsed_seconds.count();

        return check(elapsed + ((_elapsed - elapsed) * timerDivider * sampleTime / tau));
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

        elapsed = check(elapsed);

        return elapsed;
    }


    /**
     * @brief Returns the last lap-time as percentage based on a time period.
     * @return
     */
    double percent() const {
        return check(100. * elapsed / sampleTime);
    }
};

}
}

#ifndef INTERVAL_H
#define INTERVAL_H

#include <math.h>
#include <stdbool.h>

#define INTERVAL_EMPTY (Interval) {INFINITY, -INFINITY}
#define INTERVAL_UNIVERSE (Interval) {-INFINITY, INFINITY}
#define INTERVAL(min, max) (Interval) {min, max}

typedef struct {
    double min, max;
} Interval;

static inline Interval interval() {
    return (Interval) {
        .min = -INFINITY,
        .max = INFINITY,
    };
}

static inline Interval interval_with(double min, double max) {
    return (Interval) {
        .min = min,
        .max = max,
    };
}

static inline bool interval_contains(const Interval* interval, double x) {
    return interval->min <= x
        && interval->max >= x;
}

static inline bool interval_surrounds(const Interval* interval, double x) {
    return interval->min < x
        && interval->max > x;
}

#endif

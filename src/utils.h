#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#include "constants.h"

static inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

static inline double random_double_in(double min, double max) {
    return min + (max - min) * random_double();
}

static inline double deg_to_rad(double deg) {
    return deg * PI / 180.0;
}

#endif

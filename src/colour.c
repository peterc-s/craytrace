#include "colour.h"
#include "interval.h"

#include <stdint.h>

static inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0.0) {
        return sqrt(linear_component);
    }

    return 0;
}

void write_colour(FILE* fd, const Colour* pixel_colour) {
    static const Interval intensity = {
        .min = 0.000,
        .max = 0.999,
    };

    double r = linear_to_gamma(pixel_colour->e[X]);
    double g = linear_to_gamma(pixel_colour->e[Y]);
    double b = linear_to_gamma(pixel_colour->e[Z]);

    uint8_t rbyte = (uint8_t)(256 * interval_clamp(&intensity, r));
    uint8_t gbyte = (uint8_t)(256 * interval_clamp(&intensity, g));
    uint8_t bbyte = (uint8_t)(256 * interval_clamp(&intensity, b));

    fprintf(fd, "%d %d %d\n", rbyte, gbyte, bbyte);
}

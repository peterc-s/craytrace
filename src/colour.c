#include "colour.h"
#include "interval.h"

#include <stdint.h>

void write_colour(FILE* fd, const Colour* pixel_colour) {
    static const Interval intensity = {
        .min = 0.000,
        .max = 0.999,
    };

    uint8_t rbyte = (uint8_t)(256 * interval_clamp(&intensity, pixel_colour->e[X]));
    uint8_t gbyte = (uint8_t)(256 * interval_clamp(&intensity, pixel_colour->e[Y]));
    uint8_t bbyte = (uint8_t)(256 * interval_clamp(&intensity, pixel_colour->e[Z]));

    fprintf(fd, "%d %d %d\n", rbyte, gbyte, bbyte);
}

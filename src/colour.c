#include "colour.h"

#include <stdint.h>

void write_colour(FILE* fd, const Colour* pixel_colour) {
    uint8_t rbyte = (uint8_t)(255.999 * pixel_colour->e[X]);
    uint8_t gbyte = (uint8_t)(255.999 * pixel_colour->e[Y]);
    uint8_t bbyte = (uint8_t)(255.999 * pixel_colour->e[Z]);

    fprintf(fd, "%d %d %d\n", rbyte, gbyte, bbyte);
}

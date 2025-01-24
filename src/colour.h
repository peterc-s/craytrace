#ifndef COLOUR_H
#define COLOUR_H

#include <stdio.h>

#include "vec3.h"

typedef Vec3 Colour;

void write_colour(FILE* fd, const Colour* pixel_colour);

#endif

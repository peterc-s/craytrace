#ifndef CAMERA_H
#define CAMERA_H

#include <stdint.h>

#include "constants.h"
#include "vec3.h"
#include "ray.h"
#include "sphere_list.h"
#include "colour.h"

typedef struct {
    Point3 centre;
    Point3 pixel00_loc;
    Vec3 pixel_delta_u;
    Vec3 pixel_delta_v;
    double aspect_ratio;
    uint16_t image_width;
    uint16_t image_height;
} Camera;

Result camera_init(Camera* camera, double aspect_ratio, uint16_t image_width);
void camera_render(Camera* camera, const SphereList* spheres);

#endif

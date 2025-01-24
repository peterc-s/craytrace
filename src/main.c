#include <stdio.h>
#include <stdint.h>

#include "constants.h"
#include "vec3.h"
#include "sphere_list.h"
#include "camera.h"

int main(void) {

    /// world
    // spheres
    SphereList spheres;
    sphere_list_new(&spheres);
    sphere_list_add(&spheres, vec3_with(0, -100.5, -1), 100);
    sphere_list_add(&spheres, vec3_with(0, 0, -1), 0.5);

    // camera
    Camera cam;
    camera_init(&cam, 16.0 / 9.0, 400);
    camera_render(&cam, &spheres);

    // free memory
    sphere_list_destroy(&spheres);
}

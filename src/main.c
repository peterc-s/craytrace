#include <stdlib.h>
#include <time.h>

#include "vec3.h"
#include "sphere_list.h"
#include "camera.h"

int main(void) {
    srand(time(NULL));

    /// world
    // spheres
    SphereList spheres;
    void* res = sphere_list_new(&spheres);
    if (res == NULL) exit(EXIT_FAILURE);

    Result result = sphere_list_add(&spheres, vec3_with(0, -100.5, -1), 100);
    if (result == ERROR) exit(EXIT_FAILURE);

    result = sphere_list_add(&spheres, vec3_with(0, 0, -1), 0.5);
    if (result == ERROR) exit(EXIT_FAILURE);

    // camera
    Camera cam;
    result = camera_init(&cam, 16.0 / 9.0, 400, 33, 6);
    if (result == ERROR) exit(EXIT_FAILURE);

    camera_render(&cam, &spheres);

    // free memory
    sphere_list_destroy(&spheres);
}

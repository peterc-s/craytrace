#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "colour.h"
#include "ray.h"
#include "vec3.h"

// 16 KiB IO buffer
#define IO_BUFSIZ 1024 * 16

double hit_sphere(const Point3* centre, double radius, const Ray* r) {
    // C - Q
    Vec3 oc = vec3_sub(centre, &r->orig);

    // a = d . d
    double a = vec3_dot(&r->dir, &r->dir);

    // b = -2d . (C - Q)
    double b = -2.0 * vec3_dot(&r->dir, &oc);

    // c = (C - Q) . (C - Q) - r^2
    double c = vec3_dot(&oc, &oc) - radius * radius;

    // \Delta = b^2 + 4ac
    double discriminant = b * b - 4 * a * c;

    // if geq 0 then at least one real root
    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-b - sqrt(discriminant)) / (2.0 * a);
    }
}

Colour ray_colour(const Ray* r) {
    Vec3 sphere_centre = vec3_with(0, 0, -1);
    double t = hit_sphere(&sphere_centre, 0.5, r);
    if (t > 0) {
        // ray at the t where it hit the sphere
        Vec3 r_at_t = ray_at(r, t);

        // diff between that and sphere centre
        Vec3 r_at_t_diff_centre = vec3_sub(&r_at_t, &sphere_centre);
        
        // hit normal vector
        Vec3 N = vec3_unit(&r_at_t_diff_centre);

        return vec3_mult(VEC3(N.e[X] + 1, N.e[Y] + 1, N.e[Z] + 1), 0.5);
    }

    Vec3 unit_direction = vec3_unit(&r->dir);

    // a
    double a = 0.5 * (unit_direction.e[Y] + 1.0);

    // (1.0 - a) * white
    Colour white_blend = vec3_mult(VEC3(1.0, 1.0, 1.0), 1.0 - a);

    // a * blue
    Colour blue_blend = vec3_mult(VEC3(0.5, 0.7, 1.0), a);

    // (1.0-a) * white + a * blue
    return vec3_add(&white_blend, &blue_blend);
}

int main(void) {
    // set up stdout buffering
    char stdout_buf[IO_BUFSIZ];
    setvbuf(stdout, stdout_buf, _IOFBF, IO_BUFSIZ);

    // image
    double aspect_ratio = 16.0 / 9.0;
    uint16_t image_width = 400;
    double image_height_f = image_width / aspect_ratio;
    uint16_t image_height = (image_height_f < 1.0) ? 1 : (uint16_t)image_height_f;

    // camera
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)image_width / image_height);
    Point3 camera_center = vec3_with(0, 0, 0);

    Vec3 viewport_u = vec3_with(viewport_width, 0, 0);
    Vec3 viewport_v = vec3_with(0, -viewport_height, 0);

    Vec3 pixel_delta_u = vec3_div(&viewport_u, image_width);
    Vec3 pixel_delta_v = vec3_div(&viewport_v, image_height);

    // camera_center - [0, 0, focal_length] - viewport_u/2 - viewport_v/2
    Vec3 viewport_upper_left;
    {
        // [0, 0, focal_length]
        Vec3 focal = vec3_with(0, 0, focal_length);

        // camera_center - [0, 0, focal_length]
        Vec3 center_focal_diff = vec3_sub(&camera_center, &focal);

        // viewport_u/2
        Vec3 viewport_u_half = vec3_div(&viewport_u, 2);

        // viewport_v/2
        Vec3 viewport_v_half = vec3_div(&viewport_v, 2);

        // camera_center - [0, 0, focal_length] - viewport_u/2
        Vec3 center_focal_u_half_diff = vec3_sub(&center_focal_diff, &viewport_u_half);

        // camera_center - [0, 0, focal_length] - viewport_u/2 - viewport_v/2
        viewport_upper_left = vec3_sub(&center_focal_u_half_diff, &viewport_v_half);
    }

    // viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)
    Vec3 pixel00_loc;
    {
        // (pixel_delta_u + pixel_delta_v)
        Vec3 pixel_delta_uv = vec3_add(&pixel_delta_u, &pixel_delta_v);

        // 0.5 * (pixel_delta_u + pixel_delta_v)
        Vec3 half_pixel_delta_uv = vec3_mult(&pixel_delta_uv, 0.5);

        // viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)
        pixel00_loc = vec3_add(&viewport_upper_left, &half_pixel_delta_uv);
    }

    printf("P3\n%d %d\n255\n", image_width, image_height);

    for (uint16_t i = 0; i < image_height; ++i) {
        fprintf(stderr, "\rLines done: %d / %d", i + 1, image_height);
        for (uint16_t j = 0; j < image_width; ++j) {
            // pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v)
            Vec3 pixel_center;
            {
                // (j * pixel_delta_u)
                Vec3 j_delta_u = vec3_mult(&pixel_delta_u, j);

                // (i * pixel_delta_v)
                Vec3 i_delta_v = vec3_mult(&pixel_delta_v, i);

                // (j * pixel_delta_u) + (i * pixel_delta_v)
                Vec3 ij_delta_vu = vec3_add(&j_delta_u, &i_delta_v);

                // pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v)
                pixel_center = vec3_add(&pixel00_loc, &ij_delta_vu);
            }

            Vec3 ray_direction = vec3_sub(&pixel_center, &camera_center);
            Ray r = ray_with(&camera_center, &ray_direction);

            Colour pixel_colour = ray_colour(&r);
            write_colour(stdout, &pixel_colour);
        }
    }

    fprintf(stderr, "\nDone.\n");
    fflush(stdout);
}

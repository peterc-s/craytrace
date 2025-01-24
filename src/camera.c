#include "camera.h"
#include "constants.h"
#include "ray.h"
#include "vec3.h"

static Colour ray_colour(const Ray* r, const SphereList* sphere_list) {
    HitRecord rec = { 0 };
    if (sphere_list_hit(sphere_list, r, INTERVAL(0, INFINITY), &rec)) {
        Vec3 normal_colour = vec3_add(&rec.normal, &VEC3(1, 1, 1));
        vec3_mult_assign(&normal_colour, 0.5);
        return normal_colour;
    }

    Vec3 unit_direction = vec3_unit(&r->dir);

    // a
    double a = 0.5 * (unit_direction.e[Y] + 1.0);

    // (1.0 - a) * white
    Colour white_blend = vec3_mult(&VEC3(1.0, 1.0, 1.0), 1.0 - a);

    // a * blue
    Colour blue_blend = vec3_mult(&VEC3(0.5, 0.7, 1.0), a);

    // (1.0-a) * white + a * blue
    return vec3_add(&white_blend, &blue_blend);
}

Result camera_init(Camera* camera, double aspect_ratio, uint16_t image_width, uint16_t sample_grid_size) {
    camera->aspect_ratio = aspect_ratio;
    camera->image_width = image_width;
    camera->sample_grid_size = sample_grid_size;

    if (camera->aspect_ratio <= 0 || camera->image_width <= 0 || camera->sample_grid_size == 0) {
        fprintf(stderr, "Failed to initialise camera. Invalid aspect ratio, image width, or sample grid size.\n");
        return ERROR;
    }

    double image_height_f = camera->image_width / camera->aspect_ratio;
    camera->image_height = (image_height_f < 1.0) ? 1 : (uint16_t)image_height_f;

    // for normalising pixel samples later
    camera->pixel_samples_scale = 1.0 / (camera->sample_grid_size * camera->sample_grid_size);

    // viewport
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * ((double)camera->image_width / camera->image_height);
    camera->centre = VEC3(0, 0, 0);

    Vec3 viewport_u = vec3_with(viewport_width, 0, 0);
    Vec3 viewport_v = vec3_with(0, -viewport_height, 0);

    // deltas
    camera->pixel_delta_u = vec3_div(&viewport_u, camera->image_width);
    camera->pixel_delta_v = vec3_div(&viewport_v, camera->image_height);

    // camera_centre - [0, 0, focal_length] - viewport_u/2 - viewport_v/2
    Vec3 viewport_upper_left;
    {
        // [0, 0, focal_length]
        Vec3 focal = vec3_with(0, 0, focal_length);

        // camera_centre - [0, 0, focal_length]
        Vec3 center_focal_diff = vec3_sub(&camera->centre, &focal);

        // viewport_u/2
        Vec3 viewport_u_half = vec3_div(&viewport_u, 2);

        // viewport_v/2
        Vec3 viewport_v_half = vec3_div(&viewport_v, 2);

        // camera_centre - [0, 0, focal_length] - viewport_u/2
        Vec3 center_focal_u_half_diff = vec3_sub(&center_focal_diff, &viewport_u_half);

        // camera_centre - [0, 0, focal_length] - viewport_u/2 - viewport_v/2
        viewport_upper_left = vec3_sub(&center_focal_u_half_diff, &viewport_v_half);
    }

    // (pixel_delta_u + pixel_delta_v)
    Vec3 pixel_delta_uv = vec3_add(&camera->pixel_delta_u, &camera->pixel_delta_v);

    // 0.5 * (pixel_delta_u + pixel_delta_v)
    Vec3 half_pixel_delta_uv = vec3_mult(&pixel_delta_uv, 0.5);

    // viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v)
    camera->pixel00_loc = vec3_add(&viewport_upper_left, &half_pixel_delta_uv);

    return SUCCESS;
}

static inline Vec3 sample_grid(uint16_t grid_size, uint16_t sample_num) {
    // creates offsets [-0.5, 0.5] from a grid
    return VEC3( \
            0.5 - (double)(sample_num % grid_size) / (grid_size - 1), \
            0.5 - (uint16_t)(sample_num / grid_size) / (double)(grid_size - 1), \
            0);
}

Ray get_ray(const Camera* camera, uint16_t i, uint16_t j, uint16_t sample_num) {
    // initialise offset to 0
    Vec3 offset = vec3();

    // if antialiasing on, then get the sample offset
    if (camera->sample_grid_size != 1) {
        offset = sample_grid(camera->sample_grid_size, sample_num);
    }

    Point3 pixel_sample = camera->pixel00_loc;
    {
        // calculate offset from pixel00
        Point3 delta_u_j_offset = vec3_mult(&camera->pixel_delta_u, (j + offset.e[X]));
        Point3 delta_v_i_offset = vec3_mult(&camera->pixel_delta_v, (i + offset.e[Y]));
        vec3_add_assign(&pixel_sample, &delta_u_j_offset);
        vec3_add_assign(&pixel_sample, &delta_v_i_offset);
    }

    // get ray direction using pixel sample offset
    Vec3 ray_direction = vec3_sub(&pixel_sample, &camera->centre);

    return ray_with(&camera->centre, &ray_direction);
}

void camera_render(const Camera* camera, const SphereList* spheres) {
    // set up stdout buffering
    char stdout_buf[IO_BUFSIZ];
    setvbuf(stdout, stdout_buf, _IOFBF, IO_BUFSIZ);

    // header
    printf("P3\n%d %d\n255\n", camera->image_width, camera->image_height);

    for (uint16_t i = 0; i < camera->image_height; ++i) {
        fprintf(stderr, "\rLines done: %d / %d", i + 1, camera->image_height);
        for (uint16_t j = 0; j < camera->image_width; ++j) {
            // pixel starts with nothing
            Colour pixel_colour = vec3();

            // add samples to pixel
            for (uint16_t sample = 0; sample < camera->sample_grid_size * camera->sample_grid_size; ++sample) {
                Ray r = get_ray(camera, i, j, sample);
                Colour ray_col = ray_colour(&r, spheres);
                vec3_add_assign(&pixel_colour, &ray_col);
            }

            // average samples
            vec3_mult_assign(&pixel_colour, camera->pixel_samples_scale);
            write_colour(stdout, &pixel_colour);
        }
    }

    fprintf(stderr, "\nDone.\n");
    fflush(stdout);
}

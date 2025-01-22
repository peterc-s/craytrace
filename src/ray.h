#ifndef RAY_H
#define RAY_H

#include "vec3.h"

typedef struct {
    Point3 orig;
    Vec3 dir;
} Ray;

inline Ray ray() {
    return (Ray){ 0 };
}

inline Ray ray_with(const Point3* orig, const Vec3* dir) {
    return (Ray) {
        .orig = *orig,
        .dir = *dir,
    };
}

inline const Point3* ray_origin(const Ray* ray) {
    return &ray->orig;
}

inline const Vec3* ray_direction(const Ray* ray) {
    return &ray->dir;
}

inline Point3 at(const Ray* ray, double t) {
    Vec3 mult_dir = vec3_mult(&ray->dir, t);
    return vec3_add(&ray->orig, &mult_dir);
}

#endif

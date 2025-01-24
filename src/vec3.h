#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

#include "utils.h"

#define VEC3(e0, e1, e2) \
    ((Vec3) { .e = { (e0), (e1), (e2) } })

typedef enum {
    X,
    Y,
    Z,
} Vec3Index;

typedef struct {
    double e[3];
} Vec3;

typedef Vec3 Point3;

static inline Vec3 vec3() {
    return (Vec3) { .e = {0, 0, 0} };
}

static inline Vec3 vec3_with(double e0, double e1, double e2) {
    return (Vec3) { .e = {e0, e1, e2} };
}

static inline Vec3 vec3_random() {
    return VEC3(random_double(), random_double(), random_double());
}

static inline Vec3 vec3_random_in(double min, double max) {
    return VEC3(random_double_in(min, max), random_double_in(min, max), random_double_in(min, max));
}

static inline Vec3 vec3_neg(Vec3* v0) {
    return vec3_with(-v0->e[X], -v0->e[Y], -v0->e[Z]);
}

static inline void vec3_add_assign(Vec3* v0, const Vec3* v1) {
    v0->e[X] += v1->e[X];
    v0->e[Y] += v1->e[Y];
    v0->e[Z] += v1->e[Z];
}

static inline void vec3_mult_assign(Vec3* v0, double t) {
    v0->e[X] *= t;
    v0->e[Y] *= t;
    v0->e[Z] *= t;
}

static inline void vec3_div_assign(Vec3* v0, double t) {
    vec3_mult_assign(v0, 1.0/t);
}

static inline double vec3_length_squared(const Vec3* v0) {
    return v0->e[X] * v0->e[X]
         + v0->e[Y] * v0->e[Y]
         + v0->e[Z] * v0->e[Z]; 
}

static inline double vec3_length(const Vec3* v0) {
    return sqrt(vec3_length_squared(v0));
}

static inline void vec3_fprint(FILE* fd, const Vec3* v0) {
    fprintf(fd, "%lf %lf %lf", v0->e[X], v0->e[Y], v0->e[Z]);
}

static inline Vec3 vec3_add(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[X] + v1->e[X], \
                     v0->e[Y] + v1->e[Y], \
                     v0->e[Z] + v1->e[Z]);
}

static inline Vec3 vec3_sub(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[X] - v1->e[X], \
                     v0->e[Y] - v1->e[Y], \
                     v0->e[Z] - v1->e[Z]);
}

static inline Vec3 vec3_mult(const Vec3* v0, double t) {
    return vec3_with(v0->e[X] * t, \
                     v0->e[Y] * t, \
                     v0->e[Z] * t);
}

static inline Vec3 vec3_div(const Vec3* v0, double t) {
    return vec3_with((1.0/t) * v0->e[X], \
                     (1.0/t) * v0->e[Y], \
                     (1.0/t) * v0->e[Z]);
}

static inline double vec3_dot(const Vec3* v0, const Vec3* v1) {
    return v0->e[X] * v1->e[X]
         + v0->e[Y] * v1->e[Y]
         + v0->e[Z] * v1->e[Z];
}

static inline Vec3 vec3_cross(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[Y] * v1->e[Z] - v0->e[Z] * v1->e[Y], \
                     v0->e[Z] * v1->e[X] - v0->e[X] * v1->e[Z], \
                     v0->e[X] * v1->e[Y] - v0->e[Y] * v1->e[X]);
}

static inline Vec3 vec3_unit(const Vec3* v0) {
    return vec3_div(v0, vec3_length(v0));
}

static inline Vec3 vec3_random_unit() {
    while (true) {
        Vec3 p = vec3_random_in(-1.0, 1.0);
        double len_sq = vec3_length_squared(&p); 
        if (1e-160 < len_sq && len_sq <= 1) {
            return vec3_div(&p, sqrt(len_sq));
        }
    }
}

static inline Vec3 vec3_random_unit_hsphere(const Vec3* normal) {
    Vec3 on_unit_sphere = vec3_random_unit();
    if (vec3_dot(&on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return vec3_neg(&on_unit_sphere);
    }
}

#endif



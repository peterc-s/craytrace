#ifndef VEC3_H
#define VEC3_H

#include <math.h>
#include <stdio.h>

typedef enum {
    X,
    Y,
    Z,
} Vec3Index;

typedef struct {
    double e[3];
} Vec3;

typedef Vec3 Point3;

inline Vec3 vec3() {
    return (Vec3) { .e = {0, 0, 0} };
}

inline Vec3 vec3_with(double e0, double e1, double e2) {
    return (Vec3) { .e = {e0, e1, e2} };
}

inline Vec3 vec3_neg(Vec3* v0) {
    return vec3_with(-v0->e[X], -v0->e[Y], -v0->e[Z]);
}

inline void vec3_add_assign(Vec3* v0, const Vec3* v1) {
    v0->e[X] += v1->e[X];
    v0->e[Y] += v1->e[Y];
    v0->e[Z] += v1->e[Z];
}

inline void vec3_mult_assign(Vec3* v0, double t) {
    v0->e[X] *= t;
    v0->e[Y] *= t;
    v0->e[Z] *= t;
}

inline void vec3_div_assign(Vec3* v0, double t) {
    vec3_mult_assign(v0, 1.0/t);
}

inline double vec3_length_squared(const Vec3* v0) {
    return v0->e[X] * v0->e[X]
         + v0->e[Y] * v0->e[Y]
         + v0->e[Z] * v0->e[Z]; 
}

inline double vec3_length(const Vec3* v0) {
    return sqrt(vec3_length_squared(v0));
}

inline void vec3_fprint(FILE* fd, const Vec3* v0) {
    fprintf(fd, "%lf %lf %lf", v0->e[X], v0->e[Y], v0->e[Z]);
}

inline Vec3 vec3_add(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[X] + v1->e[X], \
                     v0->e[Y] + v1->e[Y], \
                     v0->e[Z] + v1->e[Z]);
}

inline Vec3 vec3_sub(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[X] - v1->e[X], \
                     v0->e[Y] - v1->e[Y], \
                     v0->e[Z] - v1->e[Z]);
}

inline Vec3 vec3_mult(const Vec3* v0, double t) {
    return vec3_with(v0->e[X] * t, \
                     v0->e[Y] * t, \
                     v0->e[Z] * t);
}

inline Vec3 vec3_div(const Vec3* v0, double t) {
    return vec3_with((1.0/t) * v0->e[X], \
                     (1.0/t) * v0->e[Y], \
                     (1.0/t) * v0->e[Z]);
}

inline double dot(const Vec3* v0, const Vec3* v1) {
    return v0->e[X] * v1->e[X]
         + v0->e[Y] * v1->e[Y]
         + v0->e[Z] * v1->e[Z];
}

inline Vec3 cross(const Vec3* v0, const Vec3* v1) {
    return vec3_with(v0->e[Y] * v1->e[Z] - v0->e[Z] * v1->e[Y], \
                     v0->e[Z] * v1->e[X] - v0->e[X] * v1->e[Z], \
                     v0->e[X] * v1->e[Y] - v0->e[Y] * v1->e[X]);
}

inline Vec3 vec3_unit(const Vec3* v0) {
    return vec3_div(v0, vec3_length(v0));
}

#endif



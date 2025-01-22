#ifndef HIT_H
#define HIT_H

#include "vec3.h"
#include "ray.h"

typedef struct {
    Point3 p;
    Vec3 normal;
    double t;
    bool front_face;
} HitRecord;

void hit_set_face_norm(HitRecord* rec, const Ray* r, const Vec3* outward_normal) {
    // NOTE: outward_normal assumed to have unit length.

    rec->front_face = vec3_dot(&r->dir, outward_normal) < 0;
    rec->normal = rec->front_face ? *outward_normal : vec3_neg(outward_normal); 
}

#endif

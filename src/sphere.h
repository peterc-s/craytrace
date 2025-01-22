#ifndef SPHERE_H
#define SPHERE_H

#include <stdbool.h>
#include <math.h>

#include "hit.h"
#include "vec3.h"
#include "ray.h"

#define SPHERE(centre, radius) \
    (&(Sphere) { .centre = *(centre), .radius = fmax(0, (radius)) })

typedef struct {
    const Point3 centre;
    double radius;
} Sphere;

static inline Sphere sphere(const Point3* centre, double radius) {
    return (Sphere) {
        .centre = *centre,
        .radius = fmax(0, radius),
    };
}

static inline bool sphere_hit(const Sphere* sphere, const Ray* r, double ray_tmin, double ray_tmax, HitRecord* rec) {
    Vec3 oc = vec3_sub(&sphere->centre, &r->orig);
    double a = vec3_length_squared(&r->dir);
    double h = vec3_dot(&r->dir, &oc);
    double c = vec3_length_squared(&oc) - sphere->radius * sphere->radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0) {
        return false;
    }

    double sqrt_d = sqrt(discriminant);

    double root = (h - sqrt_d) / a;
    if (root <= ray_tmin || ray_tmax <= root) {
        root = (h + sqrt_d / a);
        if (root <= ray_tmin || ray_tmax <= root) {
            return false;
        }
    }

    rec->t = root;
    rec->p = ray_at(r, rec->t);
    Vec3 outward_normal = vec3_sub(&rec->p, &sphere->centre);
    vec3_div_assign(&outward_normal, sphere->radius);
    hit_set_face_norm(rec, r, &outward_normal);

    return true;
}

#endif

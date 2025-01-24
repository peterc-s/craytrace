#include <stdlib.h>
#include <math.h>

#include "sphere_list.h"
#include "constants.h"

SphereList* sphere_list_new(SphereList* out) {
    SphereList temp = {
        .centres = (Point3*)malloc(SPHERE_LIST_DEFAULT_SIZE * sizeof(Point3)),
        .radii = (double*)malloc(SPHERE_LIST_DEFAULT_SIZE * sizeof(double)),
    };
    if (temp.centres == NULL || temp.radii == NULL) {
        fprintf(stderr, "Failed to malloc sphere list of size %zu.\n", SPHERE_LIST_DEFAULT_SIZE);
        return NULL;
    }

    *out = (SphereList) {
        .centres = temp.centres,
        .radii = temp.radii,
        .cap = SPHERE_LIST_DEFAULT_SIZE,
        .len = 0,
    };

    return out;
}

void sphere_list_destroy(SphereList* sphere_list) {
    free(sphere_list->radii);
    free(sphere_list->centres);
}

Result sphere_list_add(SphereList* sphere_list, Point3 centre, double radii) {
    // realloc if needed
    if (sphere_list->len == sphere_list->cap) {
        SphereList temp = {
            .centres = (Point3*)realloc(sphere_list->centres, sphere_list->cap * 2),
            .radii = (double*)realloc(sphere_list->radii, sphere_list->cap * 2),
        };
        if (temp.centres == NULL || temp.radii == NULL) {
            fprintf(stderr, "Failed to realloc sphere list with size %zu.\n", sphere_list->cap * 2);
            return ERROR;
        }

        sphere_list->cap *= 2;
        sphere_list->centres = temp.centres;
        sphere_list->radii = temp.radii;
    }

    // add new members and increase list size
    sphere_list->centres[sphere_list->len] = centre;
    sphere_list->radii[sphere_list->len] = radii;
    sphere_list->len++;
    return SUCCESS;
}

bool sphere_hit(Point3* centre, double radius, const Ray* r, double ray_tmin, double ray_tmax, HitRecord* rec) {
    Vec3 oc = vec3_sub(centre, &r->orig);
    double a = vec3_length_squared(&r->dir);
    double h = vec3_dot(&r->dir, &oc);
    double c = vec3_length_squared(&oc) - radius * radius;

    // calculate discriminant of quadratic from sphere intersection
    // equation
    double discriminant = h * h - a * c;

    // if less than zero, no real roots so no collision
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
    Vec3 outward_normal = vec3_sub(&rec->p, centre);
    vec3_div_assign(&outward_normal, radius);
    hit_set_face_norm(rec, r, &outward_normal);

    return true;
}

bool sphere_list_hit(SphereList* sphere_list, const Ray* r, double ray_tmin, double ray_tmax, HitRecord* rec) {
    HitRecord temp_rec;
    bool hit_anything = false;
    double closest_so_far = ray_tmax;

    for (size_t i = 0; i < sphere_list->len; ++i) {
        if (sphere_hit(sphere_list->centres + i, sphere_list->radii[i], r, ray_tmin, closest_so_far, &temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            *rec = temp_rec;
        }
    }

    return hit_anything;
}

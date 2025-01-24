#ifndef SPHERE_LIST_H
#define SPHERE_LIST_H

#include <stdbool.h>

#include "hit.h"
#include "vec3.h"
#include "ray.h"
#include "constants.h"

typedef struct {
    Point3* centres;
    double* radii;
    size_t cap;
    size_t len;
} SphereList;

SphereList* sphere_list_new(SphereList* out);
void sphere_list_destroy(SphereList* sphere_list);
Result sphere_list_add(SphereList* sphere_list, Point3 centre, double radii);

bool sphere_hit(Point3* centre, double radius, const Ray* r, double ray_tmin, double ray_tmax, HitRecord* rec);
bool sphere_list_hit(SphereList* sphere_list, const Ray* r, double ray_tmin, double ray_tmax, HitRecord* rec);

#endif

#ifndef HITTABLE_H
#define HITTABLE_H

#include "rtweekend.h"
#include "ray.h"
// 3.6. Constructing Bounding Boxes for Hittables
#include "aabb.h"

class material;

struct hit_record 
{
    point3 p;
    vec3 normal;
    shared_ptr<material> mat_ptr;
    double t;
    bool front_face;

    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal :-outward_normal;
    }
};

class hittable 
{
    public:
        virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
        // 3.6. Constructing Bounding Boxes for Hittables
        // We now need to add a function to compute the bounding boxes of all the hittables. 
        // Then we will make a hierarchy of boxes over all the primitives, 
        // and the individual primitives—like spheres—will live at the leaves. 
        // That function returns a bool because not all primitives have bounding boxes (e.g., infinite planes). 
        // In addition, moving objects will have a bounding box 
        // that encloses the object for the entire time interval [time1,time2].
        virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};

#endif

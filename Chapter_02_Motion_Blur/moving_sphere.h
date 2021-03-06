// 2.3. Adding Moving Spheres
// A sphere class that has its center move linearly from center0 at time0 to center1 at time1. 
// Outside that time interval it continues on, 
// so those times need not match up with the camera aperture open and close.
#ifndef MOVING_SPHERE_H
#define MOVING_SPHERE_H

#include "rtweekend.h"

#include "hittable.h"


class moving_sphere : public hittable 
{
    public:
        moving_sphere() {}
        moving_sphere(
            point3 cen0, point3 cen1, double t0, double t1, double r, shared_ptr<material> m
        ) : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), mat_ptr(m) {};

        virtual bool hit(
            const ray& r, double t_min, double t_max, hit_record& rec) const override;

        point3 center(double time) const;

    public:
        point3 center0, center1;
        double time0, time1;
        double radius;
        shared_ptr<material> mat_ptr;
};

// 2.3. Adding Moving Spheres
// The moving_sphere::hit() function is almost identical to the sphere::hit() function: 
// center just needs to become a function center(time):
bool moving_sphere::hit(const ray &r, double t_min, double t_max, hit_record &rec) const 
{
    // 2.3. Adding Moving Spheres
    vec3 oc = r.origin() - center(r.time());
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = half_b*half_b - a*c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            // 2.3. Adding Moving Spheres
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.at(rec.t);
            // 2.3. Adding Moving Spheres
            auto outward_normal = (rec.p - center(r.time())) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

point3 moving_sphere::center(double time) const 
{
    return center0 + ((time - time0) / (time1 - time0))*(center1 - center0);
}

#endif

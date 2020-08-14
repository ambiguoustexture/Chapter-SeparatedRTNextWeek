// 2.4. Tracking the Time of Ray Intersection
// Be sure that in the materials you have the scattered rays 
// be at the time of the incident ray.
#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"
#include "hittable.h"
// 4.2. Texture Coordinates for Spheres
#include "texture.h"

struct hit_record;

double schlick(double cosine, double ref_idx) {
    auto r0 = (1-ref_idx) / (1+ref_idx);
    r0 = r0*r0;
    return r0 + (1-r0)*pow((1 - cosine),5);
}

class material 
{
    public:
        virtual bool scatter(
            const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
        ) const = 0;
};

class lambertian : public material 
{
    public:
        // 4.2. Texture Coordinates for Spheres
        // lambertian(const color& a) : albedo(a) {}
        lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
        lambertian(shared_ptr<texture> a) : albedo(a) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) 
            const override 
        {
            vec3 scatter_direction = rec.normal + random_unit_vector();
            // 2.4. Tracking the Time of Ray Intersection
            // scattered = ray(rec.p, scatter_direction);
            scattered = ray(rec.p, scatter_direction, r_in.time());
            // 4.2. Texture Coordinates for Spheres
            // attenuation = albedo;
            attenuation = albedo->value(rec.u, rec.v, rec.p);

            return true;
        }

    public:
        // 4.2. Texture Coordinates for Spheres
        // color albedo;
        shared_ptr<texture> albedo;
};

class metal : public material 
{
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) 
            const override 
        {
            vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_in_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }

    public:
       color albedo; 
       double fuzz;
};

class dielectric : public material
{
    public:
        dielectric(double ri) : ref_idx(ri) {}

        virtual bool scatter( const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) 
            const override 
        {
            attenuation = color(1.0, 1.0, 1.0);
            double etai_over_etat = rec.front_face ? (1.0 / ref_idx) : ref_idx;

            vec3 unit_direction = unit_vector(r_in.direction());
            
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta*cos_theta);
            if (etai_over_etat * sin_theta > 1.0 ) {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected);
                return true;
            }
            double reflect_prob = schlick(cos_theta, etai_over_etat);
            if (random_double() < reflect_prob)
            {
                vec3 reflected = reflect(unit_direction, rec.normal);
                scattered = ray(rec.p, reflected);
                return true;
            }
            vec3 refracted = refract(unit_direction, rec.normal, etai_over_etat);
            scattered = ray(rec.p, refracted);
            return true;
        }

        double ref_idx;
};

#endif

// 4. Solid Texture
// A texture in graphics usually means a function that makes the colors on a surface procedural.

// 4.1. The First Texture Class: Constant Texture
#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"

class texture 
{
    public:
        virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture 
{
    public:
        solid_color() {}
        solid_color(color c) : color_value(c) {}

        solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}

        virtual color value(double u, double v, const vec3& p) const override 
        {
            return color_value;
        }

    private:
        color color_value;
};

// 4.3. A Checker Texture
class checker_texture : public texture 
{
    public:
        checker_texture() {}

        checker_texture(shared_ptr<texture> t0, shared_ptr<texture> t1) : even(t0), odd(t1) {}

        checker_texture(color c1, color c2) 
            : even(make_shared<solid_color>(c1)) , odd(make_shared<solid_color>(c2)) {}

        virtual color value(double u, double v, const point3& p) const override 
        {
            auto sines = sin(10*p.x())*sin(10*p.y())*sin(10*p.z());
            if (sines < 0) return odd->value(u, v, p);
            else           return even->value(u, v, p);
        }

    public:
        shared_ptr<texture> even;
        shared_ptr<texture> odd;
};

// 5.1. Using Blocks of Random Numbers
#include "perlin.h"
class noise_texture : public texture 
{
    public:
        noise_texture() {}
        // 5.4. Tweaking The Frequency
        // It is also a bit low frequency in 5.2. 
        // We can scale the input point to make it vary more quickly:
        noise_texture(double sc) : scale(sc) {}

        virtual color value(double u, double v, const point3& p) const override 
        {
            // 5.1 Using Blocks of Random Numbers
            // return color(1,1,1) * noise.noise(p);
            
            // 5.4. Tweaking The Frequency
            // return color(1,1,1) * noise.noise(scale * p);
            
            // 5.5. Using Random Vectors on the Lattice Points
            // return color(1,1,1) * noise.noise(scale * p);
            // The output of the perlin interpretation can return negative values. 
            // These negative values will be passed to the sqrt() function 
            // of our gamma function and get turned into NaNs. 
            // We will cast the perlin output back to between 0 and 1.
            // return color(1,1,1) * 0.5 * (1.0 + noise.noise(scale * p));
            
            // 5.6. Introducing Turbulence
            // return color(1,1,1) * noise.turb(scale * p);
            
            // 5.7. Adjusting the Phase
            // However, usually turbulence is used indirectly. For example, 
            // the “hello world” of procedural solid textures is a simple marble-like texture. 
            // The basic idea is to make color proportional to something like a sine function, 
            // and use turbulence to adjust the phase (so it shifts x in sin(x)) 
            // which makes the stripes undulate. 
            // Commenting out straight noise and turbulence, and giving a marble-like effect is:
            return color(1,1,1) * 0.5 * (1 + sin(scale*p.z() + 10*noise.turb(p)));
        }

    public:
        perlin noise;
        double scale;
};

#endif

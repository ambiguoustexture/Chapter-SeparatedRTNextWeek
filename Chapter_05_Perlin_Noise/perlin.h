// 5. Perlin Noise
// A key part of Perlin noise is that it is repeatable: 
// it takes a 3D point as input and always returns the same randomish number. 
// Nearby points return similar numbers. 
// Another important part of Perlin noise is that it be simple and fast.

#ifndef PERLIN_H
#define PERLIN_H

#include "rtweekend.h"

// 5.2. Smoothing out the Result
// Linearly interpolate to make it smooth
inline double trilinear_interp(double c[2][2][2], double u, double v, double w) 
{
    auto accum = 0.0;
    for (int i=0; i < 2; i++)
        for (int j=0; j < 2; j++)
            for (int k=0; k < 2; k++)
                accum += (i*u + (1-i)*(1-u)) * (j*v + (1-j)*(1-v)) *
                         (k*w + (1-k)*(1-w)) * c[i][j][k];

    return accum;
}

class perlin {
    public:
        perlin() 
        {
            // 5.5. Using Random Vectors on the Lattice Points
            // to instead put random unit vectors (instead of just floats) on the lattice points, 
            // and use a dot product to move the min and max off the lattice.
            // ranfloat = new double[point_count];
            ranvec = new vec3[point_count];
            for (int i = 0; i < point_count; ++i) {
                // ranfloat[i] = random_double();
                ranvec[i] = unit_vector(vec3::random(-1,1));
            }

            perm_x = perlin_generate_perm();
            perm_y = perlin_generate_perm();
            perm_z = perlin_generate_perm();
        }

        ~perlin() 
        {
            // 5.5. Using Random Vectors on the Lattice Points
            // delete[] ranfloat;
            delete[] ranvec;
            delete[] perm_x;
            delete[] perm_y;
            delete[] perm_z;
        }

        double noise(const point3& p) const 
        {
            // 5.5. Using Random Vectors on the Lattice Points
            // auto u = p.x() - floor(p.x());
            // auto v = p.y() - floor(p.y());
            // auto w = p.z() - floor(p.z());
            auto u = p.x() - floor(p.x());
            auto v = p.y() - floor(p.y());
            auto w = p.z() - floor(p.z());

            // 5.3. Improvement with Hermitian Smoothing
            // Smoothing yields an improved result,
            // but there are obvious grid features in 5.2.
            // Some of it is Mach bands,
            // a known perceptual artifact of linear interpolation of color.
            // A standard trick is to use a Hermite cubic
            // to round off the interpolation:
            u = u*u*(3-2*u);
            v = v*v*(3-2*v);
            w = w*w*(3-2*w);

            // 5.2. Smoothing out the Result
            // Linearly interpolate to make it smooth
            // auto i = static_cast<int>(4*p.x()) & 255;
            // auto j = static_cast<int>(4*p.y()) & 255;
            // auto k = static_cast<int>(4*p.z()) & 255;

            // return ranfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
            int i = floor(p.x());
            int j = floor(p.y());
            int k = floor(p.z());
            // 5.5. Using Random Vectors on the Lattice Points
            // double c[2][2][2];
            vec3 c[2][2][2];

            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++)
                        // c[di][dj][dk] = ranfloat[
                        //     perm_x[(i+di) & 255] ^ 
                        //     perm_y[(j+dj) & 255] ^ 
                        //     perm_z[(k+dk) & 255]];
                        c[di][dj][dk] = ranvec[
                            perm_x[(i+di) & 255] ^
                            perm_y[(j+dj) & 255] ^
                            perm_z[(k+dk) & 255]
                        ];
            return perlin_interp(c, u, v, w);
        }

        // 5.6. Introducing Turbulence
        // Very often, a composite noise that has multiple summed frequencies is used. 
        // This is usually called turbulence, and is a sum of repeated calls to noise:
        double turb(const point3& p, int depth=7) const {
            auto accum = 0.0;
            auto temp_p = p;
            auto weight = 1.0;

            for (int i = 0; i < depth; i++) {
                accum += weight * noise(temp_p);
                weight *= 0.5;
                temp_p *= 2;
            }

            return fabs(accum);
        }

    private:
        // 5.5. Using Random Vectors on the Lattice Points
        // the interpolation becomes a bit more complicated:
        inline double perlin_interp(vec3 c[2][2][2], double u, double v, double w) const 
        {
            auto uu = u*u*(3-2*u);
            auto vv = v*v*(3-2*v);
            auto ww = w*w*(3-2*w);
            auto accum = 0.0;

            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++) {
                        vec3 weight_v(u-i, v-j, w-k);
                        accum += (i*uu + (1-i)*(1-uu))
                               * (j*vv + (1-j)*(1-vv))
                               * (k*ww + (1-k)*(1-ww))
                               * dot(c[i][j][k], weight_v);
                    }

            return accum;
        }

        static const int point_count = 256;
        // double* ranfloat;
        vec3* ranvec;
        int* perm_x;
        int* perm_y;
        int* perm_z;

        static int* perlin_generate_perm() 
        {
            auto p = new int[point_count];

            for (int i = 0; i < perlin::point_count; i++)
                p[i] = i;

            permute(p, point_count);

            return p;
        }

        static void permute(int* p, int n) 
        {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
};

#endif

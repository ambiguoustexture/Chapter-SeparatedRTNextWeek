// 3. Bounding Volume Hierarchies
// The ray-object intersection is the main time-bottleneck in a ray tracer, 
// and the time is linear with the number of objects.
// But it’s a repeated search on the same model, 
// so we ought to be able to make it a logarithmic search in the spirit of binary search.
// Because we are sending millions to billions of rays on the same model, 
// we can do an analog of sorting the model, and then each ray intersection can be a sublinear search.

// 3.1. The Key Idea
// The key idea of a bounding volume over a set of primitives is 
// to find a volume that fully encloses (bounds) all the objects.

// 3.2. Hierarchies of Bounding Volumes
// To make things sub-linear we need to make the bounding volumes hierarchical.

// 3.3. Axis-Aligned Bounding Boxes (AABBs)
// The key observation to turn that 1D math into a hit test is that for a hit, 
// the t-intervals need to overlap.

// 3.4. Ray Intersection with an AABB

#ifndef AABB_H
#define AABB_H

#include "rtweekend.h"

class aabb {
    public:
        aabb() {}
        aabb(const point3& a, const point3& b) { _min = a; _max = b;}

        point3 min() const {return _min; }
        point3 max() const {return _max; }

        bool hit(const ray& r, double tmin, double tmax) const {
            for (int a = 0; a < 3; a++) {
                // auto t0 = fmin((_min[a] - r.origin()[a]) / r.direction()[a],
                //                (_max[a] - r.origin()[a]) / r.direction()[a]);
                // auto t1 = fmax((_min[a] - r.origin()[a]) / r.direction()[a],
                //                (_max[a] - r.origin()[a]) / r.direction()[a]);
                // tmin = fmax(t0, tmin);
                // tmax = fmin(t1, tmax);

                // 3.5. An Optimized AABB Hit Method
                auto invD = 1.0f / r.direction()[a];
                auto t0 = (min()[a] - r.origin()[a]) * invD;
                auto t1 = (max()[a] - r.origin()[a]) * invD;
                if (invD < 0.0f) std::swap(t0, t1);
                tmin = t0 > tmin ? t0 : tmin;
                tmax = t1 < tmax ? t1 : tmax;

                if (tmax <= tmin) return false;
            }
            return true;
        }

        point3 _min;
        point3 _max;
};

// 3.7. Creating Bounding Boxes of Lists of Objects
aabb surrounding_box(aabb box0, aabb box1) {
    point3 small(fmin(box0.min().x(), box1.min().x()),
                 fmin(box0.min().y(), box1.min().y()),
                 fmin(box0.min().z(), box1.min().z()));

    point3 big(fmax(box0.max().x(), box1.max().x()),
               fmax(box0.max().y(), box1.max().y()),
               fmax(box0.max().z(), box1.max().z()));

    return aabb(small,big);
}

#endif

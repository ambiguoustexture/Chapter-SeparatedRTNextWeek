// In a real camera, the shutter opens and stays open for a time interval, 
// and the camera and objects may move during that time. 
// Its really an average of what the camera sees over that interval 
// that we want.

// 2.1. Introduction of SpaceTime Ray Tracing
// We can get a random estimate by sending each ray at some random time 
// when the shutter is open. 
// As long as the objects are where they should be at that time, 
// we can get the right average answer with a ray 
// that is at exactly a single time.
// 
// The basic idea is to generate rays at random times 
// while the shutter is open and intersect the model at that one time. 
// The way it is usually done is to have the camera move and the objects move, 
// but have each ray exist at exactly one time. 
// This way the “engine” of the ray tracer can just make sure 
// the objects are where they need to be for the ray, 
// and the intersection guts don’t change much.

#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        // 2.1. Introduction of SpaceTime Ray Tracing
        // To have a ray store the time it exists at.
        // ray(const point3& origin, const vec3& direction) 
        //     : orig(origin), dir(direction) {}
        ray(const point3& origin, const vec3& direction, double time = 0.0)
            : orig(origin), dir(direction), tm(time) {}
        // 2.1. Introduction of SpaceTime Ray Tracing
        point3 origin() const  { return orig; }
        vec3 direction() const { return dir; }

        double time() const {return tm; }

        point3 at(double t) const {
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
        // 2.1. Introduction of SpaceTime Ray Tracing
        double tm;
};

#endif


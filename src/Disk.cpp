#include "Disk.h"
#include <limits>
using namespace Raytracer148;
using namespace Eigen;
using namespace std;

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection
HitRecord Disk::intersect(const Ray &ray) {
    HitRecord result;
    result.t = -1;
    
    float t = 0;
    
    // Dot product between normal and direction of ray to find denominator of ray parametric equation
    float denom = (ray.direction).dot(n);

    // Make sure the denominator is not lower than a very small number
    if (denom > 1e-6) {
        // Ray parametric equation to find the parametric distance between the origin of ray and point of interest along the ray
        t = (p - ray.origin).dot(n) / denom;
    }
    
    result.position = ray.origin + ray.direction * t;
    // Compute intersection point
    Vector3d v = result.position - (p);
    
    // Compute the distance from this point to the disk's center
    float d2 = v.dot(v);
    
    // If distance is lower or equal to the disk radius, then hit!
    if (sqrtf(d2) <= r) {
        result.t = sqrtf(d2);
    }
    
    // Taken from sphere intersect function
    result.position = ray.origin + result.t * ray.direction;
    result.normal = (result.position - n).normalized();
    
    // Add color
    result.color[0] = rd;
    result.color[1] = gr;
    result.color[2] = bl;
    
    return result;
}

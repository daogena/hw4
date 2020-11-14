#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"

namespace Raytracer148 {
class Sphere : public Shape {
public:
    Sphere(Eigen::Vector3d &center, double radius, double red, double green, double blue) : c(center), r(radius), rd(red), gr(green), bl(blue) {}

    virtual HitRecord intersect(const Ray &ray);
    bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1);

private:
    Eigen::Vector3d c;
    double r;
    double rd;
    double gr;
    double bl;
};
}

#endif

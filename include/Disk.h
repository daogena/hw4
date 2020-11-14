#ifndef DISK_H
#define DISK_H

#include "Shape.h"

namespace Raytracer148 {
    class Disk : public Shape {
    public:
        Disk(Eigen::Vector3d &normal, Eigen::Vector3d &position, double radius, double red, double green, double blue) : n(normal), p(position), r(radius), rd(red), gr(green), bl(blue) {}

        virtual HitRecord intersect(const Ray &ray);

    private:
        Eigen::Vector3d n;
        Eigen::Vector3d p; 
        double r;
        double rd;
        double gr;
        double bl;
    };
}

#endif




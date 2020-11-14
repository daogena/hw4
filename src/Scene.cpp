#include "Scene.h"
#include <cmath>
using namespace Raytracer148;
using namespace std;
using namespace Eigen;

HitRecord Scene::closestHit(const Ray &ray) {
    HitRecord result;
    result.t = -1;
    bool foundSomething = false;

    for (unsigned int i = 0; i < shapes.size(); i++) {
        HitRecord r = shapes[i]->intersect(ray);
        if (r.t > std::numeric_limits<double>::epsilon() && (!foundSomething || r.t < result.t)) {
            result = r;
            foundSomething = true;
        }
    }

    return result;
}

// Returns an RGB color, where R/G/B are each in the range [0,1]
// Added depth to render reflection
Vector3d Scene::trace(const Ray &ray, const int &depth) {
    // For now we'll have diffuse shading with no shadows, so it's easy!
    HitRecord r = closestHit(ray);
    
    // Set view position to calculate specular lighting
    Vector3d viewPos;
    viewPos[0] = 3;
    viewPos[1] = 0;
    viewPos[2] = 0;
    
    Vector3d result;
    result[0] = result[1] = result[2] = 0;

    if (r.t < numeric_limits<double>::epsilon()) return result;

    // Ambient
    double ambientStrength = 0.2;
    
    // Diffuse - already done
    Vector3d lightDir = (lightPos - r.position).normalized();
    double dot = lightDir.dot(r.normal);
    if (dot < 0) dot = 0;
    
    // Specular
    double specularStrength = 0.5;
    Vector3d viewDir = (viewPos - r.position).normalized();
    // Equation for reflect function in OpenGL
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml
    Vector3d reflectDir = -lightDir - 2 * -lightDir.dot(r.normal) * r.normal;
    double specular = viewDir.dot(reflectDir) * specularStrength;
    
    double phong = ambientStrength + dot + specular;
    
    // Casting shadows
    // https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/implementing-the-raytracing-algorithm
    Ray shadowRay;
    shadowRay.direction = lightDir;
    shadowRay.origin = r.position;

    for (int i = 0; i < shapes.size(); ++i) {
        // Check if shadow ray intersects the object
        HitRecord r = shapes[i]->intersect(shadowRay);
        
        // If the parametric distance between the origin of ray and point of interest along the ray is greater than a very small number, hit
        if (r.t > 1e-6) {
            return result;
        }
    }
    
    // Reflection
    // https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing
    Vector3d reflection;

    // Point of intersection
    Vector3d phit = ray.origin + ray.direction * r.t;
    
    // Normal at intersection point
    Vector3d nhit = phit - r.normal;
    nhit.normalize();
    
    // Offset the reflection ray
    float bias = 1e-10;
    
    // Check if ray is hitting the inside of the shape
    if (ray.direction.dot(nhit) > 0) {
        nhit = -nhit;
    }
    
    if (depth < 5) {
        // Compute reflection direction
        Vector3d refldir = ray.direction - nhit * 2 * ray.direction.dot(nhit);
        refldir.normalize();

        Ray r1;
        r1.origin = phit + nhit * bias;
        r1.direction = refldir;

        // Trace reflection ray at depth
        reflection = trace(r1, depth + 1);
    }

    // Multiply each color with the Blinn-Phong model
    result[0] = (r.color[0]) * phong;
    result[1] = (r.color[1]) * phong;
    result[2] = (r.color[2]) * phong;
    
    result += reflection;
    
    return result;
}

void Scene::render(Image &image) {
    // We make the assumption that the camera is located at (0,0,0)
    // and that the image plane happens in the square from (-1,-1,1)
    // to (1,1,1).

    assert(image.getWidth() == image.getHeight());

    int size = image.getWidth();
    double pixelSize = 2. / size;
    for (int x = 0; x < size; x++)
        for (int y = 0; y < size; y++) {
            Ray curRay;
            curRay.origin[0] = curRay.origin[1] = curRay.origin[2] = 0;

            curRay.direction[0] = (x + 0.5) * pixelSize - 1;
            curRay.direction[1] = 1 - (y + 0.5) * pixelSize;
            curRay.direction[2] = 1;

            Vector3d color = trace(curRay, 3);
            image.setColor(x, y, color[0], color[1], color[2]);
        }
}

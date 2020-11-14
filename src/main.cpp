#include <iostream>
#include "Image.h"
#include "Scene.h"
#include "Sphere.h"
#include "Disk.h"
using namespace std;
using namespace Raytracer148;
using namespace Eigen;

int main() {
    Image im(400, 400);

    Scene scene;
    
    Vector3d center;

    center[0] = 0;
    center[1] = 0;
    center[2] = 6;
    scene.addShape(new Sphere(center, 3, 0, 0, 0.5));
    
    center[0] = 1;
    center[1] = 1;
    center[2] = 2.75;
    scene.addShape(new Sphere(center, 0.5, 0, .5, 0));

    center[0] = 1.25;
    center[1] = 0;
    center[2] = 2.75;
    scene.addShape(new Sphere(center, 0.5, .5, 0, 0));

    center[0] = .25;
    center[1] = -0.25;
    center[2] = 2;

    // Adding disk to scene
    Vector3d center1;
    center1[0] = 3;
    center1[1] = -2;
    center1[2] = 10;

    scene.addShape(new Disk(center, center1, 2, 0, 0.5, 0.5));


    scene.render(im);

    im.writePNG("test.png");

    return 0;
}

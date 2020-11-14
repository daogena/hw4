# hw4
This HW4 assignment was completed by Gena Dao, PSID: 1845342.

## Objective
The objective of this project was to add some features to a barebones ray tracer. 

## Environment
The environment that I used was XCode on MacOS. For this assignment, I had to install several libraries using the following Homebrew commands: 
```bash 
brew install eigen
```
```bash 
brew install libpng
```
In the provided Makefil, I had to change EIGEN_DIR to equal eigen3.3.8/ since this Homebrew command installed the latest version of the eigen library. I had to add the objs/disk.o object to OBJS. Additionally, I added one more line to the default section of the Makefile: 
```bash
objs/disk.o: src/Disk.cpp include/Disk.h
	$(CPP) -c $(CPPFLAGS) $(INCLUDE) src/Disk.cpp -o objs/disk.o
```
The program is compiled in the Mac (UNIX) terminal using the Makefile and the following command: 
```bash
make
```
The program was then run using the following command: 
```bash
./hw4
```
to generate the text.png. 

## Project details
This project required the completion of four different problems, which was the addition of four new features to the ray tracer. 
### Problem 1
This problem entailed adding another shape to the ray tracer. To do this, I added two new files: Disk.cpp and Disk.h. I used a scratchapixel.com tutorial to create these as well as the Sphere.cpp and Sphere.h for a template. 
#### Disk.h
A disk is also a shape, so it inherits from the Shape class. 
```cpp
class Disk : public Shape
```
A disk requires the input of a normal vector, a position vector and a radius. This was added as a public constructor:
```cpp
Disk(Eigen::Vector3d &normal, Eigen::Vector3d &position, double radius) : n(normal), p(position), r(radius) {}
```
These variables were included as private variables in the Disk class. 
#### Disk.cpp
There is only one function to be completed, which is the intersect(const Ray &ray) function. In completing this, I used the scratchapixel tutorial to combine some features of a plane and a disk into one in order to form the disk. 
I first declare the result, which the function returns at the end, and t, which is required by the result: 
```cpp
HitRecord result; 
result.t = -1; 

float t = 0;
```
Next, I find the dot product between the normal and direction of the ray to find the denominator of the ray parametric equation: 
```cpp
float denom = (ray.direction).dot(n); 
```
I make sure that the denominator is not lower than a very small number to determine whether or not we can use the ray parametric equation to find the parametric distance between the origin of ray and point of interest along the ray; otherwise, we would be dividing by zero. p is the position of the disk, a private variable we included earlier.  
```cpp
if (denom > 1e-6) {
  t = (p - ray.origin).dot(n) / denom; 
}
```
The position of the result is then calculated in order to compute the intersection point: 
```cpp
result.position = ray.origin + ray.direction * t; 

Vector3d v = result.position - p; 
```
Then, we comput the distinct from this point to the disk's center: 
```cpp
float d2 = v.dot(v); 
```
If this distance is lower or equal to the disk radius, then there's a hit. We take the square root of d2 to compare it to the radius, which isn't squared: 
```cpp
if (sqrtf(d2) < r) {
  result.t = sqrtf(d2); 
}
```
Lastly, taken from the sphere intersect function, we find the position and normal of the result to return: 
```cpp
result.position = ray.origin + result.t * ray.direction;
result.normal = (result.position - n).normalized(); 

return result; 
```
#### main.cpp
This shape is then added to the main.cpp file to render. Unlike the sphere, it requires two vectors a normal and a position, so I added another vector to input for the position. 
```cpp
scene.addShape(new Disk(center, center1, 2)); 
```
### Problem 2
This second problem requires adding Phong shading to the scene with each shape being a different color. 
#### Scene.cpp
In this file, I had to add to the trace function, which returns an RGB color. The code I'm using is almost similar to the code I used for my hw3. To add the ambient light, all I needed was an ambient strength: 
```cpp
double ambientStrength = 0.2; 
```
The diffuse lighting was already done in the given code.
For specular lighting, I needed to add a view position: 
```cpp
Vector3d viewPos; 
viewPos[0] = 3; 
viewPos[1] = 0; 
viewPos[2] = 0; 
```
Then, I had to define the specular strength as well: 
```cpp
double specularStrength = 0.5; 
```
Next, I had to calculate the view direction and the reflection direction. Since we could not use OpenGL for this homework, I used a khronos website to find out what the reflection function for OpenGL really calculates. 
```cpp
Vector3d viewDir = (viewPos - r.position).normalized();
Vector3d reflectDir = -lightDir - 2 * -lightDir.dot(r.normal) * r.normal;
```
Then, to calculate the specular lighting: 
```cpp
double specular = viewDir.dot(reflectDir) * specularStrength;
```
The phong model is calculated using the following formula: 
```cpp
double phong = ambientStrength + dot + specular;
```
At the end of the function, each result color is multiplied by the phong variable to produce the Blinn-Phong model on all shapes: 
```cpp
result[0] = (r.color[0]) * phong;
result[1] = (r.color[1]) * phong;
result[2] = (r.color[2]) * phong;
```
### Disk.cpp, Disk.h, Sphere.cpp, Sphere.h
The colors then need to be added to each shape. I'll use Disk in this example. 
In both the header files, I added three more private variables and in the constructor for RGB. 
```cpp
Disk(Eigen::Vector3d &normal, Eigen::Vector3d &position, double radius, double red, double green, double blue) : n(normal), p(position), r(radius), rd(red), gr(green), bl(blue) {}
```
In both intersection functions, I assign the colors at the end before returning the result: 
```cpp
result.color[0] = rd;
result.color[1] = gr;
result.color[2] = bl;
```
### main.cpp
Each shape in main.cpp now needs three extra parameters like so: 
```cpp
scene.addShape(new Disk(center, center1, 2, 0, 0.5, 0.5));
```
### Problem 3
This problem required adding support for casting shadows. 
#### Scene.cpp
In this file, I also added to the trace function the support for casting shadows. I used a scratchapixel tutorial to figure out how. 
First, I created a shadow ray with the same direction as the light direction and the origin as the closestHit(ray) position: 
```cpp
Ray shadowRay;
shadowRay.direction = lightDir;
shadowRay.origin = r.position;
```
Next, I looped through all of the shapes in the shapes array to find if the shadow ray intersects the object. If the parametric distance between the origin of the ray and the point of interest along the ray is greater than a very small number, it is a hit. 
```cpp
for (int i = 0; i < shapes.size(); ++i) {
    HitRecord r = shapes[i]->intersect(shadowRay);

    if (r.t > 1e-6) {
        return result;
    }
}
```
### Problem 4
This problem required adding support for mirrored surfaces that reflect rays of light according to the law of reflection.
#### Scene.cpp
In this file, I added reflection to the trace function. I had to add another parameter to this function to account for the depth of reflection. Outside of the depth if statement, I declared the reflection vector, the point of intersection (phit), the normal at the intersection point (nhit), and the offset (bias) of the reflection ray to avoid graininess in the shadows.
```cpp
Vector3d reflection;

Vector3d phit = ray.origin + ray.direction * r.t;

Vector3d nhit = phit - r.normal;
nhit.normalize();

float bias = 1e-10;
```
Then, I check if the ray is hitting the inside of the shape, meaning that nhit is negative. If it is, nhit is reversed to be positive: 
```cpp
if (ray.direction.dot(nhit) > 0) {
    nhit = -nhit;
}
```
Next, we make sure that the maximum recursion depth is less than 5 to avoid an infinite loop. In this conditional, we compute the reflection direction, assign the reflection ray and trace the reflection ray. 
```cpp
if (depth < 5) {
    Vector3d refldir = ray.direction - nhit * 2 * ray.direction.dot(nhit);
    refldir.normalize();

    Ray r1;
    r1.origin = phit + nhit * bias;
    r1.direction = refldir;

    reflection = trace(r1, depth + 1);
}
```
This reflection ray is added to the final result before returning: 
```cpp
result += reflection; 
```
## Result
The result is four different images, one for each problem.

Problem 1: 

<img width="400" alt="problem1" src="https://user-images.githubusercontent.com/66931735/99137365-f7d0cd80-25ef-11eb-8fba-7dffc8b73ab1.png">
Problem 2: 

<img width="402" alt="problem2" src="https://user-images.githubusercontent.com/66931735/99137379-028b6280-25f0-11eb-83c1-b38de1194e2d.png">
Problem 3: 

<img width="399" alt="problem3" src="https://user-images.githubusercontent.com/66931735/99137385-0a4b0700-25f0-11eb-87f3-88a9eab550f5.png">
Problem 4: 

<img width="400" alt="problem4" src="https://user-images.githubusercontent.com/66931735/99137389-1040e800-25f0-11eb-9cc1-fef9c93b9f7a.png">

For problem 4, I drew in a red circle manually for the viewer to easily identify the reflection since it may be difficult to see. 
## References 
This website was used to add the disk shape: 

https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection

This website was used to find the reflect function of OpenGL: 

https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/reflect.xhtml

This website was used for casting shadows: 

https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-ray-tracing/implementing-the-raytracing-algorithm

This website was used for adding the reflection:

https://www.scratchapixel.com/code.php?id=3&origin=/lessons/3d-basic-rendering/introduction-to-ray-tracing



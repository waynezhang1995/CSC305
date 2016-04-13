#include "Image.h"
#include "raytrace.h"
#include <cstdlib>
#include <ctime>
//#include "raytracer.h"

int main(int, char**){

    Image image(1000, 1000);
    srand (static_cast <unsigned> (time(0)));
    //RayTrace(&image);     //old version
    RayTraceSphere(&image);
    image.show(" CSC 305 RayTracer Demo ");
    //image.save("/Users/waynezhang/Desktop/RayTracerDemo.png");
}

#include "Image.h"
#include "raytrace.h"
//#include "raytracer.h"

int main(int, char**){

    Image image(1000, 1000);
    //RayTrace(&image);     //old version
    RayTraceSphere(&image);
    image.show(" CSC 305 RayTracer Demo ");
    //image.save("/Users/waynezhang/Desktop/RayTracerDemo.png");
}

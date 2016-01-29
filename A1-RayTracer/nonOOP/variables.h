#pragma once
#include "Image.h"
#include <math.h>
#include <cmath>
#include <iostream>

    struct Vector3
    {
        float x, y, z;
        Vector3(float x_in, float y_in, float z_in)
        {
            x = x_in;
            y = y_in;
            z = z_in;
        }
    };

    Vector3 Add(Vector3 a, Vector3 b) {	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z); }
    Vector3 Minus(Vector3 a, Vector3 b) { return Vector3(a.x - b.x, a.y - b.y, a.z - b.z); }
    Vector3 MultiplyScalar(Vector3 a, float b) { return Vector3(a.x * b, a.y * b, a.z * b); }
    float DotProduct(Vector3 a, Vector3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }

    Vector3 Normalize(Vector3 a){
        float length = a.x * a.x + a.y * a.y + a.z * a.z;
        length = sqrt(length);
        return Vector3(a.x / length, a.y / length, a.z / length);
    }

    void SetColor(Pixel & px, Vector3 CalculatedColor){
        if (CalculatedColor.x < 0) px.R = 0;
        else if (CalculatedColor.x > 255) px.R = 255;
        else px.R = (unsigned char)CalculatedColor.x;

        if (CalculatedColor.y < 0) px.G = 0;
        else if (CalculatedColor.y > 255) px.G = 255;
        else px.G = (unsigned char)CalculatedColor.y;

        if (CalculatedColor.z < 0) px.B = 0;
        else if (CalculatedColor.z > 255) px.B = 255;
        else px.B = (unsigned char)CalculatedColor.z;

        px.A = 255;
    }

    /*
     * TODO: Optimize Structure
     * 1: Create an individual structure for each sphere?
     */
        //-500
    Vector3 Camera(256, 256, -500);
    Vector3 SphereCenter1(600, 156, 600);       //Sphere 1
    Vector3 SphereCenter2(300, 156 , 400);       //Sphere 2
    Vector3 SphereCenter3(350, 112 , 400);       //Sphere 3

    float SphereRadius1 = 100;                  //Sphere 1
    float SphereRadius2 = 100;                  //Sphere 2
    float SphereRadius3 = 56;                  //Sphere 3
    Vector3 Light(550,500 , 0);

    Vector3 AmbientColour(20, 10, 10);      //ambient component

    Vector3 DiffuseColourSphere1(255, 0, 0);        //red
    Vector3 SpecularColourSphere1(255,255,255);

    Vector3 DiffuseColourSphere2(124, 250, 0);//green
    Vector3 SpecularColourSphere2(255,255,255);

    Vector3 DiffuseColourSphere3(160, 32, 240);     //purple
    Vector3 SpecularColourSphere3(255,255,255);

    Vector3 BackgroundColor(0,0,0 );    //black
    Vector3 PlaneColour(200,100,100);
    Vector3 yellow(255,0,0);
    Vector3 White(255,255,255);
    Vector3 Black(0,0,0);

    /* define plane: Random point in the plane + plane normal */
    Vector3 plane(0,56,0);    //define plane. y_axis = 56 -->the height difference between sphere and plane is 100
    Vector3 planenormal(0,1,0);


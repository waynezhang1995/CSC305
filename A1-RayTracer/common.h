#pragma once
#include <math.h>
struct Vector3
{
    float x, y, z;
    Vector3() { x = y = z = 0; }
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
Vector3 CrossProduct(Vector3 a,Vector3 b){return Vector3(a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x);}
Vector3 Normalize(Vector3 a)
{
    float length = a.x * a.x + a.y * a.y + a.z * a.z;
    length = sqrt(length);
    return Vector3(a.x / length, a.y / length, a.z / length);
}
void SetColor(Pixel & px, Vector3 CalculatedColor)
{
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





Vector3 AmbientColour(20, 20, 20);
Vector3 DiffuseColour(124, 250, 250);
Vector3 BackgroundColor(8, 8, 8);

Vector3 Light(450.0f,900.0f,550.0f);
/*
Vector3 Light2(500,900,550);  //define second light source
Vector3 Light3(550,900,550);  //define second light source

Vector3 Light4(500,900,500);  //define second light source
Vector3 Light5(450,900,500);  //define second light source
Vector3 Light6(550,900,500);  //define second light source

Vector3 Light7(500,900,450);  //define second light source
Vector3 Light8(450,900,450);  //define second light source
Vector3 Light9(550,900,450);  //define second light source
*/

Vector3 Camera(500,500, -800);
Vector3 SpecularColourSphere(255,255,255);
Vector3 White(255,255,255);
Vector3 Black(0,0,0);
Vector3 yellow(135,135,135);
Vector3 left(0,0,255);
Vector3 right(255,0,0);
Vector3 back(135,135,135);
Vector3 top(245,245,200);
Vector3 red(255,0,0);

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
Vector3 Normalize(Vector3 a)
{
    float length = a.x * a.x + a.y * a.y + a.z * a.z;
    length = sqrt(length);
    return Vector3(a.x / length, a.y / length, a.z / length);
}



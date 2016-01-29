#pragma once

#include "common.h"

class Object
{
public:
    virtual bool Intersect(Vector3 Origin, Vector3 Direction,
                           float * t, Vector3 * normal) = 0;
    virtual Pixel DiffuseShade(int flag,Vector3 Direction,Vector3 Surface, Vector3 Normal,std::vector<Object *> pObjectList) = 0;
    virtual Vector3 getCenter() = 0;
    virtual float getRadius() = 0;
    virtual int getflag() = 0;
};

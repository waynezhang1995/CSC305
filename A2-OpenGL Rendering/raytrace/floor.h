#pragma once
#include "object.h"

class Floor : public Object
{
public:
    //****WARNING: YOU CANNOT COPY THIS CODE FOR YOUR ASSIGNMENT 1****
    //This is a partial demo, and it is ***INCORRECT***
    //PLEASE IMPLEMENT THE 'TRUE' RAY-PLANE INTERSECTION
    //ON SLIDE PAGE 21 IN 'RAY-SHADE.PDF' IN SYLLABUS
    virtual bool Intersect(Vector3 Origin, Vector3 Direction,
                           float * t, Vector3 * normal)
    {
        if (Direction.y > 0)
        {
            (*t) = Origin.y / Direction.y;
            normal->x = 0;
            normal->y = -1;
            normal->z = 0;
            return true;
        }
        return false;
    }
};

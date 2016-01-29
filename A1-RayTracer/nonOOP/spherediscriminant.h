#pragma once
#include "Image.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include "variables.h"

    float CalculateDiscriminant(Vector3 Direction, Vector3 OC, float SphereRadius){


        float D_Dot_OC = DotProduct(Direction, OC);
        float RadiusSquare = SphereRadius * SphereRadius;
        float OCSquare = DotProduct(OC, OC);

        return RadiusSquare - OCSquare + D_Dot_OC * D_Dot_OC;
    }

    float CalculateT(float discriminant,Vector3 OC, Vector3 Direction){
        float discRoot = sqrt(discriminant);
        float B = (-1) * DotProduct(Direction, OC);
        float t1 = B + discRoot;
        float t2 = B - discRoot;
        //determine which one is the real intersection point
        if (t1 > 0){
            if (t2 > 0){
                return t2;
            }
            else { //Camera is in the sphere
                return t1;
            }
        }
    }

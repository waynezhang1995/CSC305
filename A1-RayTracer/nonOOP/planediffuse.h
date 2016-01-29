#pragma once
#include "Image.h"
#include <math.h>
#include <tgmath.h>
#include <cmath>
#include <iostream>
#include "variables.h"

float shadow(Vector3 planeintersection,Vector3 SphereCenter,float SphereRadius){

    Vector3 Camera = planeintersection;      //define a new camera point = ray-plane intersection point
    Vector3 NewDirection = Minus(Light,Camera);//ray-plane intersection point to light
    NewDirection = Normalize(NewDirection);
    Vector3 NewOC = Minus(Camera, SphereCenter);
    float NewD_Dot_OC = DotProduct(NewDirection,NewOC);
    float NewOCSqure = DotProduct(NewOC,NewOC);
    return (SphereRadius*SphereRadius) - NewOCSqure + NewD_Dot_OC * NewD_Dot_OC;

}

void CalculatePlaneDiffuse(Vector3 Direction, Pixel &px,Vector3 NewCamera){

    int checkerSize = 128;
    /* Plane !!!!!!! */
     Vector3 PixelColour = AmbientColour;
    /* calculate t */
    float tplane = -1;
    Vector3 NminusP = Minus(NewCamera,plane);  //(o-p)
    float tmp = -1 * DotProduct(NminusP,planenormal); //(o-p).n
    float D_N = DotProduct(Direction,planenormal); // d.n
    tplane = tmp / D_N; // t

    if(tplane > 0){  //intersect with ray
        Vector3 planeintersection = MultiplyScalar(Direction,tplane);
        planeintersection = Add(planeintersection,NewCamera);//ray-plane intersection point
        /*
         * cast a new ray toward light source
         * Hard Shadow code begins:
         */
        float NewDiscriminant1 = shadow(planeintersection,SphereCenter1,SphereRadius1);
        float NewDiscriminant2 = shadow(planeintersection,SphereCenter2,SphereRadius2);
        //float NewDiscriminant3 = shadow(planeintersection,SphereCenter3,SphereRadius3);
        if(NewDiscriminant1 > 0 || NewDiscriminant2 > 0 ){  //intersect to one of the three spheres --> Blocked !
            SetColor(px, Black);
        }
        /**************************************/
        else{       //ray-plane intersection !!

            int blackOrWhite = 0;

            float result = abs((int)planeintersection.x/200%2);
            float result1 = abs((int)planeintersection.z/200%2);
            //std::cout << result << std::endl;

            if(result == result1 ){
                blackOrWhite = 1;
            }

            //std::cout << blackOrWhite<< std::endl;
            Vector3 PlaneLightVector =Minus(Light, planeintersection); //L
            PlaneLightVector = Normalize(PlaneLightVector);
            planeintersection = Normalize(planeintersection);

            float PlaneDiffuseTerm = DotProduct(planenormal,PlaneLightVector);

                if(PlaneDiffuseTerm > 0){

                    if (blackOrWhite){
                        Vector3 PixelDiffuseColour = MultiplyScalar(White, PlaneDiffuseTerm);
                        PixelColour = Add(PixelColour,PixelDiffuseColour);
                        SetColor(px, PixelColour);
                    }else{
                        Vector3 PixelDiffuseColour = MultiplyScalar(yellow, PlaneDiffuseTerm);
                        PixelColour = Add(PixelColour,PixelDiffuseColour);
                        SetColor(px, PixelColour);
                    }
                /*
                    Vector3 PixelDiffuseColour = MultiplyScalar(White, PlaneDiffuseTerm);
                    PixelColour = Add(PixelColour,PixelDiffuseColour);
                    SetColor(px, PixelColour);
                */
                }
        }
    }else{
        SetColor(px, BackgroundColor);
    }
}

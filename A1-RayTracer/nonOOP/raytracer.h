#pragma once
#include "Image.h"
#include <math.h>
#include "spherediffuse.h"
#include "planediffuse.h"
#include "variables.h"
#include "spherediscriminant.h"
#include <cmath>
#include <iostream>


void RayTrace(Image * pImage)
{
    for (int i = 0; i < 700; ++ i)
        for (int j = 0; j < 700; ++j)
        {
            Pixel px;
            Vector3 PixelPosition((float)i, (float)j, 0);
            Vector3 Direction = Minus(PixelPosition, Camera);
            Direction = Normalize(Direction);

            Vector3 OC1 = Minus(Camera, SphereCenter1);
            Vector3 OC2 = Minus(Camera, SphereCenter2);
            //Vector3 OC3 = Minus(Camera, SphereCenter3);

            float discriminant1 = CalculateDiscriminant(Direction,OC1,SphereRadius1);
            float discriminant2 = CalculateDiscriminant(Direction,OC2,SphereRadius2);
            //float discriminant3 = CalculateDiscriminant(Direction,OC3,SphereRadius3);

            float t_1= -1;
            float t_2= -1;
            //float t_3= -1;

            if (discriminant1 > 0){
               t_1 = CalculateT(discriminant1,OC1,Direction);
            }
            /* Sphere #2 */
            if (discriminant2 > 0){
               t_2 = CalculateT(discriminant2,OC2,Direction);
            }
            /*
            if (discriminant3 > 0){
               t_3 = CalculateT(discriminant3,OC3,Direction);
            }
            */
            int flag = 0; /*
                           *  if ray intersects with sphere 1, Flag = 1
                           *  if ray intersects with sphere 2, Flag = 2
                           *  if ray intersects with sphere 3, flag = 3
                           */
            if (t_1 > 0){
                flag = 1;
                CalculateSphereDiffuse(px,t_1,Direction,Camera,1);
            }else if(t_2 > 0){ //No Intersection, calculate plane intersection
                flag = 2;
                CalculateSphereDiffuse(px,t_2,Direction,Camera,2);
            }/*else if(t_3 >0){
                flag = 3;
                CalculateSphereDiffuse(px,t_3,Direction,3);
            }*/else{
                CalculatePlaneDiffuse(Direction,px,Camera);
            }

                //128 500

                (*pImage)(i, 700-j) = px; // Change origin !!!!! top-left to bottom-left!!!!!!!!!


        }
}


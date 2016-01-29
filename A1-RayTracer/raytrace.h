//This is a simple ray tracing example showing how to use the Image 
#pragma once
#include "Image.h"
#include <vector>
#include "sphere.h"
#include "floor.h"
#include "mirrorsphere.h"
#include <iostream>

void RayTraceSphere(Image * pImage)
{
    std::vector<Object *> pObjectList;
    /*
     * Flag!!
     *
     * 1 = sphere;
     * 2 = mirror sphere
     *
     * 99 = left wall
     * 100 = bottom floor
     * 98 = right wall
     * 97 = back wall
     * 96 = top wall
     */
    Floor floorLeft(Vector3(-300,0,0),Vector3(1,0,0),99);
    Floor floorBot(Vector3(0,56,0),Vector3(0,1,0),100);
    Floor floorRight(Vector3(900,0,0),Vector3(-1,0,0),98);
    Floor floorBack(Vector3(0,0,1000),Vector3(0,0,-1),97);
    Floor floorTop(Vector3(0,700,0),Vector3(0,-1,0),96);

    Sphere sphere(Vector3(200, 156,300), //
                  100,1);//radius
    mirrorsphere sphereRefl(Vector3(500, 156, 500), //center
                  100,2);//radius
    pObjectList.push_back(&floorLeft);
    pObjectList.push_back(&floorBot);
    pObjectList.push_back(&floorRight);
    pObjectList.push_back(&floorBack);
    pObjectList.push_back(&floorTop);
    pObjectList.push_back(&sphere);
    pObjectList.push_back(&sphereRefl);

    for (int i = 0; i < 700; ++ i)
        for (int j = 0; j < 700; ++j)
		{
            //Set up the ray we're tracing: R = O + tD;
            Pixel px;
            Vector3 PixelPosition((float)i, (float)j, 0);
			Vector3 Direction = Minus(PixelPosition, Camera);
			Direction = Normalize(Direction);

            float t_min = 999999;
            Vector3 Normal_min;
            bool HasIntersection = false;
            int index;
            //Intersect with the list of objects
            for (int k = 0; k < pObjectList.size(); ++ k)
            {

                float t;
                Vector3 normal;
                bool DoesIntersect = pObjectList[k]->Intersect(Camera, Direction,
                                                             &t, &normal);
                if (DoesIntersect)
                {
                    HasIntersection = true;
                    if (t_min > t)
                    {
                        index = k;
                        t_min = t;
                        Normal_min = normal;
                    }
                }
            }

            if (HasIntersection)
			{              
                Vector3 Intersection = MultiplyScalar(Direction, t_min);
				Intersection = Add(Intersection, Camera);
                px = pObjectList[index]->DiffuseShade(pObjectList[index]->getflag(),Direction,Intersection, Normal_min,pObjectList);
			}//if t > 0
			else //No Intersection, set background colour
			{
				SetColor(px, BackgroundColor);
			}
			 
            (*pImage)(i, 700-j) = px;  //change origin
		}
}

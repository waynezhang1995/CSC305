//This is a simple ray tracing example showing how to use the Image 
#pragma once
#include "Image.h"
#include <vector>
#include "sphere.h"
#include "floor.h"
#include "mirrorsphere.h"
#include <iostream>
#include "triangle.h"
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
     *
     */
    Floor floorLeft(Vector3(-300,0,0),Vector3(1,0,0),99);
    Floor floorBot(Vector3(0,0,0),Vector3(0,1,0),100);
    Floor floorRight(Vector3(1300,0,0),Vector3(-1,0,0),98);
    Floor floorBack(Vector3(0,0,900),Vector3(0,0,-1),97);
    Floor floorTop(Vector3(0,1300,0),Vector3(0,-1,0),96);

    Sphere sphere(Vector3(300, 75,200), //
                  75,1);//radius
    mirrorsphere sphereRefl(Vector3(500, 200, 500), //center
                  200,2);//radius
    Sphere sphere2(Vector3(700, 75, 200), //center
                  75,1);//radius
    triangle BotLeft(Vector3(0,0,1000),Vector3(1000,0,0),Vector3(0,0,0),100);
    triangle BotRight(Vector3(1000,0,1000),Vector3(1000,0,0),Vector3(0,0,1000),100);

    triangle TopLeft(Vector3(0,1000,1000),Vector3(1000,1000,0),Vector3(0,1000,0),96);
    triangle TopRight(Vector3(1000,1000,1000),Vector3(1000,1000,0),Vector3(0,1000,1000),96);



    triangle LeftLeft(Vector3(0,0,0),Vector3(0,1000,0),Vector3(0,1000,1000),99);
    triangle LeftRight(Vector3(0,0,0),Vector3(0,0,1000),Vector3(0,1000,1000),99);

    triangle BackLeft(Vector3(0,0,1000),Vector3(1000,0,1000),Vector3(0,1000,1000),97);
    triangle BackRight(Vector3(0,1000,1000),Vector3(1000,0,1000),Vector3(1000,1000,1000),97);

    triangle RightLeft(Vector3(1000,0,0),Vector3(1000,1000,0),Vector3(1000,1000,1000),98);
    triangle RightRight(Vector3(1000,0,0),Vector3(1000,0,1000),Vector3(1000,1000,1000),98);

    pObjectList.push_back(&BotLeft);
    pObjectList.push_back(&BotRight);
    pObjectList.push_back(&TopLeft);
    pObjectList.push_back(&TopRight);
    pObjectList.push_back(&LeftLeft);
    pObjectList.push_back(&LeftRight);
    pObjectList.push_back(&BackLeft);
    pObjectList.push_back(&BackRight);

    pObjectList.push_back(&RightLeft);
    pObjectList.push_back(&RightRight);

    pObjectList.push_back(&sphere);
    pObjectList.push_back(&sphereRefl);
    pObjectList.push_back(&sphere2);

    for (int i = 0; i < 1000; ++ i)
        for (int j = 0; j < 1000; ++j)
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
			 
            (*pImage)(1000-j, i) = px;  //change origin
		}
}

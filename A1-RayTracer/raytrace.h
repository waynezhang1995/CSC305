//This is a simple ray tracing example showing how to use the Image
#pragma once
#include "Image.h"
#include <vector>
#include "sphere.h"
#include "mirrorsphere.h"
#include <iostream>
#include "triangle.h"



Vector3 TraceRay(Vector3 Direction,std::vector<Object *> pObjectList){
    Vector3 Colour;
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
        Colour = pObjectList[index]->DiffuseShade(pObjectList[index]->getflag(),Direction,Intersection, Normal_min,pObjectList);
        return Colour;
    }//if t > 0
    else //No Intersection, set background colour
    {
        return BackgroundColor;
    }
}


Vector3 randomVector(){

    float x = (-0.25) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.25-(-0.25))));
    float y = (-0.25) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(0.25-(-0.25))));

    return Vector3(x, y, 0.0f);
}

void RayTraceSphere(Image * pImage)
{
    std::vector<Object *> pObjectList;
    Sphere sphere(Vector3(200, 35,100),35,1);//radius
    Sphere sphere2(Vector3(400,45,750),45,1);//radius
    refrasphere sphereR(Vector3(800, 100, 500),100,6);//radius
    mirrorsphere sphereRefl(Vector3(500, 70, 300),70,2);

    triangle BotLeft(Vector3(0,0,1000),Vector3(1000,0,0),Vector3(0,0,0),100,Vector3(0,1,0),yellow);
    triangle BotRight(Vector3(1000,0,1000),Vector3(1000,0,0),Vector3(0,0,1000),100,Vector3(0,1,0),yellow);

    triangle TopLeft(Vector3(0,1000,1000),Vector3(1000,1000,0),Vector3(0,1000,0),96,Vector3(0,-1,0),top);
    triangle TopRight(Vector3(1000,1000,1000),Vector3(1000,1000,0),Vector3(0,1000,1000),96,Vector3(0,-1,0),top);

    triangle LeftLeft(Vector3(0,0,0),Vector3(0,1000,0),Vector3(0,1000,1000),99,Vector3(1,0,0),left);
    triangle LeftRight(Vector3(0,0,0),Vector3(0,0,1000),Vector3(0,1000,1000),99,Vector3(1,0,0),left);

    triangle BackLeft(Vector3(0,0,1000),Vector3(1000,0,1000),Vector3(0,1000,1000),97,Vector3(0,0,-1),back);
    triangle BackRight(Vector3(0,1000,1000),Vector3(1000,0,1000),Vector3(1000,1000,1000),97,Vector3(0,0,-1),back);

    triangle RightLeft(Vector3(1000,0,0),Vector3(1000,1000,0),Vector3(1000,1000,1000),98,Vector3(-1,0,0),right);
    triangle RightRight(Vector3(1000,0,0),Vector3(1000,0,1000),Vector3(1000,1000,1000),98,Vector3(-1,0,0),right);


    triangle CubeTopLeft(Vector3(100,200,300),Vector3(300,200,500),Vector3(100,200,500),56,Vector3(0,1,0),red);
    triangle CubeTopRight(Vector3(300,200,300),Vector3(300,200,500),Vector3(100,200,300),56,Vector3(0,1,0),red);

    triangle CubeLeftLeft(Vector3(100,0,500),Vector3(100,200,500),Vector3(100,200,300),59,Vector3(-1,0,0),red);
    triangle CubeLeftRight(Vector3(100,0,500),Vector3(100,0,300),Vector3(100,200,300),59,Vector3(-1,0,0),red);

    triangle CubeRightLeft(Vector3(300,0,500),Vector3(300,200,500),Vector3(300,200,300),58,Vector3(1,0,0),red);
    triangle CubeRightRight(Vector3(300,0,500),Vector3(300,0,300),Vector3(300,200,300),58,Vector3(1,0,0),red);

    triangle CubeFrontLeft(Vector3(100,0,300),Vector3(300,0,300),Vector3(100,200,300),57,Vector3(0,0,1),red);
    triangle CubeFrontRight(Vector3(100,200,300),Vector3(300,0,300),Vector3(300,200,300),57,Vector3(0,0,1),red);

    triangle CubeBackLeft(Vector3(100,0,500),Vector3(300,0,500),Vector3(100,200,500),55,Vector3(0,0,-1),red);
    triangle CubeBackRight(Vector3(100,200,500),Vector3(300,0,500),Vector3(300,200,500),55,Vector3(0,0,-1),red);

    triangle CubeBotLeft(Vector3(100,0,300),Vector3(300,0,500),Vector3(100,0,500),52,Vector3(0,-1,0),red);
    triangle CubeBotRight(Vector3(300,0,300),Vector3(300,0,500),Vector3(100,0,300),52,Vector3(0,-1,0),red);


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
    pObjectList.push_back(&sphere2);
    pObjectList.push_back(&sphereR);
    pObjectList.push_back(&sphereRefl);

    pObjectList.push_back(&CubeTopLeft);
    pObjectList.push_back(&CubeTopRight);
    pObjectList.push_back(&CubeLeftLeft);
    pObjectList.push_back(&CubeLeftRight);
    pObjectList.push_back(&CubeRightLeft);
    pObjectList.push_back(&CubeRightRight);
    pObjectList.push_back(&CubeBackLeft);
    pObjectList.push_back(&CubeBackRight);
    pObjectList.push_back(&CubeFrontLeft);
    pObjectList.push_back(&CubeFrontRight);
    pObjectList.push_back(&CubeBotLeft);
    pObjectList.push_back(&CubeBotRight);



       //static_cast <float> (rand()) / static_cast <float> (RAND_MAX);       // <--- Gives 0->1 Float

    for (int i = 0; i < 1000; ++ i)
        for (int j = 0; j < 1000; ++j)
		{

            //Set up the ray we're tracing: R = O + tD;
            Pixel px;
            Vector3 Colour(0.0f,0.0f,0.0f);
            Vector3 Colour2(0.0f,0.0f,0.0f);
            if(i == 0 || j == 0 || i == 1000 || j == 1000){
                 Vector3 PixelPosition((float)i, (float)j, 0);
                 Vector3 Direction = Normalize(Minus(PixelPosition, Camera));
                 Vector3 ColourEdge = TraceRay(Direction,pObjectList);
                 SetColor(px,ColourEdge);
                 (*pImage)(i, 1000-j) = px;  //change origin
            }else{

                /* Jitterd */

                Vector3 PixelPosition((float)i+0.5f, (float)j+0.5f, 0);
                //Vector3 sp[64];
                for(int i = 0; i < 512;i += 4){
                    Vector3 PixelPosition1 = Add(Add(PixelPosition, Vector3(-0.25, 0.25, 0)),randomVector());
                    Vector3 PixelPosition2 = Add(Add(PixelPosition, Vector3(0.25, 0.25, 0)),randomVector());
                    Vector3 PixelPosition3 = Add(Add(PixelPosition, Vector3(-0.25, -0.25, 0)),randomVector());
                    Vector3 PixelPosition4 = Add(Add(PixelPosition, Vector3(0.25, -0.25, 0)),randomVector());

                    Vector3 Direction1 = Normalize(Minus(PixelPosition1, Camera));
                    Vector3 Direction2 = Normalize(Minus(PixelPosition2, Camera));
                    Vector3 Direction3 = Normalize(Minus(PixelPosition3, Camera));
                    Vector3 Direction4 = Normalize(Minus(PixelPosition4, Camera));

                    Colour = Add(Colour,TraceRay(Direction1,pObjectList));
                    Colour = Add(Colour,TraceRay(Direction2,pObjectList));
                    Colour = Add(Colour,TraceRay(Direction3,pObjectList));
                    Colour = Add(Colour,TraceRay(Direction4,pObjectList));

                }

                Colour = MultiplyScalar(Colour,1.0f/512.0f);

                SetColor(px,Colour);
                (*pImage)(i, 1000-j) = px;  //change origin

            }
		}
}



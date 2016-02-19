//This is a simple ray tracing example showing how to use the Image 
#pragma once
#include "Image.h"
#include <vector>
#include "sphere.h"
#include "floor.h"


#include <iostream>

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
Vector3 DiffuseColour(250, 250, 250);
Vector3 BackgroundColor(8, 8, 8);
Vector3 Light(128, 128, 0);
//******WARNING: THIS CODE MAKES EVERYTHING IN THE SCENE LOOKS THE SAME****
//SIMPLY COPY THIS CODE DOES NOT MEET THE REQUIREMENT FOR ASSIGNMENT 1
//YOU ARE EXPECTED TO HAVE DIFFERENT SURFACE MATERIALS FOR DIFFERENT OBJECTS
//IN YOUR SCENE.
Pixel DiffuseShade(Vector3 Surface, Vector3 Normal)
{
    //These colours will give the sphere a red appearance
    Pixel shade;
    Vector3 LightVector = Minus(Light, Surface);
    LightVector = Normalize(LightVector);
    float DiffuseTerm = DotProduct(LightVector, Normal);
    Vector3 PixelColour = AmbientColour;
    if (DiffuseTerm > 0)
    {

        Vector3 PixelDiffuseColour = MultiplyScalar(DiffuseColour, DiffuseTerm);
        PixelColour = Add(PixelColour, PixelDiffuseColour);
    }
    SetColor(shade, PixelColour);
    return shade;
}

void RayTraceSphere(Image * pImage)
{
    std::vector<Object *> pObjectList;
    Floor floor;
    Sphere sphere(Vector3(256, 256, 450), //center
                  250);//radius
    pObjectList.push_back(&floor);
    pObjectList.push_back(&sphere);

    Vector3 Camera(256, 256, -400);

    for (int i = 0; i < 512; ++ i)
		for (int j = 0; j < 512; ++j)
		{
            //Set up the ray we're tracing: R = O + tD;
            Pixel px;
            Vector3 PixelPosition((float)j, (float)i, 0);
			Vector3 Direction = Minus(PixelPosition, Camera);
			Direction = Normalize(Direction);

            float t_min = 999999;
            Vector3 Normal_min;
            bool HasIntersection = false;

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
                        t_min = t;
                        Normal_min = normal;
                    }
                }
            }

            if (HasIntersection)
			{              
                Vector3 Intersection = MultiplyScalar(Direction, t_min);
				Intersection = Add(Intersection, Camera);
                px = DiffuseShade(Intersection, Normal_min);
			}//if t > 0
			else //No Intersection, set background colour
			{
				SetColor(px, BackgroundColor);
			}
			 
			(*pImage)(i, j) = px;
		}
}

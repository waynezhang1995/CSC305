#pragma once
#include <math.h>
#include <tgmath.h>
#include <cmath>
#include "object.h"
#include "common.h"
#include <iostream>

class refrasphere : public Object
{
    Vector3 Center;
    float Radius;
    int flag;

public:
    refrasphere(Vector3 Center_in, float Radius_in,int flag_in)
    {
        Center = Center_in;
        Radius = Radius_in;
        flag = flag_in;
    }
    virtual Vector3 getCenter(){return Center;}
    virtual float getRadius(){return Radius;}
    virtual int getflag() {return flag;}

    virtual bool Intersect(Vector3 Origin, Vector3 Direction,
                           float *t_out, Vector3 *normal_out)
    {
        Vector3 EO = Minus(Center, Origin);
        float v = DotProduct(EO, Direction);
        float RadiusSquare = Radius * Radius;
        float EO_Square = DotProduct(EO, EO);
        float discriminant = RadiusSquare - (EO_Square - v * v);

        float t = -1;
        if (discriminant > 0)
        {
            float d = sqrt(discriminant);
            t = v - d;
        }

        if (t > 0) {
            *t_out = t;
            Vector3 IntersectionPoint = MultiplyScalar(Direction, t);
            IntersectionPoint = Add(IntersectionPoint, Origin);
            Vector3 SurfaceNormal = Minus(IntersectionPoint, Center);
            (*normal_out) = Normalize(SurfaceNormal);
            return true;
        } else
        {
            return false;
        }
    }

    virtual Pixel DiffuseShade(int flag,Vector3 Direction,Vector3 Surface, Vector3 Normal,std::vector<Object *> pObjectList)
    {

        Pixel shade;
        int index = 0;
        bool HasIntersection = false;
        float t_min = 999999;
        Vector3 Normal_min;
        Vector3 priRayDir = Direction;
        float Rindex = 1.0003f/1.655f; //glass refraction index

        float c1 = -1 * DotProduct(Normal,priRayDir);
        float c2 = sqrt(1-Rindex*Rindex*(1-c1*c1));
        Vector3 SecondaryRay = Add(MultiplyScalar(priRayDir,Rindex),MultiplyScalar(Normal,(Rindex*c1-c2)));
        SecondaryRay = Normalize(SecondaryRay);

        for(int i =0;i<pObjectList.size();++i){
            if(pObjectList[i]->getflag() != 6){
                float t2;
                Vector3 normal2;
                bool DoesIntersect = pObjectList[i]->Intersect(Surface, SecondaryRay,
                                                             &t2, &normal2);
                if (DoesIntersect)
                {
                    HasIntersection = true;
                    if (t_min > t2)
                    {
                        index = i;
                        t_min = t2;
                        Normal_min = normal2;
                    }
                }
            }
        }

        if (HasIntersection)
        {
            Vector3 Intersection = MultiplyScalar(SecondaryRay, t_min);
            Intersection = Add(Intersection, Surface);
            shade = pObjectList[index]->DiffuseShade(pObjectList[index]->getflag(),SecondaryRay,Intersection, Normal_min,pObjectList);
        }//if t > 0

        else //No Intersection, set background colour
        {
            SetColor(shade, BackgroundColor);
        }

        return shade;
    }


};


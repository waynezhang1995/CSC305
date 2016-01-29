#pragma once
#include <math.h>
#include <tgmath.h>
#include <cmath>
#include "object.h"
#include "common.h"

class Floor : public Object
{
        Vector3 FloorPos;
        Vector3 FloorNormal;
        int flag;
public:
        Floor(Vector3 FloorPos_in,Vector3 FloorNormal_in,int flag_in){
               FloorPos = FloorPos_in;
               FloorNormal = FloorNormal_in;
               flag = flag_in;
        }
        virtual Vector3 getCenter() {}  // how to get rid of this ?
        virtual float getRadius() {}
        virtual int getflag() {return flag;}

    virtual bool Intersect(Vector3 Origin, Vector3 Direction,
                           float * t, Vector3 * normal)
    {
            (*normal) = FloorNormal;
            Vector3 NminusP = Minus(Origin,FloorPos);  //(o-p)
            float tmp = -1 * DotProduct(NminusP,FloorNormal); //(o-p).n
            float D_N = DotProduct(Direction,FloorNormal); // d.n
            (*t) = tmp / D_N; // t
            if((*t) > 0){
                return true;
            }else{
                return false;
            }
    }

    Vector3 addcolour(int flag,float PlaneDiffuseTerm,Vector3 PixelColour,int blackOrWhite){

        if(flag == 99){ //left
            Vector3 PixelDiffuseColour = MultiplyScalar(left, PlaneDiffuseTerm);
            PixelColour = Add(PixelColour,PixelDiffuseColour);
        }
        if(flag == 98){ //right
            Vector3 PixelDiffuseColour = MultiplyScalar(right, PlaneDiffuseTerm);
            PixelColour = Add(PixelColour,PixelDiffuseColour);
        }
        if(flag == 97){ //back
            Vector3 PixelDiffuseColour = MultiplyScalar(back, PlaneDiffuseTerm);
            PixelColour = Add(PixelColour,PixelDiffuseColour);
        }
        if(flag == 96 ){ //top
            Vector3 PixelDiffuseColour = MultiplyScalar(Black, PlaneDiffuseTerm);
            PixelColour = Add(PixelColour,PixelDiffuseColour);
        }
        if(flag == 100){

            if (blackOrWhite){
                Vector3 PixelDiffuseColour = MultiplyScalar(White, PlaneDiffuseTerm);
                PixelColour = Add(PixelColour,PixelDiffuseColour);
                //SetColor(shade, PixelColour);
            }else{
                Vector3 PixelDiffuseColour = MultiplyScalar(yellow, PlaneDiffuseTerm);
                PixelColour = Add(PixelColour,PixelDiffuseColour);
                //SetColor(shade, PixelColour);
            }


        }
        return PixelColour;
    }

    virtual Pixel DiffuseShade(int flag,Vector3 Direction,Vector3 Surface, Vector3 Normal,std::vector<Object *> pObjectList)
    {
             Pixel shade;
             Vector3 PixelColour = AmbientColour;
             for(int i =0;i<pObjectList.size();++i){     //calculate shadow for each sphere
                 if(pObjectList[i]->getflag() < 10){
                    float NewDiscriminant = shadow(Surface,pObjectList[i]->getCenter(),pObjectList[i]->getRadius(),Light);
                    float NewDiscriminant2 = shadow(Surface,pObjectList[i]->getCenter(),pObjectList[i]->getRadius(),Light2);
                    if(NewDiscriminant > 0  ){  //intersect to one of the three spheres --> Blocked !
                        SetColor(shade, Black);
                        return shade;
                    }
                    if(NewDiscriminant2 > 0  ){  //intersect to one of the three spheres --> Blocked !
                        SetColor(shade, Black);
                        return shade;
                    }
                }
             }
            int blackOrWhite = 0;
            float result = abs((int)Surface.x/150%2);
            float result1 = abs((int)Surface.z/150%2);
            //std::cout << result << std::endl;
            if(result == result1 ){
                blackOrWhite = 1;
            }
            Vector3 PlaneLightVector =Minus(Light, Surface); //L
            Vector3 PlaneLightVector2 =Minus(Light2, Surface); //L
            PlaneLightVector = Normalize(PlaneLightVector);
            PlaneLightVector2 = Normalize(PlaneLightVector2);
            Surface = Normalize(Surface);
            float PlaneDiffuseTerm = DotProduct(Normal,PlaneLightVector);
            float PlaneDiffuseTerm2 = DotProduct(Normal,PlaneLightVector2);

                if(PlaneDiffuseTerm > 0){
                    PixelColour = addcolour(flag,PlaneDiffuseTerm,PixelColour,blackOrWhite);
                }

                if(PlaneDiffuseTerm2 > 0){
                    PixelColour = addcolour(flag,PlaneDiffuseTerm2,PixelColour,blackOrWhite);
                }
                PixelColour = MultiplyScalar(PixelColour,0.5);
                SetColor(shade, PixelColour);

                 return shade;
        }

        float shadow(Vector3 planeintersection,Vector3 SphereCenter,float SphereRadius,Vector3 Light){

            Vector3 Camera = planeintersection;      //define a new camera point = ray-plane intersection point
            Vector3 NewDirection = Minus(Light,Camera);//ray-plane intersection point to light
            NewDirection = Normalize(NewDirection);
            Vector3 NewOC = Minus(Camera, SphereCenter);
            float NewD_Dot_OC = DotProduct(NewDirection,NewOC);
            float NewOCSqure = DotProduct(NewOC,NewOC);
            return (SphereRadius*SphereRadius) - NewOCSqure + NewD_Dot_OC * NewD_Dot_OC;

        }
};

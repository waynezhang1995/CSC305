#pragma once
#include "object.h"
#include "common.h"
#include <iostream>
class triangle : public Object
{
    Vector3 Vertax3;
    Vector3 Vertax2;
    Vector3 Vertax1;
    Vector3 TriangleNormal;
    Vector3 TriangleColor;
    int flag;
public:
    triangle(Vector3 Vertax3_in, Vector3 Vertax2_in,Vector3 Vertax1_in,int flag_in, Vector3 TriangleNormal_in,Vector3 TriangleColor_in)
    {
        Vertax3 = Vertax3_in;
        Vertax2 = Vertax2_in;
        Vertax1 = Vertax1_in;
        TriangleNormal = TriangleNormal_in;
        TriangleColor = TriangleColor_in;
        flag = flag_in;
    }
    virtual Vector3 getCenter(){return Vertax3;}
    virtual float getRadius(){}
    virtual int getflag() {return flag;}

    virtual bool Intersect(Vector3 Origin, Vector3 Direction,
                           float *t, Vector3 *normal)
    {
         Vector3 trianglenormal;
         trianglenormal = TriangleNormal;
        (*normal) = trianglenormal;
        Vector3 NminusP = Minus(Origin,Vertax3);  //(o-p)
        float tmp = -1 * DotProduct(NminusP,trianglenormal); //(o-p).n
        float D_N = DotProduct(Direction,trianglenormal); // d.n
        (*t) = tmp / D_N; // t
        //bool inside = false;
        Vector3 Intersectiontmp = MultiplyScalar(Direction, tmp / D_N);
        Intersectiontmp = Add(Intersectiontmp, Origin);
        if((*t) > 0 ){
            int line1,line2,line3;

                line1 = DotProduct(trianglenormal,CrossProduct(Minus(Vertax3,Vertax1),Minus(Intersectiontmp,Vertax1)));
                line2 = DotProduct(trianglenormal,CrossProduct(Minus(Vertax2,Vertax3),Minus(Intersectiontmp,Vertax3)));
                line3 = DotProduct(trianglenormal,CrossProduct(Minus(Vertax1,Vertax2),Minus(Intersectiontmp,Vertax2)));

            if(line1 >= 0 && line2 >= 0 && line3 >= 0){

                return true;
            }
            if(line1 <= 0 && line2 <= 0 && line3 <= 0){

                return true;
            }

        }else{
            return false;
        }
    }

    Vector3 addcolour(int flag,float PlaneDiffuseTerm,Vector3 PixelColour,int blackOrWhite){


        if(flag == 100){
            if (blackOrWhite== -1){
                Vector3 PixelDiffuseColour = MultiplyScalar(White, PlaneDiffuseTerm);
                PixelColour = Add(PixelColour,PixelDiffuseColour);
                //SetColor(shade, PixelColour);
            }else{
                Vector3 PixelDiffuseColour = MultiplyScalar(yellow, PlaneDiffuseTerm);
                PixelColour = Add(PixelColour,PixelDiffuseColour);
                //SetColor(shade, PixelColour);
            }
        }else{
            Vector3 PixelDiffuseColour = MultiplyScalar(TriangleColor, PlaneDiffuseTerm);
            PixelColour = Add(PixelColour,PixelDiffuseColour);
        }
        return PixelColour;
    }

    virtual Pixel DiffuseShade(int flag,Vector3 Direction,Vector3 Surface, Vector3 Normal,std::vector<Object *> pObjectList)
    {
             Pixel shade;
             Vector3 PixelColour = AmbientColour;
             int blackOrWhite = -1;
             float result = abs((int)Surface.x/50%2);
             float result1 = abs((int)Surface.z/50%2);

             if(result == result1  ){
                 blackOrWhite = 1;
             }
             if(Surface.x < 0 ){
                blackOrWhite = blackOrWhite * -1;
             }
             if(Surface.z < 0){
                 blackOrWhite = blackOrWhite * -1;
             }
             Vector3 PlaneLightVector =Minus(Light, Surface); //L
             Vector3 PlaneLightVector2 =Minus(Light2, Surface); //L
             PlaneLightVector = Normalize(PlaneLightVector);
             PlaneLightVector2 = Normalize(PlaneLightVector2);
             Vector3 tmp = Surface;
             Surface = Normalize(Surface);
             float PlaneDiffuseTerm = DotProduct(Normal,PlaneLightVector);
             float PlaneDiffuseTerm2 = DotProduct(Normal,PlaneLightVector2);
             for(int i =0;i<pObjectList.size();++i){     //calculate shadow for each sphere
                 if(pObjectList[i]->getflag() < 10 && flag != 96){
                    float NewDiscriminant = shadowSphere(tmp,pObjectList[i]->getCenter(),pObjectList[i]->getRadius(),Light);
                    float NewDiscriminant2 = shadowSphere(tmp,pObjectList[i]->getCenter(),pObjectList[i]->getRadius(),Light2);

                    if(NewDiscriminant > 0){  //intersect to one of the three spheres --> Blocked !
                        if(PlaneDiffuseTerm > 0){
                            PixelColour = addcolour(flag,PlaneDiffuseTerm,PixelColour,blackOrWhite);
                        }

                        if(PlaneDiffuseTerm2 > 0){
                            PixelColour = addcolour(flag,PlaneDiffuseTerm2,PixelColour,blackOrWhite);
                        }
                        PixelColour = MultiplyScalar(PixelColour,0.125);
                        SetColor(shade, PixelColour);
                        return shade;
                    }

                    if(NewDiscriminant2 > 0){  //intersect to one of the three spheres --> Blocked !
                        if(PlaneDiffuseTerm > 0){
                            PixelColour = addcolour(flag,PlaneDiffuseTerm,PixelColour,blackOrWhite);
                        }

                        if(PlaneDiffuseTerm2 > 0){
                            PixelColour = addcolour(flag,PlaneDiffuseTerm2,PixelColour,blackOrWhite);
                        }
                        PixelColour = MultiplyScalar(PixelColour,0.125);
                        SetColor(shade, PixelColour);
                        return shade;
                    }
                }
             }
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

        float shadowSphere(Vector3 planeintersection,Vector3 SphereCenter,float SphereRadius,Vector3 Light){

            Vector3 Camera = planeintersection;      //define a new camera point = ray-plane intersection point
            Vector3 NewDirection = Minus(Light,Camera);//ray-plane intersection point to light
            float len1 = sqrt(NewDirection.x*NewDirection.x+NewDirection.y*NewDirection.y+NewDirection.z*NewDirection.z);
            NewDirection = Normalize(NewDirection);
            Vector3 NewOC = Minus(Camera, SphereCenter);
            float len2 = sqrt(NewOC.x*NewOC.x+NewOC.y*NewOC.y+NewOC.z*NewOC.z);
            float NewD_Dot_OC = DotProduct(NewDirection,NewOC);
            float NewOCSqure = DotProduct(NewOC,NewOC);
            if(len2 > len1){
                return -1;
            }
            return (SphereRadius*SphereRadius) - NewOCSqure + NewD_Dot_OC * NewD_Dot_OC;
        }
};

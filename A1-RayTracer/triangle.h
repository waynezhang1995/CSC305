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
    float PlaneDiffuseTerm;


public:
    triangle(Vector3 Vertax3_in, Vector3 Vertax2_in,Vector3 Vertax1_in,int flag_in, Vector3 TriangleNormal_in,Vector3 TriangleColor_in)
    {
        Vertax3 = Vertax3_in;
        Vertax2 = Vertax2_in;
        Vertax1 = Vertax1_in;
        TriangleNormal = TriangleNormal_in;
        TriangleColor = TriangleColor_in;
        flag = flag_in;
        PlaneDiffuseTerm = 0;

    }
    virtual Vector3 getCenter(){return Vertax3;}
    virtual float getRadius(){}
    virtual int getflag() {return flag;}


    Vector3 randomVector(){

        float x = (-100) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100-(-100))));
        float z = (-100) + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(100-(-100))));
        //printf("x is %f",x);
        return Vector3(x, 0.0f, z);

    }

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

    Vector3 CalculatePlaneColour(int flag,Vector3 PixelColour,int blackOrWhite){
        if(PlaneDiffuseTerm > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm,PixelColour,blackOrWhite);
        }
         return PixelColour;
    }

    virtual Vector3 DiffuseShade(int flag,Vector3 Direction,Vector3 Surface, Vector3 Normal,std::vector<Object *> pObjectList)
    {
             Pixel shade;
             Vector3 PixelColour = AmbientColour;
             int blackOrWhite = -1;
             float result = abs((int)Surface.x/50%2);
             float result1 = abs((int)Surface.z/50%2);
             Vector3 Colour;
             if(result == result1  ){
                 blackOrWhite = 1;
             }
             if(Surface.x < 0 ){
                blackOrWhite = blackOrWhite * -1;
             }
             if(Surface.z < 0){
                 blackOrWhite = blackOrWhite * -1;
             }

             Vector3 randomV = randomVector();
             Vector3 NewLight(Light.x+randomV.x,900,Light.z+randomV.z);
             Vector3 PlaneLightVector =Minus(NewLight, Surface); //L
             PlaneLightVector = Normalize(PlaneLightVector);
             Vector3 V =Minus(Surface,Camera);
             V = Normalize(V);
             Vector3 tmp = Surface;
             PlaneDiffuseTerm = DotProduct(Normal,PlaneLightVector);
             Vector3 PlaneColour = CalculatePlaneColour(flag,PixelColour,blackOrWhite);

            /*********Shadow************/
             float NewDiscriminant1;
             Vector3 normal;
             Vector3 Direction1 =Minus(NewLight,tmp);
             Direction1 = Normalize(Direction1);

        if(flag >= 90){
             for(int i =0;i<pObjectList.size();++i){     //calculate shadow for each sphere
                 Vector3 SoftShadow1(0.0f,0.0f,0.0f);
                 int count = 0;
                 if(pObjectList[i]->getflag() < 90 && flag != 96 && pObjectList[i]->getflag()!= 52){

                     bool DoesIntersect1 = pObjectList[i]->Intersect(tmp, Direction1,
                                                                  &NewDiscriminant1, &normal);
                    if(DoesIntersect1){  //intersect to one of the three spheres --> Blocked !
                        SoftShadow1 = PlaneColour;
                        SoftShadow1 = MultiplyScalar(SoftShadow1,0.125);
                        return SoftShadow1;
                    }
                }
             }

        }
            /*******************************/
        if(flag > 10 && flag < 90 ){
            Pixel shade2;
            Vector3 Colour2;
            int index = 0;
            bool HasIntersection = false;
            float t_min = 999999;
            Vector3 Normal_min;
            Vector3 priRayDir = Direction;
            //priRayDir = MultiplyScalar(priRayDir,-1);
            Vector3 SecondaryRay = MultiplyScalar(Normal,2 * -DotProduct(Normal,priRayDir));
            SecondaryRay = Add(SecondaryRay,priRayDir);
            SecondaryRay = Normalize(SecondaryRay); //new direction

            for(int i =0;i<pObjectList.size();++i){
                if(pObjectList[i]->getflag() >= 90 || pObjectList[i]->getflag() <= 10 ){
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
                Colour2 = pObjectList[index]->DiffuseShade(pObjectList[index]->getflag(),SecondaryRay,Intersection, Normal_min,pObjectList);

            }//if t > 0

            else //No Intersection, set background colour
            {
                return BackgroundColor;
            }

            return Colour2;

            }else{ //if no shadow add colour
                return PlaneColour;
            }

        }
};

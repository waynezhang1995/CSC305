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
    float PlaneDiffuseTerm2;
    float PlaneDiffuseTerm3;
    float PlaneDiffuseTerm4;
    float PlaneDiffuseTerm5;
    float PlaneDiffuseTerm6;
    float PlaneDiffuseTerm7;
    float PlaneDiffuseTerm8;
    float PlaneDiffuseTerm9;

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
        PlaneDiffuseTerm2 = 0;
        PlaneDiffuseTerm3 = 0;
        PlaneDiffuseTerm4 = 0;
        PlaneDiffuseTerm5 = 0;
        PlaneDiffuseTerm6 = 0;
        PlaneDiffuseTerm7 = 0;
        PlaneDiffuseTerm8 = 0;
        PlaneDiffuseTerm9 = 0;


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

    Vector3 CalculatePlaneColour(int flag,Vector3 PixelColour,int blackOrWhite){
        if(PlaneDiffuseTerm > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm2 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm2,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm3 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm3,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm4 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm4,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm5 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm5,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm6 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm6,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm7 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm7,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm8 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm8,PixelColour,blackOrWhite);
        }
        if(PlaneDiffuseTerm9 > 0){
            PixelColour = addcolour(flag,PlaneDiffuseTerm9,PixelColour,blackOrWhite);
        }

        PixelColour = MultiplyScalar(PixelColour,1.0f/9.0f);
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
             Vector3 PlaneLightVector =Minus(Light, Surface); //L
             Vector3 PlaneLightVector2 =Minus(Light2, Surface); //L
             Vector3 PlaneLightVector3 =Minus(Light3, Surface); //L

             Vector3 PlaneLightVector4 =Minus(Light4, Surface); //L
             Vector3 PlaneLightVector5 =Minus(Light5, Surface); //L
             Vector3 PlaneLightVector6 =Minus(Light6, Surface); //L

             Vector3 PlaneLightVector7 =Minus(Light7, Surface); //L
             Vector3 PlaneLightVector8 =Minus(Light8, Surface); //L
             Vector3 PlaneLightVector9 =Minus(Light9, Surface); //L

             PlaneLightVector = Normalize(PlaneLightVector);
             PlaneLightVector2 = Normalize(PlaneLightVector2);
             PlaneLightVector3 = Normalize(PlaneLightVector3);

             PlaneLightVector4 = Normalize(PlaneLightVector4);
             PlaneLightVector5 = Normalize(PlaneLightVector5);
             PlaneLightVector6 = Normalize(PlaneLightVector6);

             PlaneLightVector7 = Normalize(PlaneLightVector7);
             PlaneLightVector8 = Normalize(PlaneLightVector8);
             PlaneLightVector9 = Normalize(PlaneLightVector9);

             Vector3 V =Minus(Surface,Camera);
             V = Normalize(V);
             Vector3 tmp = Surface;

              PlaneDiffuseTerm = DotProduct(Normal,PlaneLightVector);
              PlaneDiffuseTerm2 = DotProduct(Normal,PlaneLightVector2);
              PlaneDiffuseTerm3 = DotProduct(Normal,PlaneLightVector3);

              PlaneDiffuseTerm4 = DotProduct(Normal,PlaneLightVector4);
              PlaneDiffuseTerm5 = DotProduct(Normal,PlaneLightVector5);
              PlaneDiffuseTerm6 = DotProduct(Normal,PlaneLightVector6);

              PlaneDiffuseTerm7 = DotProduct(Normal,PlaneLightVector7);
              PlaneDiffuseTerm8 = DotProduct(Normal,PlaneLightVector8);
              PlaneDiffuseTerm9 = DotProduct(Normal,PlaneLightVector9);

              Vector3 PlaneColour = CalculatePlaneColour(flag,PixelColour,blackOrWhite);

            /*********Shadow************/
             float NewDiscriminant1;
             float NewDiscriminant2;
             float NewDiscriminant3;
             float NewDiscriminant4;
             float NewDiscriminant5;
             float NewDiscriminant6;
             float NewDiscriminant7;
             float NewDiscriminant8;
             float NewDiscriminant9;

             Vector3 normal;
             Vector3 Direction1 =Minus(Light,tmp);
             Vector3 Direction2 =Minus(Light2,tmp);
             Vector3 Direction3 =Minus(Light3,tmp);

             Vector3 Direction4 =Minus(Light4,tmp);
             Vector3 Direction5 =Minus(Light5,tmp);
             Vector3 Direction6 =Minus(Light6,tmp);

             Vector3 Direction7 =Minus(Light7,tmp);
             Vector3 Direction8 =Minus(Light8,tmp);
             Vector3 Direction9 =Minus(Light9,tmp);

             Direction1 = Normalize(Direction1);
             Direction2 = Normalize(Direction2);
             Direction3 = Normalize(Direction3);

             Direction4 = Normalize(Direction4);
             Direction5 = Normalize(Direction5);
             Direction6 = Normalize(Direction6);

             Direction7 = Normalize(Direction7);
             Direction8 = Normalize(Direction8);
             Direction9 = Normalize(Direction9);




        if(flag >= 90){
             for(int i =0;i<pObjectList.size();++i){     //calculate shadow for each sphere
                 Vector3 SoftShadow1(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow2(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow3(0.0f,0.0f,0.0f);

                 Vector3 SoftShadow4(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow5(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow6(0.0f,0.0f,0.0f);

                 Vector3 SoftShadow7(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow8(0.0f,0.0f,0.0f);
                 Vector3 SoftShadow9(0.0f,0.0f,0.0f);

                 if(pObjectList[i]->getflag() < 90 && flag != 96 && pObjectList[i]->getflag()!= 52){
                     int count = 0;

                     bool DoesIntersect1 = pObjectList[i]->Intersect(tmp, Direction1,
                                                                  &NewDiscriminant1, &normal);
                     bool DoesIntersect2 = pObjectList[i]->Intersect(tmp, Direction2,
                                                                  &NewDiscriminant2, &normal);
                     bool DoesIntersect3 = pObjectList[i]->Intersect(tmp, Direction3,
                                                                  &NewDiscriminant3, &normal);

                     bool DoesIntersect4 = pObjectList[i]->Intersect(tmp, Direction4,
                                                                  &NewDiscriminant4, &normal);
                     bool DoesIntersect5 = pObjectList[i]->Intersect(tmp, Direction5,
                                                                  &NewDiscriminant5, &normal);
                     bool DoesIntersect6 = pObjectList[i]->Intersect(tmp, Direction6,
                                                                  &NewDiscriminant6, &normal);

                     bool DoesIntersect7 = pObjectList[i]->Intersect(tmp, Direction7,
                                                                  &NewDiscriminant7, &normal);
                     bool DoesIntersect8 = pObjectList[i]->Intersect(tmp, Direction8,
                                                                  &NewDiscriminant8, &normal);
                     bool DoesIntersect9 = pObjectList[i]->Intersect(tmp, Direction9,
                                                                  &NewDiscriminant9, &normal);

                    if(DoesIntersect1){  //intersect to one of the three spheres --> Blocked !
                        SoftShadow1 = PlaneColour;
                        SoftShadow1 = MultiplyScalar(SoftShadow1,0.08);
                        count++;
                    }else{
                        SoftShadow1 = PlaneColour;
                    }

                    if(DoesIntersect2){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow2 = PlaneColour;
                        SoftShadow2 = MultiplyScalar(SoftShadow2,0.08);

                    }else{
                        SoftShadow2 = PlaneColour;
                    }
                    if(DoesIntersect3){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow3 = PlaneColour;
                        SoftShadow3 = MultiplyScalar(SoftShadow3,0.08);

                    }else{
                        SoftShadow3 = PlaneColour;
                    }
                    if(DoesIntersect4){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow4 = PlaneColour;
                        SoftShadow4 = MultiplyScalar(SoftShadow4,0.08);

                    }else{
                        SoftShadow4 = PlaneColour;
                    }
                    if(DoesIntersect5){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow5 = PlaneColour;
                        SoftShadow5 = MultiplyScalar(SoftShadow5,0.08);

                    }else{
                        SoftShadow5 = PlaneColour;
                    }
                    if(DoesIntersect6){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow6 = PlaneColour;
                        SoftShadow6 = MultiplyScalar(SoftShadow6,0.08);

                    }else{
                        SoftShadow6 = PlaneColour;
                    }
                    if(DoesIntersect7){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow7 = PlaneColour;
                        SoftShadow7 = MultiplyScalar(SoftShadow7,0.08);

                    }else{
                        SoftShadow7 = PlaneColour;
                    }
                    if(DoesIntersect8){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow8 = PlaneColour;
                        SoftShadow8 = MultiplyScalar(SoftShadow8,0.08);

                    }else{
                        SoftShadow8 = PlaneColour;
                    }
                    if(DoesIntersect9){  //intersect to one of the three spheres --> Blocked !
                        count++;
                        SoftShadow9 = PlaneColour;
                        SoftShadow9 = MultiplyScalar(SoftShadow9,0.08);

                    }else{
                        SoftShadow9 = PlaneColour;
                    }
                    if(count > 0){
                        Vector3 returnColour = Add(Add(Add(Add(Add(Add(Add(Add(SoftShadow1,SoftShadow2),SoftShadow3),SoftShadow4),SoftShadow5),SoftShadow6),SoftShadow7),SoftShadow8),SoftShadow9);
                        returnColour = MultiplyScalar(returnColour,1.0f/9.0f);
                        return returnColour;
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

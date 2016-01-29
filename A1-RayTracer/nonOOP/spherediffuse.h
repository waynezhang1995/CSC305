#pragma once
#include "Image.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include "variables.h"
#include "spherediscriminant.h"
#include "planediffuse.h"

void CalculateSphereDiffuse(Pixel & px , float t, Vector3 Direction,Vector3 Camerapoint,int flag){

    Vector3 priRayDir = Direction;
    priRayDir = MultiplyScalar(priRayDir,-1);
    Vector3 Intersection = MultiplyScalar(Direction, t);
    Intersection = Add(Intersection,Camerapoint); // vector from camera to sphere center
    Vector3 V =Minus(Intersection,Camerapoint);
    V = Normalize(V);
    Vector3 Normal1 = Minus(Intersection, SphereCenter1);
    Normal1 = Normalize(Normal1);
    Vector3 Normal2 = Minus(Intersection, SphereCenter2);
    Normal2 = Normalize(Normal2);
    Vector3 Normal3 = Minus(Intersection, SphereCenter3);
    Normal3 = Normalize(Normal3);

    Vector3 LightVector = Minus(Light, Intersection); //L
    LightVector = Normalize(LightVector);

    /* Diffuse Component */
    float DiffuseTerm1 = DotProduct(LightVector, Normal1); //cos(0)
    float DiffuseTerm2 = DotProduct(LightVector, Normal2); //cos(0)
    //float DiffuseTerm3 = DotProduct(LightVector, Normal3); //cos(0)

    Vector3 PixelColour = AmbientColour;

    /* put into another header file? necessary?*/
    if(flag == 1){

        /* Secondary ray: Sub light vector to -1*primary ray direction */
        /* Reflection part */
        Vector3 SecondaryRay = MultiplyScalar(Normal1,2 * DotProduct(Normal1,priRayDir));
        SecondaryRay = Normalize(SecondaryRay); //new direction
        Vector3 SecOC = Minus(Intersection,SphereCenter2); //OC
        float SecDiscriminant = CalculateDiscriminant(SecondaryRay,SecOC,SphereRadius2);
        float SecT = -1;
        if (SecDiscriminant > 0){
           SecT = CalculateT(SecDiscriminant,SecOC,SecondaryRay);
        }
        if(SecT > 0){
            CalculateSphereDiffuse(px,SecT,SecondaryRay,Intersection,2);
        }else{
            CalculatePlaneDiffuse(SecondaryRay,px,Intersection);
        }
        /*******************/
        /* Specular Component */
        /*
        Vector3 tmp = MultiplyScalar(Normal1,2 * DotProduct(Normal1,LightVector));   //2(N.L)N
        Vector3 R = Minus(tmp,LightVector);    //2(N.L)N - L
        R = Normalize(R);
        float rv = std::min(0.0f,DotProduct(R,V));  //(R.V)  must > 0
        //std::cout<<rv<<std::endl;

        if(rv < 0) rv = rv*-1; // > 1 = 1
        if(rv > 1) rv = 1;
        float specular = pow(rv,30);   //Specular Power ;
        if (DiffuseTerm1 > 0){
            Vector3 PixelDiffuseColour = MultiplyScalar(DiffuseColourSphere1, DiffuseTerm1);
            PixelColour = Add(PixelColour, PixelDiffuseColour); //ambient + diffuse
        }
        if(specular > 0){
            Vector3 PixelSpecularColour = MultiplyScalar(SpecularColourSphere1,specular);
            PixelColour = Add(PixelColour,PixelSpecularColour);  //ambient + diffuse + specular
        }
        */
    }

    /* Need optimize structure */
    if(flag == 2){
        Vector3 tmp = MultiplyScalar(Normal2,2 * DotProduct(Normal2,LightVector));   //2(N.L)N
        Vector3 R = Minus(tmp,LightVector);    //2(N.L)N - L
        R = Normalize(R);
        float rv = std::min(0.0f,DotProduct(R,V));  //(R.V)
        if(rv < 0) rv = rv*-1;
        float specular = pow(rv,30);   //TODO: doesnt work
        if (DiffuseTerm2 > 0){
            Vector3 PixelDiffuseColour = MultiplyScalar(DiffuseColourSphere2, DiffuseTerm2);
            PixelColour = Add(PixelColour, PixelDiffuseColour); //ambient + diffuse
        }
        if(specular > 0){
            Vector3 PixelSpecularColour = MultiplyScalar(SpecularColourSphere2,specular);
            PixelColour = Add(PixelColour,PixelSpecularColour);  //ambient + diffuse + specular
        }
         SetColor(px, PixelColour);
    }
    /*
    if(flag == 3){
        Vector3 tmp = MultiplyScalar(Normal3,2 * DotProduct(Normal3,LightVector));   //2(N.L)N
        Vector3 R = Minus(tmp,LightVector);    //2(N.L)N - L
        R = Normalize(R);
        float rv = std::min(0.0f,DotProduct(R,V));  //(R.V)  must > 0
        if(rv < 0) rv = rv*-1;
        float specular = pow(rv,30);   //Specular Power ;
        if (DiffuseTerm3 > 0){
            Vector3 PixelDiffuseColour = MultiplyScalar(DiffuseColourSphere3, DiffuseTerm3);
            PixelColour = Add(PixelColour, PixelDiffuseColour); //ambient + diffuse
        }
        if(specular > 0){
            Vector3 PixelSpecularColour = MultiplyScalar(SpecularColourSphere3,specular);
            PixelColour = Add(PixelColour,PixelSpecularColour);  //ambient + diffuse + specular
        }
    }
    */
    //SetColor(px, PixelColour);
}



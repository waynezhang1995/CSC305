#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>

using namespace Eigen;
using namespace std;

unsigned int width = 1024;
unsigned int height = 1024;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
float linelength = 0;
int timercount = 0;
Canvas canvas;
bool rotateCW = true; //Whether we're rotating in the clockwise direction
float rotateAngle = 0 ; //The angle the square currently rotated
float rotateAngle1 = M_PI * 0.5; //The angle the square currently rotated
float rotateSpeed = 0.05; //The speed of the rotation
float lastx = vppos_x;
float lasty = vppos_y;
float dis = 2.0;
Matrix4f perspective;           //perspective matrix
Matrix4f viewrot;                  //view matrix
Matrix4f view;
Matrix4f viewtmp;
float camerax = 0;
float cameray = 0;
float cameraz = 5;

void MouseMove(double x, double y)
{
   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
   float dx = vppos_x- lastx;
   float dy = vppos_y- lasty;

   if(leftButtonPressed == true){
       /*
        if(lastx < vppos_x){
            rotateAngle += rotateSpeed;
        }else if(lastx > vppos_x){
            rotateAngle -= rotateSpeed;
        }*/

        rotateAngle += rotateSpeed * -dx;

        /*
        if(lasty < vppos_y){
        rotateAngle1 += rotateSpeed;
       }else if(lasty > vppos_y){
           rotateAngle1 -= rotateSpeed;
       }*/

        rotateAngle1 += rotateSpeed * -dy;
   }
   if(rightButtonPressed == true){
        dis += dy*0.01;
   }
}

void MouseButton(MouseButtons mouseButton, bool press)
{
    //What to do with the keys?
    if (mouseButton == LeftButton)
    {
        if (press == true) leftButtonPressed = true;
        else leftButtonPressed = false;
    }
    if (mouseButton == RightButton)
    {
        if (press == true) rightButtonPressed = true;
        else rightButtonPressed = false;
    }
}

void KeyPress(char keychar)
{
    //A key is pressed! print a message
    fprintf(stderr, "The \"%c\" key is pressed!\n", keychar);
    if (keychar == 'S') rotateAngle += rotateSpeed ;
    if (keychar == 'A') rotateAngle1 += rotateSpeed;
}

void OnPaint()
{
    /*
     * n = 1
     * f = 100
     *
     *
     */

    canvas.Clear();
    //cout<<rotateAngle<<endl;
    Vector3f EysPos(dis*sin(rotateAngle1)*sin(rotateAngle),dis*cos(rotateAngle1),dis*sin(rotateAngle1)*cos(rotateAngle));       //0,0,4 initially
    Vector3f ViewUp(0,1,0);
    Vector3f GazeDir(0,0,0);
    Vector3f W = -(GazeDir-EysPos).normalized();
    Vector3f U =(ViewUp.cross(W)).normalized();
    Vector3f V = W.cross(U);

    viewtmp<<1,0,0,-EysPos.x(),
             0,1,0,-EysPos.y(),
             0,0,1,-EysPos.z(),
             0,0,0,1;

    viewrot<<U.x(),U.y(),U.z(),0,
             V.x(),V.y(),V.z(),0,
             W.x(),W.y(),W.z(),0,
             0,0,0,1;

    view = viewrot*viewtmp;
    std::vector<Vector4f> vecBuffer;
    vecBuffer.push_back(Vector4f(0.5,0.5,0.5,1)); //front topright
    vecBuffer.push_back(Vector4f(-0.5,0.5,0.5,1)); //front topleft
    vecBuffer.push_back(Vector4f(-0.5,-0.5,0.5,1)); //front bottomleft
    vecBuffer.push_back(Vector4f(0.5,-0.5,0.5,1));  //front bottomright

    vecBuffer.push_back(Vector4f(0.5,0.5,-0.5,1));
    vecBuffer.push_back(Vector4f(-0.5,0.5,-0.5,1));
    vecBuffer.push_back(Vector4f(-0.5,-0.5,-0.5,1));
    vecBuffer.push_back(Vector4f(0.5,-0.5,-0.5,1));
        //V*P*vertices
        for(int i =0;i<8;i++){
            vecBuffer[i] = view*vecBuffer[i];

            perspective<<1/vecBuffer[i].z(),0,0,0,
                         0,1/vecBuffer[i].z(),0,0,
                         0,0,0,1+50-50/vecBuffer[i].z(),
                         0,0,0,1;
            /*
            vecBuffer[i].x() = vecBuffer[i].x()*1.0/vecBuffer[i].z();
            vecBuffer[i].y() = vecBuffer[i].y()*1.0/vecBuffer[i].z();
            vecBuffer[i].z() = 1.0+50-50*1/vecBuffer[i].z();
            vecBuffer[i].w() = 1.0;
            */
            vecBuffer[i] = perspective * vecBuffer[i];
        }
    //draw line
    for(int i =0;i<3;i++){
         canvas.AddLine(vecBuffer[i].x(), vecBuffer[i].y(), vecBuffer[i+1].x(), vecBuffer[i+1].y());
    }
         canvas.AddLine(vecBuffer[0].x(), vecBuffer[0].y(), vecBuffer[3].x(), vecBuffer[3].y());

    for(int i =4;i<7;i++){
         canvas.AddLine(vecBuffer[i].x(), vecBuffer[i].y(), vecBuffer[i+1].x(), vecBuffer[i+1].y());
    }
         canvas.AddLine(vecBuffer[4].x(), vecBuffer[4].y(), vecBuffer[7].x(), vecBuffer[7].y());
    for(int i =0;i<4;i++){
         canvas.AddLine(vecBuffer[i].x(), vecBuffer[i].y(), vecBuffer[i+4].x(), vecBuffer[i+4].y());
    }
}

void OnTimer()
{
    if (rotateCW) rotateAngle -= rotateSpeed;
        else rotateAngle += rotateSpeed;
}

int main(int, char **){

    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    //canvas.SetTimer(0.1, OnTimer);//trigger OnTimer every 0.1 second
    canvas.Show(width, height, "Canvas Demo");
    return 0;
}




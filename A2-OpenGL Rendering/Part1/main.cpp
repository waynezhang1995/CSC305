#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>

using namespace Eigen;
using namespace std;
/* 1024 X 1204 */
unsigned int width = 512;
unsigned int height = 512;

float vppos_x = 0;//cursor position; x coordinate
float vppos_y = 0;//cursor position; y coordinate
bool leftButtonPressed = false;//whether left button is pressed
bool rightButtonPressed = false;//whether right button is pressed
Canvas canvas;
bool rotateCW = true; //Whether we're rotating in the clockwise direction
float rotateAngle = 0 ; //The angle the camera currently rotated
                        //The angle between cube center and camera in a spherical coordinate
float rotateAngle1 = M_PI * 0.5; //The angle the camera currently rotated
float rotateSpeed = 0.05; //The speed of the rotation
float lastx = vppos_x;//last cursor position; x coordinate
float lasty = vppos_y;//last cursor position; y coordiante
float dis = 2.0;//initail distance between camera and cube
Matrix4f perspective;//Mp
Matrix4f viewrot;//Mrot
Matrix4f view;//Mv
Matrix4f viewtmp;/*
                  * 1 0 0 −xe
                  * 0 1 0 −ye
                  * 0 0 1 −ze
                  * 0 0 0 1
                  */
Matrix4f Orth;
float camerax = 0;//camera postion; x
float cameray = 0;//camera postion; y
float cameraz = 5;//camera postion; z

void MouseMove(double x, double y)
{
   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
   float dx = vppos_x- lastx;//record the last cursor postion x
   float dy = vppos_y- lasty;//record the last cursor postion y

   if(leftButtonPressed == true){//left button
        rotateAngle += rotateSpeed * -dx;//rotate camera left or right (dx > 0 --> right; dx < 0 --> left )
        rotateAngle1 += rotateSpeed * -dy;//rotate camera up or down (dy > 0 --> up; dy < 0 --> down)
   }
   if(rightButtonPressed == true){//right button
        dis += dy*0.01;//move camera along the gaze direction (dis > 0 --> away; dis < 0 --> closer)
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

/*
void KeyPress(char keychar)
{
    //A key is pressed! print a message
    fprintf(stderr, "The \"%c\" key is pressed!\n", keychar);
    if (keychar == 'S') rotateAngle += rotateSpeed ;
    if (keychar == 'A') rotateAngle1 += rotateSpeed;
}
*/

/* draw square */
void OnPaint()
{
    canvas.Clear();//clear the canvas. draw new frame
    /* move camera in a spherical coordinate system*/
    Vector3f EysPos(dis*sin(rotateAngle1)*sin(rotateAngle),dis*cos(rotateAngle1),dis*sin(rotateAngle1)*cos(rotateAngle));//0,0,5 initially
    Vector3f ViewUp(0,1,0);//up vector in Mv
    Vector3f GazeDir(0,0,0);//gaze vector in Mv

    /*Derive a coordinate system with origin e and uvw basis*/
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

    Orth<<1,0,0,0,
          0,1,0,0,
          0,0,2/(-1-(-50)),-(-1-50)/(-1+50),
          0,0,0,1;

    view = viewrot*viewtmp;

    perspective<<1,0,0,0,
                 0,1,0,0,
                 0,0,-1-50/-1,50,
                 0,0,1/-1,0;
    Matrix4f Mvp = Orth * perspective * view;
    cout<<Mvp<<endl;
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
            Vector4f temp = Mvp * vecBuffer[i];
            vecBuffer[i] = temp / temp.w();
        }
    //draw line
        if(dis <= 50 && dis >=1){
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
}

void OnTimer()
{
    if (rotateCW) rotateAngle -= rotateSpeed;
        else rotateAngle += rotateSpeed;
}

int main(int, char **){

    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    //canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    //canvas.SetTimer(0.1, OnTimer);//trigger OnTimer every 0.1 second
    canvas.Show(width, height, "Canvas Demo");
    return 0;
}




#include "Canvas.h"
#include <math.h>

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
float rotateAngle = 0; //The angle the square currently rotated
float rotateAngle1 = 0; //The angle the square currently rotated
float rotateSpeed = 0.05; //The speed of the rotation
float lastx = vppos_x;
float lasty = vppos_y;
struct Vector4
{
    float x, y, z, w;
    Vector4() { x = y = z = w =  0; }
    Vector4(float x_in, float y_in,float z_in,float w_in)
    {
    x = x_in;
    y = y_in;
    z = z_in;
    w = w_in;
    }
};


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

        rotateAngle1 += rotateSpeed * -dx;

        /*
        if(lasty < vppos_y){
        rotateAngle1 += rotateSpeed;
       }else if(lasty > vppos_y){
           rotateAngle1 -= rotateSpeed;
       }*/

        rotateAngle += rotateSpeed * -dy;
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
    if (keychar == ' ') rotateCW = !rotateCW;


}

void DrawCross(float x_center, float y_center)
{
    canvas.AddLine(x_center - linelength,
                   y_center - linelength,
                   x_center + linelength,
                   y_center + linelength);

    canvas.AddLine(x_center - linelength,
                   y_center + linelength,
                   x_center + linelength,
                   y_center - linelength);
}

void DrawSquare(float x_center, float y_center)
{
    canvas.AddLine(x_center - linelength, y_center - linelength, x_center - linelength, y_center + linelength);
    canvas.AddLine(x_center - linelength, y_center + linelength, x_center + linelength, y_center + linelength);
    canvas.AddLine(x_center + linelength, y_center + linelength, x_center + linelength, y_center - linelength);
    canvas.AddLine(x_center + linelength, y_center - linelength, x_center - linelength, y_center - linelength);
}

void OnPaint()
{
    canvas.Clear();
    /*
    DrawCross(vppos_x, vppos_y);

    if (leftButtonPressed == true)
    {
        DrawSquare(vppos_x, vppos_y);
    }
    */
    std::vector<Vector4> vecBuffer;
    vecBuffer.push_back(Vector4(0.5,0.5,0.5,1)); //front topright
    vecBuffer.push_back(Vector4(-0.5,0.5,0.5,1)); //front topleft
    vecBuffer.push_back(Vector4(-0.5,-0.5,0.5,1)); //front bottomleft
    vecBuffer.push_back(Vector4(0.5,-0.5,0.5,1));  //front bottomright

    vecBuffer.push_back(Vector4(0.5,0.5,-0.5,1));
    vecBuffer.push_back(Vector4(-0.5,0.5,-0.5,1));
    vecBuffer.push_back(Vector4(-0.5,-0.5,-0.5,1));
    vecBuffer.push_back(Vector4(0.5,-0.5,-0.5,1));


        for(int i =0;i<8;i++){
            Vector4 afterrotate;
            afterrotate.x = vecBuffer[i].x;
            afterrotate.y = vecBuffer[i].y*cos(rotateAngle) - vecBuffer[i].z*sin(rotateAngle);
            afterrotate.z = vecBuffer[i].y*sin(rotateAngle) + vecBuffer[i].z*cos(rotateAngle);
            vecBuffer[i] = afterrotate;
        }

        for(int i =0;i<8;i++){
            Vector4 afterrotate;
            afterrotate.x = vecBuffer[i].x*cos(rotateAngle1) + vecBuffer[i].z*sin(rotateAngle1);
            afterrotate.y = vecBuffer[i].y;
            afterrotate.z = vecBuffer[i].x*(-sin(rotateAngle1)) + vecBuffer[i].z*cos(rotateAngle1);
            vecBuffer[i] = afterrotate;
        }

    for(int i =0;i<3;i++){
         canvas.AddLine(vecBuffer[i].x, vecBuffer[i].y, vecBuffer[i+1].x, vecBuffer[i+1].y);
    }
         canvas.AddLine(vecBuffer[0].x, vecBuffer[0].y, vecBuffer[3].x, vecBuffer[3].y);

    for(int i =4;i<7;i++){
         canvas.AddLine(vecBuffer[i].x, vecBuffer[i].y, vecBuffer[i+1].x, vecBuffer[i+1].y);
    }
         canvas.AddLine(vecBuffer[4].x, vecBuffer[4].y, vecBuffer[7].x, vecBuffer[7].y);
    for(int i =0;i<4;i++){
         canvas.AddLine(vecBuffer[i].x, vecBuffer[i].y, vecBuffer[i+4].x, vecBuffer[i+4].y);
    }


}

void OnTimer()
{


    if(leftButtonPressed == true){
    if (rotateCW) rotateAngle -= rotateSpeed;
        else rotateAngle += rotateSpeed;
    }
    if(rightButtonPressed == true){
    if (rotateCW) rotateAngle1 -= rotateSpeed;
        else rotateAngle1 += rotateSpeed;
    }


}

int main(int, char **){





    //Link the call backs
    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    //canvas.SetTimer(0.1, OnTimer);//trigger OnTimer every 0.1 second
    //Show Window

    canvas.Show(width, height, "Canvas Demo");
    return 0;
}




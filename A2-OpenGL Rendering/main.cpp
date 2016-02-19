/* YuWei(Wayne) Zhang*/

#include "Canvas.h"
#include <math.h>

unsigned int width = 1024;
unsigned int height = 1024;

float vppos_x = 0;
float vppos_y = 0;
bool leftButtonPressed = false;
float linelength = 0.5;
int timercount = 0;
Canvas canvas;
int aftervalue = 0;
bool rotateCW = true;
float rotateAngle = 0;
float rotateSpeed = 0.01;

struct Vector2
{
    float x,y;
    Vector2(float x_in,float y_in)
    {
        x = x_in; y = y_in;
    }
    Vector2(){x=y=0;}
};

std::vector <Vector2> SquareVertices;



void MouseMove(double x, double y)
{
   //the pointer has moved
   vppos_x = (float)(x) / 256 - 1;
   vppos_y = 1 - (float)(y) / 256;
}

void MouseButton(MouseButtons mouseButton, bool press)
{
    //What to do with the keys?
    //give position(croodinate) of mouse
    if (mouseButton == LeftButton)
    {
        if (press == true) leftButtonPressed = true;
        else leftButtonPressed = false;
    }
}

void KeyPress(char keychar)
{
    //A key is pressed! print a message
    fprintf(stderr, "The \"%c\" key is pressed!\n", keychar);
    if(keychar == 'S') {
       rotateSpeed = 2.0;
    }else if(keychar == 'A'){
       rotateSpeed = 0.1;
    }else{
      if(keychar == ' ') rotateCW = !rotateCW;
    }
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
    //-0.25  -0.25
    canvas.AddLine(x_center - linelength, y_center - linelength, x_center - linelength, y_center + linelength);
    canvas.AddLine(x_center - linelength, y_center + linelength, x_center + linelength, y_center + linelength);
    canvas.AddLine(x_center + linelength, y_center + linelength, x_center + linelength, y_center - linelength);
    canvas.AddLine(x_center + linelength, y_center - linelength, x_center - linelength, y_center - linelength);
}

void OnPaint()
{
    canvas.Clear();
    //DrawCross(vppos_x, vppos_y);

    //if (leftButtonPressed == true)
    //{
        DrawSquare(vppos_x, vppos_y);
    //}
    //DrawSquare(vppos_x, vppos_y);
    /*
    for(int i =0;i<4;i++){
        //Rotatetion transform
        Vector2 startPoint;
        startPoint.x = SquareVertices[i].x * cos(rotateAngle)
        - SquareVertices[i].y * sin(rotateAngle);
        startPoint.y = SquareVertices[i].x * sin(rotateAngle)
        + SquareVertices[i].y * cos(rotateAngle);
        Vector2 endPoint;
        endPoint.x = SquareVertices[i + 1].x * cos(rotateAngle)
        - SquareVertices[i + 1].y * sin(rotateAngle);
        endPoint.y = SquareVertices[i + 1].x * sin(rotateAngle)
        + SquareVertices[i + 1].y * cos(rotateAngle);
        canvas.AddLine(startPoint.x, startPoint.y, endPoint.x, endPoint.y);
    }
    */
}

void OnTimer()
{
    linelength = (float)(sin(timercount / 10.0) * 0.1 + 0.1);
    timercount ++;

    if(rotateCW) rotateAngle -= rotateSpeed;
    else rotateAngle += rotateSpeed;
}

int main(int, char **){
    SquareVertices.push_back(Vector2(0.5,0.5));
    SquareVertices.push_back(Vector2(0.5,-0.5));
    SquareVertices.push_back(Vector2(-0.5,-0.5));
    SquareVertices.push_back(Vector2(-0.5,0.5));
    SquareVertices.push_back(Vector2(0.5,0.5));

    //Link the call backs

    //canvas.SetMouseMove(MouseMove);
    //canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    //canvas.SetOnPaint(OnPaint);
    canvas.SetTimer(0.1, OnTimer);//trigger OnTimer every 0.1 second
    //Show Window

    DrawSquare(-0.25+aftervalue,-0.25+aftervalue);
    DrawSquare(0.25+aftervalue,0.25+aftervalue);
    canvas.AddLine(-0.75+aftervalue,0.25+aftervalue,-0.25+aftervalue,0.75+aftervalue);
    canvas.AddLine(0.25+aftervalue,0.25+aftervalue,0.75+aftervalue,0.75+aftervalue);
    canvas.AddLine(-0.75+aftervalue,-0.75+aftervalue,-0.25+aftervalue,-0.25+aftervalue);
    canvas.AddLine(0.25+aftervalue,-0.75+aftervalue,0.75+aftervalue,-0.25+aftervalue);

    //canvas.AddLine(-0.75,-0.75,);
    //canvas.AddLine(x_center - linelength, y_center + linelength, x_center + linelength, y_center + linelength);
    //canvas.AddLine(x_center + linelength, y_center + linelength, x_center + linelength, y_center - linelength);
    //canvas.AddLine(x_center + linelength, y_center - linelength, x_center - linelength, y_center - linelength);
    //AddLine(float x_start, float y_start, float x_end, float y_end)


    //DrawSquare(0,0.5);
    canvas.Show(width, height, "Canvas Demo");
    return 0;
}





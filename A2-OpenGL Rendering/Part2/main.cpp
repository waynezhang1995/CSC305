#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>
unsigned int width = 1024;
unsigned int height = 1024;

using namespace Eigen;
using namespace std;

Canvas canvas;
Matrix4f view;
Matrix4f perspective;
Matrix4f viewrot;
Matrix4f viewtmp;

const char * vshader_square = "\
#version 330 core \n \
in vec3 vpoint;\
uniform vec4 vicol0;\
uniform vec4 vicol1;\
uniform vec4 vicol2;\
uniform vec4 vicol3;\
\
void RotationMatrix(vec4 first,vec4 second,vec4 third, vec4 forth){\
    vec4 point = vec4(vpoint,1);\
    point = vi * point;\
    mat4 pes;\
    pes[0] = vec4(1/point.z,0,0,0);\
    pes[1] = vec4(0,1/point.z,0,0);\
    pes[2] = vec4(0,0,1+50-50/point.z,0);\
    pes[3] = vec4(0,0,0,1);\
    point = pes * point;\
    gl_Position = point;\
}\
void main(){\
RotationMatrix(vi);}";

const char * fshader_square = "\
#version 330 core \n \
out vec3 color;\
void main(){color = vec3(0,45,2);}";

const GLfloat vpoint[]={
        -0.5f,-0.5f,-0.5f, // triangle 1 : begin
        -0.5f,-0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f, // triangle 1 : end
         0.5f, 0.5f,-0.5f, // triangle 2 : begin
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f, // triangle 2 : end

         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
        -0.5f,-0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f,-0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f,-0.5f,
         0.5f,-0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
         0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f,-0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f,-0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
         0.5f,-0.5f, 0.5f

};

float rotateAngle = 0 ; //The angle the camera currently rotated
                        //The angle between cube center and camera in a spherical coordinate
float rotateAngle1 = M_PI * 0.5; //The angle the camera currently rotated
float RotatingSpeed = 0.02;

GLuint VertexArrayID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint RotationID = 0;
GLuint ViewIDcol0 = 0;
GLuint ViewIDcol1 = 0;
GLuint ViewIDcol2 = 0;
GLuint ViewIDcol3 = 0;


float camerax = 0;//camera postion; x
float cameray = 0;//camera postion; y
float cameraz = 5;//camera postion; z
float dis = 2.0;

void InitializeGL()
{
    //vertex Array Object
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //Vertex Buffer Object
    GLuint vertexBufferID;
    glGenBuffers(1,&vertexBufferID);    //actually contains the vertices of square
    glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vpoint),vpoint,GL_STATIC_DRAW);
    ProgramID = compile_shaders(vshader_square,fshader_square);
    glUseProgram(ProgramID);
    GLuint vpoint_id = glGetAttribLocation(ProgramID,"vpoint");
    glEnableVertexAttribArray(vpoint_id);
    glVertexAttribPointer(vpoint_id,3,GL_FLOAT,false,0,0);
    ViewIDcol0 = glGetUniformLocation(ProgramID,"vicol0");
    ViewIDcol1 = glGetUniformLocation(ProgramID,"vicol1");
    ViewIDcol2 = glGetUniformLocation(ProgramID,"vicol2");
    ViewIDcol3 = glGetUniformLocation(ProgramID,"vicol3");
}

void MouseMove(double x, double y)
{

}

void MouseButton(MouseButtons mouseButton, bool press)
{

}

void KeyPress(char keychar)
{

}

void OnPaint()
{
       glClear(GL_COLOR_BUFFER_BIT);
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

        view = viewrot*viewtmp;
    //context
    glUseProgram(ProgramID);
    glBindVertexArray(VertexArrayID);
    glUniform4f(ViewIDcol0,view.col(0).x(),view.col(0).y(),view.col(0).z(),view.col(0).w());
    glUniform4f(ViewIDcol1,view.col(0).x(),view.col(0).y(),view.col(0).z(),view.col(0).w());
    glUniform4f(ViewIDcol2,view.col(0).x(),view.col(0).y(),view.col(0).z(),view.col(0).w());
    glUniform4f(ViewIDcol3,view.col(0).x(),view.col(0).y(),view.col(0).z(),view.col(0).w());
    //glUniform4f(ViewIDcol0,);
    //glUniform1f(RotationID,RotationAngle2);       //how to deal with second angle
    /*
     * if leftbutton pressed --> RotationAngle
     * if rightbutton pressed --> RotationAngle2
     */
    glDrawArrays(GL_TRIANGLES,0,36);//12 triangles each one has 3 vertices
    glUseProgram(0);
    glBindVertexArray(0);

}

void OnTimer()
{
    rotateAngle += RotatingSpeed;
}

int main(int, char **){
    //Link the call backs
    canvas.SetMouseMove(MouseMove);
    canvas.SetMouseButton(MouseButton);
    canvas.SetKeyPress(KeyPress);
    canvas.SetOnPaint(OnPaint);
    //canvas.SetTimer(0.05, OnTimer);
    //Show Window
    canvas.Initialize(width, height, "OpenGL Intro Demo");
    //Do our initialization
    InitializeGL();
    canvas.Show();

    return 0;
}


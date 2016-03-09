#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>

unsigned int width = 512;
unsigned int height = 512;

using namespace Eigen;
using namespace std;

Canvas canvas;

const char * vshader_square = "\
#version 330 core \n \
in vec3 vpoint;\
uniform float dis;\
uniform float rotateAngle;\
uniform float rotateAngle1;\
\
void RotationMatrix(float dis, float rotateAngle, float rotateAngle1){\
   vec3 EysPos = vec3(dis*sin(rotateAngle1)*sin(rotateAngle),dis*cos(rotateAngle1),dis*sin(rotateAngle1)*cos(rotateAngle));\
   vec3 ViewUp = vec3(0,1,0);\
   vec3 GazeDir = vec3(0,0,0);\
   vec3 W = normalize(-(GazeDir-EysPos));\
   vec3 U = normalize(cross(ViewUp,W));\
   vec3 V = cross(W,U);\
    mat4 viewtemp;\
    viewtemp[0] = vec4(1,0,0,0);\
    viewtemp[1] = vec4(0,1,0,0);\
    viewtemp[2] = vec4(0,0,1,0);\
    viewtemp[3] = vec4(-EysPos.x,-EysPos.y,-EysPos.z,1);\
    mat4 viewrot;\
    viewrot[0] = vec4(U.x,V.x,W.x,0);\
    viewrot[1] = vec4(U.y,V.y,W.y,0);\
    viewrot[2] = vec4(U.z,V.z,W.z,0);\
    viewrot[3] = vec4(0,0,0,1);\
    mat4 view;\
    view = viewrot * viewtemp;\
    vec4 point = vec4(vpoint,1);\
    vec4 result = view * point;\
    mat4 pes;\
    pes[0] = vec4(1/result.z,0,0,0);\
    pes[1] = vec4(0,1/result.z,0,0);\
    pes[2] = vec4(0,0,1+50-50/result.z,0);\
    pes[3] = vec4(0,0,0,1);\
    gl_Position = pes * result;\
}\
void main(){\
RotationMatrix(dis,rotateAngle,rotateAngle1);}";

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
GLuint RotationID1 = 0;
GLuint disID = 0;

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
    disID = glGetUniformLocation(ProgramID,"dis");
    RotationID = glGetUniformLocation(ProgramID,"rotateAngle");
    RotationID1 = glGetUniformLocation(ProgramID,"rotateAngle1");

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

    glUseProgram(ProgramID);
    glBindVertexArray(VertexArrayID);
    glUniform1f(RotationID,rotateAngle);
    glUniform1f(RotationID1,rotateAngle1);
    glUniform1f(disID,dis);


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
    canvas.SetTimer(0.05, OnTimer);
    //Show Window
    canvas.Initialize(width, height, "OpenGL Intro Demo");
    //Do our initialization
    InitializeGL();
    canvas.Show();

    return 0;
}


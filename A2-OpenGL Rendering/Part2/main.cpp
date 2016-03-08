#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"

unsigned int width = 1024;
unsigned int height = 1024;

using namespace Eigen;

Canvas canvas;
Matrix4f view;
Matrix4f perspective;
Matrix4f viewrot;
Matrix4f viewtmp;

const char * vshader_square = "\
#version 330 core \n \
in vec3 vpoint;\
uniform float rotation;\
\
mat4 RotationMatrix(float rot){\
mat3 R = mat3(1);\
R[0][0] = cos(rot);\
R[0][1] = sin(rot);\
R[1][0] = -sin(rot);\
R[1][1] = cos(rot);\
return mat4(R);}\
void main(){\
gl_Position = vec4(vpoint,1);}";

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

float RotationAngle = 0;
float RotationAngle2 = 0;
float RotatingSpeed = 0.02;

GLuint VertexArrayID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint RotationID = 0;

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
    RotationID = glGetUniformLocation(ProgramID,"rotation");
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
       Vector3f EysPos(dis*sin(RotationAngle2)*sin(RotationAngle),dis*cos(RotationAngle2),dis*sin(RotationAngle2)*cos(RotationAngle));//0,0,5 initially
       Vector3f ViewUp(0,1,0);//up vector in Mv
       Vector3f GazeDir(0,0,0);//gaze vector in Mv
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
    glUniform1f(RotationID,RotationAngle);
    //glUniform1f(RotationID,RotationAngle2);       //how to deal with second angle
    /*
     * if leftbutton pressed --> RotationAngle
     * if rightbutton pressed --> RotationAngle2
     */
    glDrawArrays(GL_TRIANGLES,0,12*3);//12 triangles each one has 3 vertices
    glUseProgram(0);
    glBindVertexArray(0);

}

void OnTimer()
{
    RotationAngle += RotatingSpeed;
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


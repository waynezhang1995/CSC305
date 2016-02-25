#include "Canvas.h"
#include <math.h>

unsigned int width = 512;
unsigned int height = 512;

Canvas canvas;

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
gl_Position = RotationMatrix(rotation)*vec4(vpoint,1);}";

const char * fshader_square = "\
#version 330 core \n \
out vec3 color;\
void main(){color = vec3(0,45,2);}";

const GLfloat vpoint[]={
    -.5f,-.5f,0,
    0.5f,-.5f,0,
    -.5f,0.5f,0,
    0.5f,-0.5f,0,
    0.5f,0.5f,0,
    -0.5f,0.5f,0
};

float Rotation = 0;
float RotatingSpeed = 0.02;

GLuint VertexArrayID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint RotationID = 0;

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
    //context
    glUseProgram(ProgramID);
    glBindVertexArray(VertexArrayID);
    glUniform1f(RotationID,Rotation);
    glDrawArrays(GL_TRIANGLES,0,6);
    glUseProgram(0);
    glBindVertexArray(0);

}

void OnTimer()
{
    Rotation += RotatingSpeed;
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


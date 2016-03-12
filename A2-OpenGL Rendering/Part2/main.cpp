/*
 * Author: YuWei(Wayne) Zhang
 * V00805647
 */

#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>
#include "data.h"
#include "loadtexture.h"

unsigned int width = 1024;
unsigned int height = 1024;

using namespace Eigen;
using namespace std;

Canvas canvas;

float vppos_x = 0;//cursor position; x coordinate
float vppos_y = 0;//cursor position; y coordinate
bool leftButtonPressed = false;//whether left button is pressed
bool rightButtonPressed = false;//whether right button is pressed
float lastx = vppos_x;//last cursor position; x coordinate
float lasty = vppos_y;//last cursor position; y coordiante

//take inverse transpose
const char * vshader_square = "\
    #version 330 core \n \
    layout(location = 0) in vec3 vpoint;\
    out vec4 interPoint;\
    in vec2 vtexcoord;\
    out vec2 uv;\
    uniform mat4 UseMvp;\
    \
    void RotationMatrix(mat4 UseMvp){\
        vec4 temp = UseMvp * vec4(vpoint,1);\
        gl_Position = temp ;\
        interPoint = temp;\
        uv = vtexcoord;\
    }\
    void main(){\
    RotationMatrix(UseMvp);}";

const char * fshader_square = "\
    #version 330 core \n \
    out vec3 color;\
    in vec4 interPoint;\
    in vec2 uv;\
    uniform mat4 UseMvp;\
    uniform sampler2D tex;\
    void main(){\
    vec2 uv_center = vec2(0.5,0.5);\
    vec3 LightPos = vec3(0,0,1.0f);\
    vec4 Lp = UseMvp *vec4(LightPos,1);\
    vec4 LightDir = normalize(Lp - interPoint );\
    vec4 n = vec4(normalize(cross(dFdy(interPoint.xyz),dFdx(interPoint.xyz))),0);\
    vec3 tmp = (2*dot(n.xyz,LightDir.xyz)) * n.xyz;\
    vec3 R = normalize(tmp - LightDir.xyz);\
    float rv = max(0.0f,dot(R,normalize(-(interPoint.xyz))));\
    float specular = pow(rv,100);\
    float diffuseterm = max(dot(LightDir,n),0.0);\
        color = texture(tex,uv).rgb + texture(tex,uv).rgb * diffuseterm + vec3(1.0f,1.0f,1.0f) * specular;\
    }";



float rotateAngle = 0 ; //The angle the camera currently rotated
                        //The angle between cube center and camera in a spherical coordinate
float rotateAngle1 = M_PI * 0.5; //The angle the camera currently rotated
float RotatingSpeed = 0.02;

GLuint VertexArrayID = 0;
GLuint skyID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint MvpID = 0;

float dis = 5.0;

void InitializeGL()
{
    GLuint vertexBufferID;
    GLuint skyBufferID;


    ProgramID = compile_shaders(vshader_square,fshader_square);
    glUseProgram(ProgramID);
    MvpID = glGetUniformLocation(ProgramID,"UseMvp");

    glGenBuffers(1,&skyBufferID);
    glBindBuffer(GL_ARRAY_BUFFER,skyBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vskypoint),vskypoint,GL_DYNAMIC_DRAW);
    glGenVertexArrays(1,&skyID);
    glBindVertexArray(skyID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
    loadpng(ProgramID); //load png file

    glGenBuffers(1,&vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vpoint),vpoint,GL_STATIC_DRAW);
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,false,0,0);
    loadpng(ProgramID); //load png file

    glBindBuffer(GL_ARRAY_BUFFER,0);
    glBindVertexArray(0);

    glEnable (GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);

}

void MouseMove(double x, double y)
{
    vppos_x = (float)(x) / 256 - 1;
    vppos_y = 1 - (float)(y) / 256;
    float dx = vppos_x- lastx;//record the last cursor postion x
    float dy = vppos_y- lasty;//record the last cursor postion y

    if(leftButtonPressed == true){//left button
         rotateAngle += RotatingSpeed * -dx;//rotate camera left or right (dx > 0 --> right; dx < 0 --> left )
         //rotateAngle1 += RotatingSpeed * -dy;//rotate camera up or down (dy > 0 --> up; dy < 0 --> down)
    }
    if(rightButtonPressed == true){//right button
         dis += dy*0.01;//move camera along the gaze direction (dis > 0 --> away; dis < 0 --> closer)
    }
}

void MouseButton(MouseButtons mouseButton, bool press)
{
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

}

void OnPaint()
{
    /******************Setting up Mvp***********************/
    Matrix4f Mvp;
    Matrix4f viewtmp;
    Matrix4f viewrot;
    Matrix4f view;  //Mv
    Matrix4f Orth;  //Mo
    Matrix4f perspective;//Mp
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

    perspective<<1,0,0,0,
                 0,1,0,0,
                 0,0,-1-20/-1,20,
                 0,0,1/-1,0;

    Orth<<1,0,0,0,
          0,1,0,0,
          0,0,2/(-1-(-20)),-(-1-20)/(-1+20),
          0,0,0,1;

    Mvp = Orth * perspective * view;
    /*******************End**********************/

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(ProgramID);
    glBindVertexArray(VertexArrayID);
    glUniformMatrix4fv(MvpID,1,GL_FALSE,Mvp.data());
    glDrawArrays(GL_TRIANGLES,0,36);//12 triangles each one has 3 vertices
    glClear(GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(skyID);
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


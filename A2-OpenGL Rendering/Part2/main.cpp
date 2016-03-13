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
    in vec3 vpoint;\
    out vec4 interPoint;\
    in vec2 vtexcoord;\
    in float CubeID; \
    out vec2 uv;\
    uniform mat4 UseMvp;\
    uniform mat4 SmallerCube;\
    uniform float rot;\
    \
    void RotationMatrix(mat4 UseMvp){\
        if(CubeID == 0){ \
            vec4 temp = UseMvp * vec4(vpoint,1);\
            gl_Position = temp ;\
            interPoint = temp;\
            uv = vtexcoord;\
        } \
        else { \
          mat4 R = mat4(1);\
              R[0][0] =  cos(rot);\
              R[0][1] =  sin(rot);\
              R[1][0] = -sin(rot);\
              R[1][1] =  cos(rot);\
             \
              vec4 temp =  UseMvp * SmallerCube * vec4(vpoint,1);\
              gl_Position = R * temp ;\
              interPoint = temp;\
              uv = vtexcoord;\
        }\
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
float rot = 0;
GLuint VertexArrayID = 0;
GLuint skyID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint MvpID = 0;
GLuint rotID = 0;

float dis = 6.0;

void InitializeGL()
{
    ProgramID = compile_shaders(vshader_square,fshader_square);
    glUseProgram(ProgramID);
    MvpID = glGetUniformLocation(ProgramID,"UseMvp");
    rotID = glGetUniformLocation(ProgramID,"rot");
    glGenVertexArrays(1,&VertexArrayID);
    glBindVertexArray(VertexArrayID);

    GLuint vertexBufferID;
    glGenBuffers(1,&vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER,vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vpoint),vpoint,GL_STATIC_DRAW);

    GLuint vpointid = glGetAttribLocation(ProgramID, "vpoint");
    glEnableVertexAttribArray(vpointid);
    glVertexAttribPointer(vpointid,3,GL_FLOAT,false,0,0);

    GLuint cubeBufferID;
    glGenBuffers(1,&cubeBufferID);
    glBindBuffer(GL_ARRAY_BUFFER,cubeBufferID);
    glBufferData(GL_ARRAY_BUFFER,sizeof(CubeID),CubeID,GL_STATIC_DRAW);

    GLuint cube_id = glGetAttribLocation(ProgramID, "CubeID");
    glEnableVertexAttribArray(cube_id);
    glVertexAttribPointer(cube_id, 1, GL_FLOAT, false, 0, 0);

    glClearDepth(0.0f);

    //glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    Matrix4f SmallerCube;
    SmallerCube << 0.5, 0, 0, 2.0,
                   0, 0.5, 0, 0,
                   0, 0, 0.5, 0,
                   0, 0, 0, 1;

    glUniformMatrix4fv(glGetUniformLocation(ProgramID, "SmallerCube"), 1, false, SmallerCube.data());

    Texture teximage = LoadPNGTexture("texture.png");

    GLuint texobject;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &texobject);
    glBindTexture(GL_TEXTURE_2D, texobject);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, teximage.width,
    teximage.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
    teximage.dataptr);
    GLuint tex_bindingpoint = glGetUniformLocation(ProgramID, "tex");
    glUniform1i(tex_bindingpoint, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texobject);

    GLuint texcoordbuffer;
    glGenBuffers(1, &texcoordbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, texcoordbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vtexcoord), vtexcoord,
    GL_STATIC_DRAW);
    GLuint texcoordBindingPosition = glGetAttribLocation(ProgramID,
    "vtexcoord");
    glEnableVertexAttribArray(texcoordBindingPosition);
    glVertexAttribPointer(texcoordBindingPosition, 2, GL_FLOAT,
    GL_FALSE, 0, (void *)0);
}

void MouseMove(double x, double y)
{
    vppos_x = (float)(x) / 256 - 1;
    vppos_y = 1 - (float)(y) / 256;
    float dx = vppos_x- lastx;//record the last cursor postion x
    float dy = vppos_y- lasty;//record the last cursor postion y

    if(leftButtonPressed == true){//left button
         rotateAngle += RotatingSpeed * -dx;//rotate camera left or right (dx > 0 --> right; dx < 0 --> left )
         rotateAngle1 += RotatingSpeed * -dy;//rotate camera up or down (dy > 0 --> up; dy < 0 --> down)
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
                 0,0,-1-50/-1,50,
                 0,0,1/-1,0;

    Orth<<1,0,0,0,
          0,1,0,0,
          0,0,2/(-1-(-50)),-(-1-50)/(-1+50),
          0,0,0,1;

    Mvp = Orth * perspective * view;
    /*******************End**********************/




    glUseProgram(ProgramID);
    glBindVertexArray(VertexArrayID);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);


    glUniformMatrix4fv(MvpID,1,GL_FALSE,Mvp.data());
    glUniform1f(rotID,rot);
    glDrawArrays(GL_TRIANGLES,0,72);//12 triangles each one has 3 vertices

    glUseProgram(0);
    glBindVertexArray(0);

}

void OnTimer()
{

    rot += RotatingSpeed;
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


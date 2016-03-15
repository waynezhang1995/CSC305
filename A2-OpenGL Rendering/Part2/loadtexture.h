
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

const char * vshader_square = "\
#version 330 core \n \
in vec3 vnormal;\
in vec3 vpoint;\
out vec4 interPoint;\
in vec2 vtexcoord;\
in float CubeID; \
out vec2 uv;\
out vec3 normal;\
out float id;\
uniform mat4 UseMvp;\
uniform mat4 SmallerCube;\
uniform float rot;\
uniform float spin;\
\
void RotationMatrix(mat4 UseMvp){\
if(CubeID == 0){ \
vec4 temp = UseMvp * vec4(vpoint,1);\
gl_Position = temp ;\
interPoint = temp;\
uv = vtexcoord;\
normal = vnormal;\
id = CubeID;\
}\
else if(CubeID == 1){ \
mat4 R = mat4(1);\
R[3][0] = -7.0*sin(3.141592653*0.5)*sin(rot);\
R[3][1] = -7.0*cos(3.141592653*0.5);\
R[3][2] = -7.0*sin(3.141592653*0.5)*cos(rot);\
mat4 S = mat4(1);\
S[0][0] =  cos(spin);\
S[2][0] =  sin(spin);\
S[0][2] = -sin(spin);\
S[2][2] =  cos(spin);\
vec4 temp = UseMvp * SmallerCube * R * S* vec4(vpoint,1);\
gl_Position =temp ;\
interPoint = temp;\
normal = vnormal;\
uv = vtexcoord;\
id = CubeID;\
}else{\
mat4 sky = mat4(100);\
sky[3][3] = 1;\
vec4 temp = UseMvp * sky * vec4(vpoint,1);\
gl_Position =temp ;\
interPoint = temp;\
uv = vtexcoord;\
normal = vnormal;\
id = CubeID;\
}\
}\
void main(){\
RotationMatrix(UseMvp);}";

        const char * fshader_square = "\
        #version 330 core \n \
        out vec3 color;\
        in vec4 interPoint;\
        in vec2 uv;\
        in vec3 normal;\
        in float id;\
        uniform mat4 UseMvp;\
        uniform sampler2D tex;\
        uniform vec3 EyeP;\
        uniform sampler2D bg;\
        void main(){\
        vec2 uv_center = vec2(0.5,0.5);\
        vec3 LightPos = vec3(0,0,1.0f);\
        vec4 Lp = vec4(LightPos,1);\
        vec4 LightDir = normalize(interPoint - Lp);\
        vec4 n = vec4(normalize(cross(dFdy(interPoint.xyz),dFdx(interPoint.xyz))),0);\
        vec3 tmp = (2*dot(n.xyz,LightDir.xyz)) * n.xyz;\
        vec3 R = normalize(tmp - LightDir.xyz);\
        float rv = max(0.0f,dot(R,normalize((interPoint.xyz))));\
        float specular = pow(rv,100);\
        float diffuseterm = max(dot(LightDir,n),0);\
        if(id == 0 || id == 1){\
        \
            if(specular > 0 ){\
            color = texture(tex,uv).rgb/10 + texture(tex,uv).rgb * diffuseterm * 2 + vec3(1.0f,1.0f,1.0f) * specular;\
            }else{\
            color = texture(tex,uv).rgb/10 + texture(tex,uv).rgb * diffuseterm * 2;\
            }\
        \
        }else{\
        color = texture(tex,uv).rgb;\
        }\
        }";



float rotateAngle = 0 ; //The angle the camera currently rotated
                        //The angle between cube center and camera in a spherical coordinate
float rotateAngle1 = M_PI * 0.5; //The angle the camera currently rotated
float RotatingSpeed = 0.01;
float rot = M_PI * 0.5;
float spin = 0;
float far = -100.0f;
float near = -1.0f;
GLuint VertexArrayID = 0;
GLuint skyID = 0;
GLuint ProgramID = 0;//the program we wrote
GLuint MvpID = 0;
GLuint rotID = 0;
GLuint spinID = 0;
GLuint EyeID = 0;

float dis = 3.0;

void InitializeGL()
{
        ProgramID = compile_shaders(vshader_square,fshader_square);
        glUseProgram(ProgramID);
        MvpID = glGetUniformLocation(ProgramID,"UseMvp");
        rotID = glGetUniformLocation(ProgramID,"rot");
        spinID = glGetUniformLocation(ProgramID,"spin");
        EyeID = glGetUniformLocation(ProgramID,"EyeP");

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

        float vnormal[36*2*3];
        for(int i =0; i<36*2*3;i+=9){
            Vector3f first = Vector3f(vpoint[i],vpoint[i+1],vpoint[i+2]);
            Vector3f second = Vector3f(vpoint[i+3],vpoint[i+4],vpoint[i+5]);
            Vector3f third = Vector3f(vpoint[6],vpoint[i+7],vpoint[i+8]);
            Vector3f tmp1 =  ((second - first).cross(third - second)).normalized();
            vnormal[i] = tmp1.x();
            vnormal[i+1] = tmp1.y();
            vnormal[i+2] = tmp1.z();
            vnormal[i+3] = tmp1.x();
            vnormal[i+4] = tmp1.y();
            vnormal[i+5] = tmp1.z();
            vnormal[i+6] = tmp1.x();
            vnormal[i+7] = tmp1.y();
            vnormal[i+8] = tmp1.z();

        }
        /* normal vertex attibute */
           GLuint normalbuffer;
           glGenBuffers(1, &normalbuffer);
           glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
           glBufferData(GL_ARRAY_BUFFER, sizeof(vnormal), vnormal,
           GL_STATIC_DRAW);
           GLuint normalBindingPosition = glGetAttribLocation(ProgramID,
           "vnormal");
           glEnableVertexAttribArray(normalBindingPosition);
           glVertexAttribPointer(normalBindingPosition, 3, GL_FLOAT,
           GL_FALSE, 0, (void *)0);

        glClearDepth(0.0f);
        glDepthFunc(GL_GREATER);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        Matrix4f SmallerCube;
        SmallerCube << 0.2, 0, 0, 0,
        0, 0.2, 0, 0,
        0, 0, 0.2, 0,
        0, 0, 0, 1;

        glUniformMatrix4fv(glGetUniformLocation(ProgramID, "SmallerCube"), 1, false, SmallerCube.data());

        GLuint texobject;
        Texture teximage = LoadPNGTexture("texture.png");
        Texture teximageBG = LoadPNGTexture("textureBG.png");

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
        glActiveTexture(GL_TEXTURE0 + 0);

        glBindTexture(GL_TEXTURE_2D, texobject);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA, teximageBG.width,
                     teximageBG.height, 1, GL_RGBA, GL_UNSIGNED_BYTE,
                     teximageBG.dataptr);
        GLuint tex_bindingpointBG = glGetUniformLocation(ProgramID, "bg");
        glUniform1i(tex_bindingpointBG, 1);
        glActiveTexture(GL_TEXTURE0+1);

        //glBindTexture(GL_TEXTURE_2D, texobject);
        /************************/


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
         dis += dy*0.05;//move camera along the gaze direction (dis > 0 --> away; dis < 0 --> closer)
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
       0,0,(near+far)/near,-far,
       0,0,1/near,0;

       Orth<<1,0,0,0,
       0,1,0,0,
       0,0,2/(near-far),-(near+far)/(near-far),
       0,0,0,1;

       Mvp = Orth * perspective * view;
       /*******************End**********************/

       glUseProgram(ProgramID);
       glBindVertexArray(VertexArrayID);
       glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       glEnable(GL_DEPTH_TEST);


       glUniformMatrix4fv(MvpID,1,GL_FALSE,Mvp.data());
       glUniform1f(rotID,rot);
       glUniform1f(spinID,spin);
       glUniform3f(EyeID,EysPos.x(),EysPos.y(),EysPos.z());
       glDrawArrays(GL_TRIANGLES,0,36*3);//12 triangles each one has 3 vertices

       glUseProgram(0);
       glBindVertexArray(0);

}

void OnTimer()
{
    glClearDepth(0.0f);

    spin += RotatingSpeed * 8;
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



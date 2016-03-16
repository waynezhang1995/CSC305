#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>

unsigned int width = 1024; //window width
unsigned int height = 1024; //window height

using namespace Eigen;
using namespace std;

Canvas canvas;

float vppos_x = 0;//cursor position; x coordinate
float vppos_y = 0;//cursor position; y coordinate
bool leftButtonPressed = false;//whether left button is pressed
bool rightButtonPressed = false;//whether right button is pressed
float lastx = vppos_x;//last cursor position; x coordinate
float lasty = vppos_y;//last cursor position; y coordiante
float rotateAngle = 0 ; //The angle the camera currently rotated
                        //The angle between cube center and camera in a spherical coordinate
float rotateAngle1 = M_PI * 0.5; //The angle the camera currently rotated
float RotatingSpeed = 0.01; //rotate speed. Both camera and smaller cube share the same rotation speed
float rot = M_PI * 0.5; //smaller cube orbiting angle.
float spin = 0; //smaller cube self-rotating angle
float far = -100.0f; //far plane
float near = -1.0f; //near plane
GLuint VertexArrayID = 0; //vertex array buffer ID
GLuint ProgramID = 0;//the program we wrote
GLuint MvpID = 0; //Mvp matrix ID
GLuint rotID = 0; //rotation ID
GLuint spinID = 0; //spin ID
GLuint EyeID = 0; //Eye position ID
GLuint tex_bindingpoint; //first texture binding point
GLuint tex_bindingpointBG; //skybox texture binding point
GLuint texobject; //cube texture
GLuint texobjectBG; //skybox texture
float dis = 3.0; //distance between camera and object

const GLfloat vpoint[]={

    -0.5f ,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,

    -0.5f,-0.5f,-0.5f, // triangle 1 : beg
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
     //back
     0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end

     0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
     //bottom
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,

     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    //#7 frontal
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //right
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,

     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //top
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,

    -0.5f ,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,

    -0.5f,-0.5f,-0.5f, // triangle 1 : beg
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
     //back
     0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end

     0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
     //bottom
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,

     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    //#7 frontal
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //right
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,

     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //top
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,

    -0.5f ,-0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,

    -0.5f,-0.5f,-0.5f, // triangle 1 : beg
    -0.5f,-0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f, // triangle 1 : end
     //back
     0.5f, 0.5f,-0.5f, // triangle 2 : begin
    -0.5f,-0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f, // triangle 2 : end

     0.5f, 0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,
    -0.5f,-0.5f,-0.5f,
     //bottom
     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
     0.5f,-0.5f,-0.5f,

     0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
    -0.5f,-0.5f,-0.5f,
    //#7 frontal
    -0.5f, 0.5f, 0.5f,
    -0.5f,-0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //right
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f,-0.5f,

     0.5f,-0.5f,-0.5f,
     0.5f, 0.5f, 0.5f,
     0.5f,-0.5f, 0.5f,
     //top
     0.5f, 0.5f, 0.5f,
     0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f,-0.5f,

     0.5f, 0.5f, 0.5f,
    -0.5f, 0.5f,-0.5f,
    -0.5f, 0.5f, 0.5f,
};


const GLfloat CubeID[] = {

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2
};



const GLfloat vtexcoord[] = {

    //LEFT
    1.0f/2, 2.0f/3,
    1.0f/4, 1.0f,
    1.0f/2, 1.0f, //upper half of the square

    1.0f/2, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0, //upper half of the square
    //back
    3.0f/4, 1.0f/3,
    1.0f/2, 2.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    3.0f/4, 1.0f/3,
    1.0f/2, 1.0f/3,
    1.0f/2, 2.0f/3,
    //bottom
    1.0f/4, 1.0f/3,
    1.0f/2, 2.0f/3,
    1.0f/2, 1.0f/3, //upper half of the square

    1.0f/4, 1.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/2, 2.0f/3,
    //frontal
    0, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square

    0, 1.0f/3,
    0, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square
    //right
    1.0f/4, 0,
    1.0f/2, 1.0f/3,
    1.0f/2, 0, //upper half of the square


    1.0f/2, 1.0f/3,
    1.0f/4, 0,
    1.0f/4, 1.0f/3, //upper half of the square
    //top
    1, 1.0f/3,
    3.0f/4, 1.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    1, 1.0f/3,
    3.0f/4, 2.0f/3,
    1, 2.0f/3, //upper half of the square
    //left
    //LEFT
    1.0f/2, 2.0f/3,
    1.0f/4, 1.0f,
    1.0f/2, 1.0f, //upper half of the square

    1.0f/2, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0, //upper half of the square
    //back
    3.0f/4, 1.0f/3,
    1.0f/2, 2.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    3.0f/4, 1.0f/3,
    1.0f/2, 1.0f/3,
    1.0f/2, 2.0f/3,
    //bottom
    1.0f/2, 2.0f/3,
    1.0f/4, 1.0f/3,
    1.0f/4, 2.0f/3, //upper half of the square

    1.0f/2, 2.0f/3,
    1.0f/2, 1.0f/3,
    1.0f/4, 1.0f/3,
    //frontal
    0, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square

    0, 1.0f/3,
    0, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square
    //right
    1.0f/4, 0,
    1.0f/2, 1.0f/3,
    1.0f/2, 0, //upper half of the square


    1.0f/2, 1.0f/3,
    1.0f/4, 0,
    1.0f/4, 1.0f/3, //upper half of the square
    //top
    1, 1.0f/3,
    3.0f/4, 1.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    1, 1.0f/3,
    3.0f/4, 2.0f/3,
    1, 2.0f/3, //upper half of the square


    //LEFT
    1.0f/2, 2.0f/3,
    1.0f/4, 1.0f,
    1.0f/2, 1.0f, //upper half of the square

    1.0f/2, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0, //upper half of the square
    //back
    3.0f/4, 1.0f/3,
    1.0f/2, 2.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    3.0f/4, 1.0f/3,
    1.0f/2, 1.0f/3,
    1.0f/2, 2.0f/3,
    //bottom
    1.0f/2, 2.0f/3,
    1.0f/4, 1.0f/3,
    1.0f/4, 2.0f/3, //upper half of the square

    1.0f/2, 2.0f/3,
    1.0f/2, 1.0f/3,
    1.0f/4, 1.0f/3,
    //frontal
    0, 2.0f/3,
    1.0f/4, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square

    0, 1.0f/3,
    0, 2.0f/3,
    1.0f/4, 1.0f/3, //upper half of the square
    //right
    1.0f/4, 0,
    1.0f/2, 1.0f/3,
    1.0f/2, 0, //upper half of the square


    1.0f/2, 1.0f/3,
    1.0f/4, 0,
    1.0f/4, 1.0f/3, //upper half of the square
    //top
    1, 1.0f/3,
    3.0f/4, 1.0f/3,
    3.0f/4, 2.0f/3, //upper half of the square

    1, 1.0f/3,
    3.0f/4, 2.0f/3,
    1, 2.0f/3, //upper half of the square
}; //lower half of the square

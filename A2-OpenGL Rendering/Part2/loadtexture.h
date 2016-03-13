#include "Canvas.h"
#include <math.h>
#include "Eigen/Dense"
#include <iostream>


void loadpng(GLuint ProgramID){

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

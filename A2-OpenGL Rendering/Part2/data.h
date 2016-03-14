

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
};

const GLfloat vnormal [] =  {
    -1.0f,0,0,
    -1.0f,0,0,
    -1.0f,0,0,

    0,0,-1.0f,
    0,0,-1.0f,
    0,0,-1.0f,

    0,-1.0f,0,
    0,-1.0f,0,
    0,-1.0f,0,

    0,0,1.0f,
    0,0,1.0f,
    0,0,1.0f,

    1.0f,0,0,
    1.0f,0,0,
    1.0f,0,0,

    0,1.0f,0,
    0,1.0f,0,
    0,1.0f,0,

    -1.0f,0,0,
    -1.0f,0,0,
    -1.0f,0,0,

    0,0,-1.0f,
    0,0,-1.0f,
    0,0,-1.0f,

    0,-1.0f,0,
    0,-1.0f,0,
    0,-1.0f,0,

    0,0,1.0f,
    0,0,1.0f,
    0,0,1.0f,

    1.0f,0,0,
    1.0f,0,0,
    1.0f,0,0,

    0,1.0f,0,
    0,1.0f,0,
    0,1.0f,0,

};

const GLfloat CubeID[] = {

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};



const GLfloat vtexcoord[] = {
    //left
    1, 0,
    0, 1,
    0, 0, //upper half of the square

    1, 0,
    1, 1,
    0, 1, //upper half of the square
    //back
    1, 0,
    0, 1,
    1, 1, //upper half of the square

    1, 0,
    0, 0,
    0, 1, //upper half of the square
    //bottom
    1, 1,
    0, 0,
    0, 1, //upper half of the square

    1, 1,
    1, 0,
    0, 0, //upper half of the square
    //frontal
    0, 0,
    1, 0,
    1, 1, //upper half of the square

    0, 1,
    0, 0,
    1, 1, //upper half of the square
    //right
    0, 0,
    1, 1,
    0, 1, //upper half of the square

    1, 1,
    0, 0,
    1, 0, //upper half of the square
    //top
    1, 1,
    0, 1,
    0, 0, //upper half of the square

    1, 1,
    0, 0,
    1, 0, //upper half of the square

    1, 0,
    0, 1,
    0, 0, //upper half of the square

    1, 0,
    1, 1,
    0, 1, //upper half of the square
    //back
    1, 0,
    0, 1,
    1, 1, //upper half of the square

    1, 0,
    0, 0,
    0, 1, //upper half of the square

    //bottom
    1, 1,
    0, 0,
    0, 1, //upper half of the square

    1, 1,
    1, 0,
    0, 0, //upper half of the square
    //frontal
    0, 0,
    1, 0,
    1, 1, //upper half of the square

    0, 1,
    0, 0,
    1, 1, //upper half of the square
    //right
    0, 0,
    1, 1,
    0, 1, //upper half of the square

    1, 1,
    0, 0,
    1, 0, //upper half of the square
    //top
    1, 1,
    0, 1,
    0, 0, //upper half of the square

    1, 1,
    0, 0,
    1, 0, //upper half of the square
}; //lower half of the square

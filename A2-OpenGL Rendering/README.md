##Author: YuWei(Wayne) Zhang

##Part 1:
An implementation of an interactive wireframe rendering of a cube.

Basic operation: 

1.press the left mouse button down and move the cursor left and right,the camera should rotate around the centre of                     the object. When you press down the left  mouse button and move the cursor up and down, the camera rotates up and                     down

 2.When press down the right mouse button and move the cursor up and down, the camera moves closer and further away                      from the cube
 
 More details in A2-part1 spec.
 
##Part 2:
Cube !! Skybox !! check it out

## ImageKit Update 1
This project implements teaching frameworks for an introductory computer graphics course.

1. Class **"Image"** implements a simple matlab-like syntax for generating/showing/saving images. The image is shown on screen using GLFW and openGL. The image is saved to disk using libpng and zlib.
 
2. Class **"Canvas"** implements a simple warpping over GLFW3. Students can simply fill in a few callbacks in main.cpp. Includes:

  -. OnPaint

  -. MouseMove

  -. MouseButton

  -. KeyPress

  -. OnTimer

Please see main.cpp for the usage demo.

This framework has been tested on the following platforms :

- OSX Yosemite 10.10.5 + XCode 7.0.1 Build &A1001
- Windows 10.1 + Visual Studio 2013 Community (VS12)

To run this framework on any other platforms, the required libraries need to be recompiled according to the target OS.

###Useful links:
- GLFW: [http://www.glfw.org/]
- GLEW: [http://glew.sourceforge.net/] (only required on Windows)
- libpng: [http://libpng.org/pub/png/libpng.html]
- zlib: [http://www.zlib.net/]

#pragma once

#include "incbase.h"

typedef void(* MouseMoveCallback)(double, double);
typedef enum enuMouseButtons {LeftButton, MiddleButton, RightButton} MouseButtons;
typedef void(* MouseButtonCallback)(MouseButtons, bool);
typedef void(* KeyCallback)(char);
typedef void(* OnPaintCallback)();
typedef void(* TimerCallback)();

class Canvas
{
public:
    Canvas();
    ~Canvas();
    //Set the input callbacks before you call initialize
    void SetMouseMove(MouseMoveCallback callback);
    void SetMouseButton(MouseButtonCallback callback);
    void SetKeyPress(KeyCallback callback);
    void SetOnPaint(OnPaintCallback callback);
    void SetTimer(double interval, TimerCallback callback);
    void Initialize(unsigned int width,
                    unsigned int height,
                    const char * title = NULL);
    void Show();
    void AddLine(float x_start, float y_start,
                 float x_end, float y_end);
    void Clear();


    int _width, _height;
    GLFWwindow * pWindow;
    MouseMoveCallback pfun_mousemove;
    MouseButtonCallback pfun_mousebutton;
    KeyCallback pfun_key;
    OnPaintCallback pfun_paint;
    TimerCallback pfun_timer;
    double timerInverval;
    double lastTimer;
    bool clean;
    GLfloat BK_r, BK_g, BK_b;
    GLfloat Line_r, Line_g, Line_b;
    std::vector<GLfloat> line_cord;

    void initializeGL();
    void cleanupGL();
    void DrawLines();


    struct GLConfigure
    {
        GLuint _vao; ///< vertex array object
        GLuint _pid; ///< GLSL shader program ID
        GLuint _vbo_vpoint; ///< memory buffer
        GLConfigure()
        {
            _vao = _pid = _vbo_vpoint = 0;
        }
    };

   GLConfigure GLInfo;
};


#include "Canvas.h"

static const char * vshader_srctxt = " \
        #version 330 core \n\
        in vec2 vpoint; \
        void main() { \
            gl_Position = vec4(vpoint, 0, 1); \
        } \
        ";

static const char * fshader_srctxt = " \
        #version 330 core \n\
        out vec3 color; \
        uniform vec3 linecolor; \
        void main() { color = linecolor; } \
        ";

static Canvas * pCurrent = NULL;

static void GLFWCursorposInterface(GLFWwindow * pWindow, double x, double y)
{
    //Callback-OOP interface, guard from invalid pointers
    if (pCurrent == NULL) return;
    if (pWindow != pCurrent->pWindow) return;
    if (pCurrent->pfun_mousemove == NULL) return;
    //Call the registered call back
    pCurrent->pfun_mousemove(x, y);
    return;
}

static void GLFWMouseKeyInterface(GLFWwindow * pWindow, int button, int action, int modes)
{
    //Callback-OOP interface, guard from invalid pointers
    if (pCurrent == NULL) return;
    if (pWindow != pCurrent->pWindow) return;
    if (pCurrent->pfun_mousebutton == NULL) return;
    //Simplify the data and call the registered call back
    bool press = true;
    if (action == GLFW_RELEASE) press = false;
    switch (button)
    {
    case GLFW_MOUSE_BUTTON_LEFT : pCurrent->pfun_mousebutton(LeftButton, press);
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE: pCurrent->pfun_mousebutton(MiddleButton, press);
        break;
    case GLFW_MOUSE_BUTTON_RIGHT: pCurrent->pfun_mousebutton(RightButton, press);
        break;
    default:
        break;
    }

}

static void GLFWKeyInterface(GLFWwindow * pWindow, int key, int scancode, int action, int modes)
{
    //Callback-OOP interface, guard from invalid pointers
    if (pCurrent == NULL) return;
    if (pWindow != pCurrent->pWindow) return;
    if (pCurrent->pfun_key == NULL) return;
    //Simplify the data and call the registered call back
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(pCurrent->pWindow, GL_TRUE);
        else
        pCurrent->pfun_key((char)key); //UPPER CASE
    }
}

static GLfloat vpclipping(float src)
{
    if (src < -1) return -1;
    if (src > 1) return 1;
    return (GLfloat)src;
}

///////////////////////////////////////////////////////////

Canvas::Canvas()
{
    pWindow = NULL;
    pfun_mousemove = NULL;
    pfun_mousebutton = NULL;
    pfun_key = NULL;
    pfun_timer = NULL;
    clean = true;

    BK_r = BK_g = BK_b = 0;
    Line_r = Line_g = Line_b = 1;
}

Canvas::~Canvas()
{

}


void Canvas::Initialize(unsigned int width,
                   unsigned int height,
                   const char * title)
{
    pWindow = NULL;
    _width = width;
    _height = height;
    if (title == NULL)
    {
        pWindow = InitializeGLFWWindow(_width, _height, "Canvas");
    } else
    {
        pWindow = InitializeGLFWWindow(_width, _height, title);
    }
    if (pWindow == NULL) return;
    pCurrent = this;
    //initialize openGL data structures
    initializeGL();

    //set call backs
    glfwSetCursorPosCallback(pWindow, GLFWCursorposInterface);
    glfwSetMouseButtonCallback(pWindow, GLFWMouseKeyInterface);
    glfwSetKeyCallback(pWindow, GLFWKeyInterface);
}

void Canvas::Show()
{
    //show the window
    glClearColor(BK_r, BK_g, BK_b, 1);
    lastTimer = glfwGetTime();
    while (!glfwWindowShouldClose(pWindow))
    {
        //Note: Timer is triggered at most once per rendering
        //Set interval to < 0 will prevent pfun_timer being called
        if ((pfun_timer != NULL) && (timerInverval >= 0))
        {
            double now = glfwGetTime();
            if ((now - lastTimer) > timerInverval)
            {
                pfun_timer();
                lastTimer = now;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);
        if (pfun_paint != NULL) pfun_paint();
        if (!clean)
        {
            DrawLines();
        }
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(pWindow);
    cleanupGL();
    glfwTerminate();
}


void Canvas::initializeGL()
{
    //Compile Shader Program
    GLInfo._pid = compile_shaders(vshader_srctxt, fshader_srctxt);
    glUseProgram(GLInfo._pid);

    //VAO
    glGenVertexArrays(1, &GLInfo._vao);
    glBindVertexArray(GLInfo._vao);

    //VBO
    glGenBuffers(1, &GLInfo._vbo_vpoint);
    glBindBuffer(GL_ARRAY_BUFFER, GLInfo._vbo_vpoint);
    GLuint vpoint_id = glGetAttribLocation(GLInfo._pid, "vpoint");
    if (vpoint_id != -1)
    {
        glEnableVertexAttribArray(vpoint_id);
        glVertexAttribPointer(vpoint_id, 2, GL_FLOAT,
                              GL_FALSE, 0, (void *)0);
    }
    //Uniform
    GLuint linecolor_id = glGetUniformLocation(GLInfo._pid, "linecolor");
    if (linecolor_id != -1)
    {
       glUniform3f(linecolor_id, Line_r, Line_g, Line_b);
    }

    glUseProgram(0);
    glBindVertexArray(0);
}

void Canvas::cleanupGL()
{
    glBindVertexArray(0);
    glDeleteBuffers(1, &GLInfo._vbo_vpoint);
    glDeleteVertexArrays(1, &GLInfo._vao);

    glUseProgram(0);
    glDeleteProgram(GLInfo._pid);
}

void Canvas::DrawLines()
{
    glUseProgram(GLInfo._pid);
    glBindVertexArray(GLInfo._vao);
    glBufferData(GL_ARRAY_BUFFER, line_cord.size() * sizeof(float),
                 &line_cord[0],
                 GL_STATIC_DRAW);
    glDrawArrays(GL_LINES, 0, line_cord.size() / 2);
    glUseProgram(0);
    glBindVertexArray(0);
}

void Canvas::Clear()
{
    line_cord.clear();
    clean = true;
}

void Canvas::AddLine(float x_start, float y_start, float x_end, float y_end)
{
    line_cord.push_back(x_start);
    line_cord.push_back(y_start);
    line_cord.push_back(x_end);
    line_cord.push_back(y_end);
    clean = false;
}

void Canvas::SetMouseMove(MouseMoveCallback callback)
{
    pfun_mousemove = callback;
}

void Canvas::SetMouseButton(MouseButtonCallback callback)
{
    pfun_mousebutton = callback;
}

void Canvas::SetKeyPress(KeyCallback callback)
{
    pfun_key = callback;
}

void Canvas::SetOnPaint(OnPaintCallback callback)
{
    pfun_paint = callback;
}

void Canvas::SetTimer(double interval, TimerCallback callback)
{
    timerInverval = interval;
    pfun_timer = callback;
    lastTimer = glfwGetTime();
}

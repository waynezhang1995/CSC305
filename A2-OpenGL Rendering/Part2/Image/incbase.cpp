#include "incbase.h"

GLuint compile_shaders(const char * vshader,
                              const char * fshader)
{
    //compile the vertex shader
    GLuint VShaderID = glCreateShader(GL_VERTEX_SHADER);
    const char * vshader_str = vshader;
    glShaderSource(VShaderID, 1, &vshader_str, NULL);
    glCompileShader(VShaderID);

    //check the vertex shader
    GLint success = GL_FALSE;
    int InfoLogLength;
    std::vector<char> errMsg;
    glGetShaderiv(VShaderID, GL_COMPILE_STATUS, &success);
    glGetShaderiv(VShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
     if(!success) {
         errMsg.resize(InfoLogLength);
         glGetShaderInfoLog(VShaderID,
                            InfoLogLength, NULL, &errMsg[0]);
         fprintf(stderr,
                 "Vertex Shader Compilation Error:\n%s\n",
                 &errMsg[0]);
         return 0;
     }

     //compile the fragment shader
     GLuint FShaderID = glCreateShader(GL_FRAGMENT_SHADER);
     char const * fshader_str = fshader;
     glShaderSource(FShaderID, 1, &fshader_str , NULL);
     glCompileShader(FShaderID);

     /// Check Fragment Shader
     glGetShaderiv(FShaderID, GL_COMPILE_STATUS, &success);
     glGetShaderiv(FShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
     if(!success) {
         errMsg.resize(InfoLogLength);
         glGetShaderInfoLog(FShaderID,
                            InfoLogLength, NULL, &errMsg[0]);
         fprintf(stderr, "Fragment Shader Compilation Error:\n%s\n", &errMsg[0]);
         return 0;
     }

     // Link the program
     GLuint ProgramID = glCreateProgram();
     glAttachShader(ProgramID, VShaderID);
     glAttachShader(ProgramID, FShaderID);
     glLinkProgram(ProgramID);

     /// Check the program
     glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
     glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);

     if(!success) {
         errMsg.resize(InfoLogLength);
         glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &errMsg[0]);
         fprintf(stderr, "Program Link Failed:\n%s\n", &errMsg[0]);
         return 0;
     }

     glDeleteShader(VShaderID);
     glDeleteShader(FShaderID);

     return ProgramID;
}

static void error_char(int error, const char* description)
{
    fputs(description, stderr);
}

GLFWwindow * InitializeGLFWWindow(unsigned int _width,
                          unsigned int _height,
                          const char * title)
{
    glfwSetErrorCallback(error_char);
    if (!glfwInit())
    {
        fprintf(stderr, "\nGLFW System Initialization Failed.\n");
        return NULL;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow * pWindow = glfwCreateWindow(_width, _height, title, NULL, NULL);

    if (!pWindow)
    {
        fprintf(stderr, "\nGLFW Window creation failed.\n");
        glfwTerminate();
        return NULL;
    } else
    {
        int major, minor, rev;
        glfwMakeContextCurrent(pWindow);

#ifdef WIN_COMPILE
        glewExperimental=GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            fprintf(stderr, "GLEW initialization failed.\n");
            return NULL;
        }
#endif
        glfwGetVersion(&major, &minor, &rev);
        fprintf(stderr, "OpenGL version recieved: %d.%d.%d.\n", major, minor, rev);
        glfwSwapInterval(1);
        return pWindow;
    }
	return NULL;
}

Texture LoadPNGTexture(const char * filename)
{
	Texture result;
	png_image imagectrl;
	imagectrl.version = PNG_IMAGE_VERSION;
	imagectrl.opaque = NULL;
	png_imagep imagectrl_p = &imagectrl;

	int readresult = png_image_begin_read_from_file(imagectrl_p, filename);
	if (PNG_IMAGE_FAILED(imagectrl) || (readresult == 0))
	{
		fprintf(stderr, "Cannot open PNG file: %s.\n", filename);
		fprintf(stderr, imagectrl.message);
		return result;
	}

	result.width = imagectrl.width;
	result.height = imagectrl.height;
	result.bufferlen = result.width * result.height * 4;
	result.dataptr = malloc(result.bufferlen);
	imagectrl.format = PNG_FORMAT_RGBA;
	
	readresult = png_image_finish_read(imagectrl_p, NULL, result.dataptr, 0, NULL);
	
	if (PNG_IMAGE_FAILED(imagectrl) || (readresult == 0))
	{
		fprintf(stderr, "PNG file read failed: %s.\n", filename);
		fprintf(stderr, imagectrl.message);
		return result;
	}
	
	return result;
}

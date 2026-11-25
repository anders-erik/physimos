
#define GL_LINKSCOPE

// #define GL_LOADER
// #if GL_LINKSCOPE
// #endif

// #undef GL_LINKSCOPE 


// #define GL_LINKSCOPE static

#include "opengl/loader.hh"

#include <string>

#include "lib/print.hh"

// #ifdef GL_LINKSCOPE
// #undef GL_LINKSCOPE 
// #endif

// #define GL_LINKSCOPE static

// #define GL_LINKSCOPE static

// GLEXT glGetError_T glGetError = nullptr;

void load_opengl(getProcAddress_T getProcAddress)
{
    auto address_to_str = [](void* ptr) -> Str
    {
        char string[20] {0};
        sprintf(string, "%p", (void *) ptr);
        return Str{string};
    };  
    // eglGetProcAddress

    // const GLubyte *glGetString( 	GLenum name);

    // #define GLL_glGetString const GLubyte*
    // #define GLL_glGetString const GLubyte*

        // typedef const GLubyte * (APIENTRYP PFNGLGETSTRINGPROC)(GLenum name);
    
    // #define gll_glGetString_type GLubyte * ()(GLenum name)

    // typedef void* eglGetProcAddress_rettype;
    
    // typedef GLubyte* gll_glGetString_RETTYPE;
    // typedef GLubyte* (*glGetString_FNADDRESS)(GLenum);
    // glGetString_FNADDRESS glGetString_gll;

    // glGetString_gll = (glGetString_FNADDRESS)eglGetProcAddress("glGetString");
    // Print::buf("gll_glGetString = ");
    // Print::ln(address_to_str((void*)glGetString_gll));


    glGetString  = (glGetString_T)getProcAddress("glGetString");
    // glGetString  = (glGetString_T)eglGetProcAddress("glGetString");

    GLubyte* gl_version = (GLubyte*)glGetString(GL_VERSION);
    Print::buf("gl_version = ");
    Print::ln({(const char*)gl_version});
    // Print::ln(Str::UI(*gll_glGetString_version));


    glGenBuffers = (glGenBuffers_T)getProcAddress("glGenBuffers");
    glIsBuffer = (glIsBuffer_T)getProcAddress("glIsBuffer");
    glBindBuffer = (glBindBuffer_T)getProcAddress("glBindBuffer");
    glBufferData = (glBufferData_T)getProcAddress("glBufferData");
    glBufferSubData = (glBufferSubData_T)getProcAddress("glBufferSubData");
    glDeleteBuffers = (glDeleteBuffers_T)getProcAddress("glDeleteBuffers");

    glGenVertexArrays = (glGenVertexArrays_T)getProcAddress("glGenVertexArrays");
    glBindVertexArray = (glBindVertexArray_T)getProcAddress("glBindVertexArray");
    glDeleteVertexArrays = (glDeleteVertexArrays_T)getProcAddress("glDeleteVertexArrays");

    glVertexAttribPointer = (glVertexAttribPointer_T)getProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (glEnableVertexAttribArray_T)getProcAddress("glEnableVertexAttribArray");

    glCreateProgram = (glCreateProgram_T)getProcAddress("glCreateProgram");
    glCreateShader  = (glCreateShader_T)getProcAddress("glCreateShader");
    glShaderSource  = (glShaderSource_T)getProcAddress("glShaderSource");
    glCompileShader  = (glCompileShader_T)getProcAddress("glCompileShader");
    glAttachShader  = (glAttachShader_T)getProcAddress("glAttachShader");
    glLinkProgram  = (glLinkProgram_T)getProcAddress("glLinkProgram");
    glDeleteShader  = (glDeleteShader_T)getProcAddress("glDeleteShader");
    glViewport  = (glViewport_T)getProcAddress("glViewport");
    glUseProgram  = (glUseProgram_T)getProcAddress("glUseProgram");
    glClearColor  = (glClearColor_T)getProcAddress("glClearColor");
    glClear  = (glClear_T)getProcAddress("glClear");

    glDrawArrays  = (glDrawArrays_T)getProcAddress("glDrawArrays");
    glDrawElements = (glDrawElements_T)getProcAddress("glDrawElements");

    glGetUniformLocation = (glGetUniformLocation_T)getProcAddress("glGetUniformLocation");
    glUniform1f = (glUniform1f_T)getProcAddress("glUniform1f");
    glUniformMatrix4fv = (glUniformMatrix4fv_T)getProcAddress("glUniformMatrix4fv");
    
    

    glGetError  = (glGetError_T)getProcAddress("glGetError");

    // if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress)) {
    //     fprintf(stderr, "Failed to load OpenGL functions\n");
    //     exit(1);
    // }
}

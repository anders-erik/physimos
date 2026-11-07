

#include "wayland-egl.h"
#include <EGL/egl.h>

#include "wl-gl.hh"
#include "wl-egl.hh"




void WL_EGL::check_egl_error(EGLint error_code)
{
    if (error_code != EGL_SUCCESS) {
        fprintf(stderr, "ERROR: EGL: %d\n", error_code);
        exit(EXIT_FAILURE);
    }
}

void WL_EGL::init(struct wl_display* wl_display, struct wl_surface* wl_surface, i2 dims)
{
    width = dims.x;
    height = dims.y;

    // DISPLAY
    display = eglGetDisplay((EGLNativeDisplayType)wl_display);
    eglInitialize(display, &version.major, &version.minor);
    fprintf(stdout, "EGL initialized. Version: %d.%d\n", version.major, version.minor);


    eglChooseConfig(    display, 
                        config_attribs, 
                        &config, 
                        1, 
                        &num_configs    );
    printf("EGL Configs found: %d\n", num_configs);
    check_egl_error(eglGetError());

    // CONTEXT
    eglBindAPI(EGL_OPENGL_API);
    context = eglCreateContext( display, 
                                config, 
                                EGL_NO_CONTEXT, 
                                ctx_attribs         );
    check_egl_error(eglGetError());
    fprintf(stdout, "EGL Context created: %p\n", context);
    
    // SURFACE
    window = wl_egl_window_create(wl_surface, width, height);
    surface = eglCreateWindowSurface(display, config, window, NULL);
    
    eglMakeCurrent( display, 
                    surface, 
                    surface, 
                    context  );

    check_egl_error(eglGetError());
    fprintf(stdout, "EGL Surface created: %p\n", surface);


    load_GL();
    draw_triangle();

    eglSwapBuffers(display, surface);

    // eglSwapBuffers(display, surface);
    // wl_display_dispatch_pending(wl_display);
}

void WL_EGL::load_GL()
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
    
    typedef GLubyte* gll_glGetString_RETTYPE;
    typedef gll_glGetString_RETTYPE (*glGetString_FNADDRESS)(GLenum);
    glGetString_FNADDRESS glGetString_gll;

    glGetString_gll = (glGetString_FNADDRESS)eglGetProcAddress("glGetString");
    Print::buf("gll_glGetString = ");
    Print::ln(address_to_str((void*)glGetString_gll));


    gll_glGetString_RETTYPE gl_version = (gll_glGetString_RETTYPE)glGetString_gll(GL_VERSION);
    Print::buf("gl_version = ");
    Print::ln({(const char*)gl_version});
    // Print::ln(Str::UI(*gll_glGetString_version));


    glGenVertexArrays = (glGenVertexArrays_T)eglGetProcAddress("glGenVertexArrays");
    glGenBuffers = (glGenBuffers_T)eglGetProcAddress("glGenBuffers");
    glBindVertexArray = (glBindVertexArray_T)eglGetProcAddress("glBindVertexArray");
    glBindBuffer = (glBindBuffer_T)eglGetProcAddress("glBindBuffer");
    glBufferData = (glBufferData_T)eglGetProcAddress("glBufferData");
    glVertexAttribPointer = (glVertexAttribPointer_T)eglGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray = (glEnableVertexAttribArray_T)eglGetProcAddress("glEnableVertexAttribArray");
    glCreateProgram = (glCreateProgram_T)eglGetProcAddress("glCreateProgram");
    glCreateShader  = (glCreateShader_T)eglGetProcAddress("glCreateShader");
    glShaderSource  = (glShaderSource_T)eglGetProcAddress("glShaderSource");
    glCompileShader  = (glCompileShader_T)eglGetProcAddress("glCompileShader");
    glAttachShader  = (glAttachShader_T)eglGetProcAddress("glAttachShader");
    glLinkProgram  = (glLinkProgram_T)eglGetProcAddress("glLinkProgram");
    glDeleteShader  = (glDeleteShader_T)eglGetProcAddress("glDeleteShader");
    glViewport  = (glViewport_T)eglGetProcAddress("glViewport");
    glUseProgram  = (glUseProgram_T)eglGetProcAddress("glUseProgram");
    glClearColor  = (glClearColor_T)eglGetProcAddress("glClearColor");
    glClear  = (glClear_T)eglGetProcAddress("glClear");
    glDrawArrays  = (glDrawArrays_T)eglGetProcAddress("glDrawArrays");
    glGetError  = (glGetError_T)eglGetProcAddress("glGetError");

    // if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress)) {
    //     fprintf(stderr, "Failed to load OpenGL functions\n");
    //     exit(1);
    // }
} 

void WL_EGL::print_opengl_error()
{
    GLenum error_enum = glGetError();
    if(error_enum == GL_NO_ERROR)
        return;

    Print::buf("OPENGL ERROR: ");
    Str error_str;

    switch (error_enum)
    {
    case GL_INVALID_ENUM: error_str = "GL_INVALID_ENUM"; break;
    case GL_INVALID_VALUE: error_str = "GL_INVALID_VALUE"; break;
    case GL_INVALID_OPERATION: error_str = "GL_INVALID_OPERATION"; break;
    case GL_INVALID_FRAMEBUFFER_OPERATION: error_str = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
    case GL_OUT_OF_MEMORY: error_str = "GL_OUT_OF_MEMORY"; break;
    case GL_STACK_UNDERFLOW: error_str = "GL_STACK_UNDERFLOW"; break;
    case GL_STACK_OVERFLOW: error_str = "GL_STACK_OVERFLOW"; break;
    
    default:
        error_str = "UNKNOWN";
        break;
    }

    Print::ln(error_str);

}

void WL_EGL::draw_triangle()
{
    print_opengl_error();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
    glEnableVertexAttribArray(0);


    // create_program
    const char* vert_src =
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "    gl_Position = vec4(aPos, 1.0);\n"
        "}";
    const char* frag_src =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main() {\n"
        "    FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}";

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vert_src);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, frag_src);
    program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);


    glViewport(0, 0, width, height);

    glUseProgram(program);

    // rand clear color 
    // glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    print_opengl_error();
}

GLuint WL_EGL::compile_shader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);
    return shader;
}
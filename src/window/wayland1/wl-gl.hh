#pragma once



// Constant values:
// https://registry.khronos.org/OpenGL/index_gl.php: "The OpenGL registry ... includes the XML API registry of reserved enumerants and functions"
// Below is a link to the up to date values that I use to populate this header file
// https://raw.githubusercontent.com/KhronosGroup/OpenGL-Registry/refs/heads/main/xml/gl.xml

#define GL_VERSION 0x1F02

#define GL_NO_ERROR 0
#define GL_INVALID_ENUM 0x0500
#define GL_INVALID_VALUE  0x0501
#define GL_INVALID_OPERATION 0x0502
#define GL_INVALID_FRAMEBUFFER_OPERATION 0x0506
#define GL_OUT_OF_MEMORY 0x0505
#define GL_STACK_UNDERFLOW 0x0504
#define GL_STACK_OVERFLOW 0x0503

#define GL_FALSE 0
#define GL_FLOAT 0x1406
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_ARRAY_BUFFER 0x8892
#define GL_STATIC_DRAW 0x88E4
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004


typedef signed long int     khronos_ssize_t; // TODO: IMPORT ALL TYPES FROM 'extern/glad/KHR/khrplatform.h'
typedef khronos_ssize_t     GLsizeiptr;


// https://wikis.khronos.org/opengl/OpenGL_Type

typedef uint32_t	GLenum;
typedef uint8_t	    GLboolean;
typedef uint32_t	GLbitfield;
typedef void		GLvoid;
typedef int8_t	    GLbyte;	
typedef uint8_t	    GLubyte;
typedef int16_t		GLshort;
typedef uint16_t	GLushort;
typedef int32_t		GLint;	
typedef uint32_t	GLuint;	
typedef int32_t		GLsizei;
typedef float		GLfloat;



// https://registry.khronos.org/OpenGL-Refpages/gl4/

typedef void (*glGenVertexArrays_T)(GLsizei n, GLuint *arrays);
/**
    @brief short description explanation.
    @note Preconditions before calling. \n

    \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml

    \details Longer explanation. 
    asdf asdfghjk
 */
static glGenVertexArrays_T glGenVertexArrays; 

typedef void (*glGenBuffers_T)(GLsizei n, GLuint *buffers);
static glGenBuffers_T glGenBuffers; ///< @param n [GLsizei] Number of buffers  @param buffers [GLuint *] Array of buffer IDs @return void @brief [TLDR: generate buffer object names] glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to glGenBuffers. \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml


typedef void (*glBindVertexArray_T)(GLuint array);
static glBindVertexArray_T glBindVertexArray;

typedef void (*glBindBuffer_T)(GLenum target, GLuint buffer);
static glBindBuffer_T glBindBuffer;

typedef void (*glBufferData_T)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
static glBufferData_T glBufferData;

typedef void (*glVertexAttribPointer_T)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
static glVertexAttribPointer_T glVertexAttribPointer;

typedef void (*glEnableVertexAttribArray_T)(GLuint index);
static glEnableVertexAttribArray_T glEnableVertexAttribArray;

typedef GLuint (*glCreateProgram_T)(void);
static glCreateProgram_T glCreateProgram;

typedef GLuint (*glCreateShader_T)(GLenum shaderType);
static glCreateShader_T glCreateShader;

typedef void (*glShaderSource_T)(GLuint shader, GLsizei count, const char **string, const GLint *length);
static glShaderSource_T glShaderSource;

typedef void (*glCompileShader_T)(GLuint shader);
static glCompileShader_T glCompileShader;

typedef void (*glAttachShader_T)(GLuint program, GLuint shader);
static glAttachShader_T glAttachShader;

typedef void (*glLinkProgram_T)(GLuint program);
static glLinkProgram_T glLinkProgram;

typedef void (*glDeleteShader_T)(GLuint shader);
static glDeleteShader_T glDeleteShader;

typedef void (*glViewport_T)(GLint x, GLint y, GLsizei width, GLsizei height);
static glViewport_T glViewport;

typedef void (*glUseProgram_T)(GLuint program);
static glUseProgram_T glUseProgram;

typedef void (*glClearColor_T)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static glClearColor_T glClearColor;

typedef void (*glClear_T)(GLbitfield mask);
static glClear_T glClear;

typedef void (*glDrawArrays_T)(GLenum mode, GLint first, GLsizei count);
static glDrawArrays_T glDrawArrays;

typedef GLenum (*glGetError_T)(void);
static glGetError_T glGetError;

#pragma once

#include "stdint.h"


// #define GLEXT extern // Function pointers will be provided

#ifndef GL_LINKSCOPE
#define GL_LINKSCOPE extern
#endif

// #define GL_LINKSCOPE extern

// #ifdef GL_LOADER
//     #define GL_LINKSCOPE
// #else
//     #define GL_LINKSCOPE extern
// #endif

// #ifndef GL_LINKSCOPE
//     #define GL_LINKSCOPE extern // Function pointers will be provided
// #endif
// #ifdef GL_LOADER
//     #define GLEXT static
// #else
//     #define GLEXT extern
// #endif

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
#define GL_TRUE 1
#define GL_FLOAT 0x1406
#define GL_VERTEX_SHADER 0x8B31
#define GL_FRAGMENT_SHADER 0x8B30

#define GL_ARRAY_BUFFER                 0x8892      ///     Vertex attributes
#define GL_ATOMIC_COUNTER_BUFFER        0x90ED      /// 	Atomic counter storage
#define GL_COPY_READ_BUFFER             0x8F36      /// 	Buffer copy source
#define GL_COPY_WRITE_BUFFER            0x8F37      /// 	Buffer copy destination
#define GL_DISPATCH_INDIRECT_BUFFER     0x90EE      /// 	Indirect compute dispatch commands
#define GL_DRAW_INDIRECT_BUFFER         0x8F3F      /// 	Indirect command arguments
#define GL_ELEMENT_ARRAY_BUFFER         0x8893      /// 	Vertex array indices
#define GL_PIXEL_PACK_BUFFER            0x88EB      /// 	Pixel read target
#define GL_PIXEL_UNPACK_BUFFER          0x88EC      /// 	Texture data source
#define GL_QUERY_BUFFER                 0x9192      /// 	Query result buffer
#define GL_SHADER_STORAGE_BUFFER        0x90D2      /// 	Read-write storage for shaders
#define GL_TEXTURE_BUFFER               0x8C2A      /// 	Texture data buffer
#define GL_TRANSFORM_FEEDBACK_BUFFER    0x8C8E      /// 	Transform feedback buffer
#define GL_UNIFORM_BUFFER               0x8A11      /// 	Uniform block storage

#define GL_STREAM_DRAW 0x88E0   /// The data store contents will be modified once and used at most a few times. The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
#define GL_STREAM_READ 0x88E1   /// The data store contents will be modified once and used at most a few times. The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
#define GL_STREAM_COPY 0x88E2   /// The data store contents will be modified once and used at most a few times. The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
#define GL_STATIC_DRAW 0x88E4   /// The data store contents will be modified once and used many times. The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
#define GL_STATIC_READ 0x88E5   /// The data store contents will be modified once and used many times. The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
#define GL_STATIC_COPY 0x88E6   /// The data store contents will be modified once and used many times. The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.
#define GL_DYNAMIC_DRAW 0x88E8  /// The data store contents will be modified repeatedly and used many times. The data store contents are modified by the application, and used as the source for GL drawing and image specification commands.
#define GL_DYNAMIC_READ 0x88E9  /// The data store contents will be modified repeatedly and used many times. The data store contents are modified by reading data from the GL, and used to return that data when queried by the application.
#define GL_DYNAMIC_COPY 0x88EA  /// The data store contents will be modified repeatedly and used many times. The data store contents are modified by reading data from the GL, and used as the source for GL drawing and image specification commands.

#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_TRIANGLES 0x0004


typedef signed long int     khronos_ssize_t; // TODO: IMPORT ALL TYPES FROM 'extern/glad/KHR/khrplatform.h'
typedef signed   long  int     khronos_intptr_t;
typedef khronos_ssize_t     GLsizeiptr;
typedef khronos_intptr_t     GLintptr;
// typedef signed long int*     GLintptr;


// https://wikis.khronos.org/opengl/OpenGL_Type

typedef uint32_t	GLenum;
typedef uint8_t	    GLboolean;
typedef uint32_t	GLbitfield;
typedef void		GLvoid;
typedef char	    GLchar;	
typedef int8_t	    GLbyte;	
typedef uint8_t	    GLubyte;
typedef int16_t		GLshort;
typedef uint16_t	GLushort;
typedef int32_t		GLint;	
typedef uint32_t	GLuint;	
typedef int32_t		GLsizei;
typedef float		GLfloat;



// https://registry.khronos.org/OpenGL-Refpages/gl4/


typedef void (*glGenBuffers_T)(GLsizei n, GLuint *buffers);
GL_LINKSCOPE glGenBuffers_T glGenBuffers; ///< @param n [GLsizei] Number of buffers  @param buffers [GLuint *] Array of buffer IDs @return void @brief [TLDR: generate buffer object names] \n glGenBuffers returns n buffer object names in buffers. There is no guarantee that the names form a contiguous set of integers; however, it is guaranteed that none of the returned names was in use immediately before the call to glGenBuffers. \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml @note Version > 2.0

typedef GLboolean (*glIsBuffer_T)(GLuint buffer);
GL_LINKSCOPE glIsBuffer_T glIsBuffer; ///< @param buffer [GLuint] Buffer ID to check @return is_buffer [GLboolean] true if the ID corresponds to a valid buffer @brief glIsBuffer returns GL_TRUE if buffer is currently the name of a buffer object. If buffer is zero, or is a non-zero value that is not currently the name of a buffer object, or if an error occurs, glIsBuffer returns GL_FALSE. A name returned by glGenBuffers, but not yet associated with a buffer object by calling glBindBuffer, is not the name of a buffer object. \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glIsBuffer.xhtml \n https://stackoverflow.com/questions/18193327/when-are-opengl-object-names-ever-zero-or-does-zero-ever-have-semantics @note Version > 2.0

typedef void (*glBindBuffer_T)(GLenum target, GLuint buffer);
GL_LINKSCOPE glBindBuffer_T glBindBuffer; ///< @param target [GLenum] Buffer Binding Target, e.g. GL_ARRAY_BUFFER or GL_ELEMENT_ARRAY_BUFFER  @param buffer [GLuint] buffer ID for which we specify the type @return [void] @brief glBindBuffer binds a buffer object to the specified buffer binding point. Calling glBindBuffer with target set to one of the accepted symbolic constants and buffer set to the name of a buffer object binds that buffer object name to the target. If no buffer object with name buffer exists, one is created with that name. When a buffer object is bound to a target, the previous binding for that target is automatically broken.  \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml @note Version > 2.0

typedef void (*glBufferData_T)(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
GL_LINKSCOPE glBufferData_T glBufferData; ///< @param target [GLenum] The target type for which the buffer is providing data @param size [GLsizeiptr] Specifies the size in bytes of the buffer object's new data store. @param data [] Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied. @param usage [] Specifies the expected usage pattern of the data store. The symbolic constant must be GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY @return void  @brief glBufferData creates a new data store for a buffer object. The buffer object currently bound to target is used. \n While creating the new storage, any pre-existing data store is deleted. The new data store is created with the specified size in bytes and usage. If data is not NULL, the data store is initialized with data from this pointer. In its initial state, the new data store is not mapped, it has a NULL mapped pointer, and its mapped access is GL_READ_WRITE. usage is a hint to the GL implementation as to how a buffer object's data store will be accessed. This enables the GL implementation to make more intelligent decisions that may significantly impact buffer object performance. It does not, however, constrain the actual usage of the data store. usage can be broken down into two parts: first, the frequency of access (modification and usage), and second, the nature of that access. \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferData.xhtml @note Version > 2.0

typedef void (*glBufferSubData_T)(GLenum target, GLintptr offset, GLsizeiptr size, const void *data);
GL_LINKSCOPE glBufferSubData_T glBufferSubData; ///< @param target [GLenum] The target type for which the buffer is providing data @param offset [GLintptr]  Specifies the offset into the buffer object's data store where data replacement will begin, measured in bytes.   @param size [GLsizeiptr]  Specifies the size in bytes of the data store region being replaced.  @param data [] Specifies a pointer to data that will be copied into the data store for initialization, or NULL if no data is to be copied.  @return void  @brief glBufferSubData redefine some or all of the data store for the specified buffer object. Data starting at byte offset offset and extending for size bytes is copied to the data store from the memory pointed to by data. offset and size must define a range lying entirely within the buffer object's data store. \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBufferSubData.xhtml  @note Version > 2.0

typedef void (*glDeleteBuffers_T)(GLsizei n, const GLuint * buffers);
GL_LINKSCOPE glDeleteBuffers_T glDeleteBuffers; ///< @param n [GLsizei] Specifies the number of buffer objects to be deleted. @param buffers [const GLuint *] Specifies an array of buffer objects to be deleted.   @return void  @brief glDeleteBuffers deletes n buffer objects named by the elements of the array buffers. After a buffer object is deleted, it has no contents, and its name is free for reuse (for example by glGenBuffers). If a buffer object that is currently bound is deleted, the binding reverts to 0 (the absence of any buffer object). \n glDeleteBuffers silently ignores 0's and names that do not correspond to existing buffer objects.  \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glDeleteBuffers.xhtml @note Version > 2.0



typedef void (*glGenVertexArrays_T)(GLsizei n, GLuint *arrays);
/**
    @brief short description explanation.
    @note Preconditions before calling. \n
    \n @see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGenBuffers.xhtml

    \details Longer explanation. 
    asdf asdfghjk
 */
GL_LINKSCOPE glGenVertexArrays_T glGenVertexArrays; 

typedef void (*glBindVertexArray_T)(GLuint array);
GL_LINKSCOPE glBindVertexArray_T glBindVertexArray;


typedef void (*glVertexAttribPointer_T)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void * pointer);
GL_LINKSCOPE glVertexAttribPointer_T glVertexAttribPointer;

typedef void (*glEnableVertexAttribArray_T)(GLuint index);
GL_LINKSCOPE glEnableVertexAttribArray_T glEnableVertexAttribArray;

typedef GLuint (*glCreateProgram_T)(void);
GL_LINKSCOPE glCreateProgram_T glCreateProgram;

typedef GLuint (*glCreateShader_T)(GLenum shaderType);
GL_LINKSCOPE glCreateShader_T glCreateShader;

typedef void (*glShaderSource_T)(GLuint shader, GLsizei count, const char **string, const GLint *length);
GL_LINKSCOPE glShaderSource_T glShaderSource;

typedef void (*glCompileShader_T)(GLuint shader);
GL_LINKSCOPE glCompileShader_T glCompileShader;

typedef void (*glAttachShader_T)(GLuint program, GLuint shader);
GL_LINKSCOPE glAttachShader_T glAttachShader;

typedef void (*glLinkProgram_T)(GLuint program);
GL_LINKSCOPE glLinkProgram_T glLinkProgram;

typedef void (*glDeleteShader_T)(GLuint shader);
GL_LINKSCOPE glDeleteShader_T glDeleteShader;

typedef void (*glViewport_T)(GLint x, GLint y, GLsizei width, GLsizei height);
GL_LINKSCOPE glViewport_T glViewport;

typedef void (*glUseProgram_T)(GLuint program);
GL_LINKSCOPE glUseProgram_T glUseProgram;

typedef void (*glClearColor_T)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
GL_LINKSCOPE glClearColor_T glClearColor;

typedef void (*glClear_T)(GLbitfield mask);
GL_LINKSCOPE glClear_T glClear;

typedef void (*glDrawArrays_T)(GLenum mode, GLint first, GLsizei count);
GL_LINKSCOPE glDrawArrays_T glDrawArrays;

typedef GLenum (*glGetError_T)(void);
GL_LINKSCOPE glGetError_T glGetError;

typedef GLubyte* (*glGetString_T)(GLenum val);
GL_LINKSCOPE glGetString_T glGetString;

typedef void (*glUniform1f_T)(GLint location, GLfloat v0);
GL_LINKSCOPE glUniform1f_T glUniform1f;

typedef GLint (*glGetUniformLocation_T)(GLuint program, const GLchar *name);
GL_LINKSCOPE glGetUniformLocation_T glGetUniformLocation;

// GLint glGetUniformLocation(GLuint program, const GLchar *name);





typedef void* (*voidptr_return_function)();
typedef void* (*getProcAddress_T)(const char* procedure_name);

void load_opengl(getProcAddress_T getProcAddress);
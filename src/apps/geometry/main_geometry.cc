#include <cmath>

#include "lib/print.hh"
#include "lib/clock.hh"

#include "rend/sw/swrend.hh"
#include "opengl/loader.hh"

#include "window/wayland1/wayland.hh"
#include "window/wayland1/wl-defs.hh"

Stopwatch stopwatch;

void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data);


const struct wl_callback_listener callback_listener = {
    .done = callback_done,
};


void callback_done(  void *data,
		    struct wl_callback *wl_callback,
		    uint32_t callback_data)
{
    stopwatch.stop_and_print_ms();
    Print::ln("Callback done!");

    Wayland* wayland = (Wayland*) data;

    wl_callback_destroy(wl_callback);
    // wl_callback = wl_surface_frame(wayland->surfaces.opengl.wl_surface);
    wl_callback_add_listener(wl_callback, &callback_listener, wayland);

    stopwatch.start();
}





namespace OpenGL
{

typedef float Point3f[3];


struct VertexShader
{
    GLuint id = 0;

    void init(Str _code)
    {
        const char* code_c_str = _code.to_c_str();
        id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(id, 1, &code_c_str, NULL);
        glCompileShader(id);
    }

    void destroy()
    {
        glDeleteShader(id);
        id = 0;
    }
};

struct FragmentShader
{
    GLuint id = 0;

    void init(Str _code)
    {
        const char* code_c_str = _code.to_c_str();
        id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(id, 1, &code_c_str, NULL);
        glCompileShader(id);
        // return shader;
    }

    void destroy()
    {
        glDeleteShader(id);
        id = 0;
    }
};


struct Program
{
    GLuint id = 0;
    VertexShader vertex_shader;
    FragmentShader fragment_shader;

    void init(Str vertex_shader_source, Str fragment_shader_source)
    {
        id = glCreateProgram();

        vertex_shader.init(vertex_shader_source);
        fragment_shader.init(fragment_shader_source);

        glAttachShader(id, vertex_shader.id);
        glAttachShader(id, fragment_shader.id);
        glLinkProgram(id);
    }
};


void print_opengl_error()
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

void set_viewport_size(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);
}


struct Buffer
{
    /* datatype_dims_type*/
    enum Type
    {
        pos_3_f,
    };

    GLuint id = 0;
    GLuint vao = 0;

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };


    Buffer* init(Point3f* data, int data_size, Type type)
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        glDeleteBuffers(1, &id);

        if(!glIsBuffer(id))
            Print::ln("1. buffer.id not yet a valid buffer");
        glGenBuffers(1, &id);
        if(!glIsBuffer(id))
            Print::ln("2. buffer.id still not a valid buffer");
        glBindBuffer(GL_ARRAY_BUFFER, id);
        if(glIsBuffer(id))
            Print::ln("3. buffer.id is a valid buffer");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if(glIsBuffer(id))
            Print::ln("4. buffer.id is still a valid buffer");

        OpenGL::print_opengl_error();

        glBindBuffer(GL_ARRAY_BUFFER, id);




        glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);

        glBufferData(GL_ARRAY_BUFFER, data_size, data, GL_STATIC_DRAW);
        glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);


        return this;
    }


    Buffer* init()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        glEnableVertexAttribArray(0);

        glDeleteBuffers(1, &id);

        if(!glIsBuffer(id))
            Print::ln("1. buffer.id not yet a valid buffer");
        glGenBuffers(1, &id);
        if(!glIsBuffer(id))
            Print::ln("2. buffer.id still not a valid buffer");
        glBindBuffer(GL_ARRAY_BUFFER, id);
        if(glIsBuffer(id))
            Print::ln("3. buffer.id is a valid buffer");
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        if(glIsBuffer(id))
            Print::ln("4. buffer.id is still a valid buffer");

        OpenGL::print_opengl_error();

        glBindBuffer(GL_ARRAY_BUFFER, id);

        return this;
    }

    void destroy()
    {
        glDeleteBuffers(1, &id);
    }
};


struct Triangle
{
    Program program;
    Buffer buffer;

    float background_color = 0.0f;
    float background_color_time = 0.0f;

    float color = 0.0f;
    const char* color_name_in_shader = "color";
    GLint color_location = 0;

    Point3f verts[3] = {
        {0.0f,  1.0f, 0.0f}, // top center
        {-1.0f, -1.0f, 0.0f}, // bottom left
        {1.0f, -1.0f, 0.0f}  // bottom right
    };

    float new_last_vert[3] = {
        1.0f, -0.5f, 0.0f  // new bottom right
    };

    const char* vert_src =
"#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"void main() {\n"
"    gl_Position = vec4(aPos, 1.0);\n"
"}";
    
    const char* frag_src =
"#version 330 core\n"

"uniform float color;\n"
"out vec4 FragColor;\n"

"void main() {\n"
"    FragColor = vec4(color, 0.0, 0.0, 1.0);\n"
// "    FragColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
"}";


    void init()
    {
        buffer.init(verts, sizeof(verts), Buffer::pos_3_f);
        // buffer.init(verts, Buffer::pos_3_f);
        // buffer.init();


        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);

        // glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
        // glBufferSubData(GL_ARRAY_BUFFER, 6 * sizeof(float), sizeof(new_last_vert), new_last_vert);

        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

        program.init(vert_src, frag_src);

        glUseProgram(program.id);

        color_location = glGetUniformLocation(program.id, color_name_in_shader);
        glUniform1f(color_location, color);
    }


    void draw()
    {

        glUseProgram(program.id);

        // BACKGROUND
        // color_t += 0.01f;
        background_color_time += 0.03f;
        background_color = sin(background_color_time);
        // rand clear color 
        // glClearColor((float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f);
        // glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
        glClearColor(background_color, background_color, background_color, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // TRIANGLE
        glUniform1f(color_location, color);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        OpenGL::print_opengl_error();
    }


    void destroy()
    {
        glDeleteShader(program.fragment_shader.id);
        glDeleteShader(program.vertex_shader.id);

        buffer.destroy();
    }
};

} // 'OpenGL' END




int main()
{
    Print::ln("Hello, from Geometry! \n");

    Clock clock;
    PerformanceClock performance_clock;
    uint64_t prev_time = clock.get_unix_epoch_ms();
    

    Wayland wayland;
    wayland.init({  .width = 800, 
                    .height = 600, 
                    .rendering_type = WaylandRenderingType::OpenGL, 
                    // .rendering_type = WaylandRenderingType::SWR,
                    .window_title = "Main Geometry"                 });



    SWR::Renderer swrenderer;
    OpenGL::Triangle opengl_triangle;

    if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
    {
        swrenderer.set_buffer( wayland.get_content_buffer() );
        swrenderer.clear(0xFF555555);
    }
    else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
    {
        struct wl_callback *cb = wl_surface_frame(wayland.surfaces.opengl.wl_surface);
        wl_callback_add_listener(cb, &callback_listener, &wayland);

        OpenGL::print_opengl_error();
        OpenGL::set_viewport_size(wayland.surfaces.opengl.egl.width, wayland.surfaces.opengl.egl.height);
        OpenGL::print_opengl_error();

        
        opengl_triangle.init();
        opengl_triangle.draw();
    }



    while(wayland.frame_step())
    {
        if(wayland.set_size())
        {

            if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
            {

            }
            if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
            {
                OpenGL::set_viewport_size(wayland.surfaces.opengl.egl.width, wayland.surfaces.opengl.egl.height);
            }
        }

        opengl_triangle.color += 0.001f;

        Arr<UserInput> user_inputs = wayland.get_new_input_events();
        for(uint i = 0; i < user_inputs.count(); i++)
        {
            UserInput user_input = user_inputs[i];
            if(user_input.is_scroll_input())
            {
                opengl_triangle.color += 0.01f;
            }

        }
        user_inputs.clear();

        if(wayland.surfaces.render_type == WaylandRenderingType::SWR)
        {
            swrenderer.set_buffer( wayland.get_content_buffer() );
            swrenderer.clear(0xFF654321);
        }
        else if(wayland.surfaces.render_type == WaylandRenderingType::OpenGL)
        {
            opengl_triangle.draw();
            wayland.surfaces.opengl.egl.swap_buffers();
        }

        usleep(10000);

        // prev_time = clock.get_unix_epoch_ms();
        // clock.print_current_m_sec();
        // performance_clock.fps_step(true);
        // Print::ln("");
    }

    return 0;
}

#include <stdio.h>

#include <wayland-client.h>

#include "lib/print.hh"
#include "rend/sw/swrend.hh"

#include "wl-surfaces.hh"
#include "wayland.hh"

void WL_Decoration::init(wl_compositor * compositor, i2 window_dims, i2 _monitor_dims, Wayland* wayland)
{
    int monitor_size_bytes = _monitor_dims.x * _monitor_dims.y * 4;
    pool.init(monitor_size_bytes * 2);

    surface.pixel_buffer = PixelBuffer{(PX32*)pool.data, window_dims.x, window_dims.y, PX32F::RGBA, PixelBuffer::Top};
    surface.format = wl_shm_format::WL_SHM_FORMAT_ARGB8888;;
    // surface.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Window);
    surface.init(compositor, pool.pool, 0, wayland);
    draw();

    // surface.commit();
}

void WL_Decoration::full_reload_from_content_size(i2 content_dims, Wayland* wayland)
{
    i2 window_dims = content_to_window_dims(content_dims);

    surface.pixel_buffer = PixelBuffer{(PX32*)  pool.data, 
                                                window_dims.x, 
                                                window_dims.y, 
                                                PX32F::ARGB, 
                                                PixelBuffer::Top    };
    draw();
    wl_buffer_destroy(surface.buffer);
    surface.create_new_buffer(pool.pool, 0, wayland);
    surface.commit();
}


WLDecorationArea WL_Decoration::get_area(i2 pos_raw)
{
    WLDecorationArea area;

    // Step through the cases from y = 0 to y = y_max

    if(pos_raw.y < border_width)
    {
        if(pos_raw.x < border_width)
            area = WLDecorationArea::EdgeTopLeft;
        else if(pos_raw.x > surface.pixel_buffer.w - border_width)
            area = WLDecorationArea::EdgeTopRight;
        else
            area = WLDecorationArea::EdgeTop;
    }
    else if(pos_raw.y < topbar_height + border_width )
    {
        if(pos_raw.x < border_width)
            area = WLDecorationArea::EdgeLeft;
        else if(pos_raw.x < topbar_height + border_width)
            area = WLDecorationArea::TopLeft;
        else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 3)))
            area = WLDecorationArea::Topbar;
        else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 2)))
            area = WLDecorationArea::MinimizeButton;
        else if(pos_raw.x < (surface.pixel_buffer.w - border_width - (topbar_height * 1)))
            area = WLDecorationArea::MaximizeButton;
        else if(pos_raw.x < (surface.pixel_buffer.w - border_width))
            area = WLDecorationArea::CloseButton;
        else
            area = WLDecorationArea::EdgeRight;
    }
    else if(pos_raw.y < (surface.pixel_buffer.h - border_width))
    {
        if(pos_raw.x < border_width)
            area = WLDecorationArea::EdgeLeft;
        else if(pos_raw.x > (surface.pixel_buffer.w - border_width))
            area = WLDecorationArea::EdgeRight;
        else
            area = WLDecorationArea::None;
    }
    else if (pos_raw.y < surface.pixel_buffer.h)
    {
        if(pos_raw.x < border_width)
            area = WLDecorationArea::EdgeBottomLeft;
        else if(pos_raw.x < (surface.pixel_buffer.w - border_width))
            area = WLDecorationArea::EdgeBottom;
        else if(pos_raw.x < surface.pixel_buffer.w)
            area = WLDecorationArea::EdgeBottomRight;
        else
            area = WLDecorationArea::None;
    }
    else
    {
        area = WLDecorationArea::None;
    }

    return area;
}


i2 WL_Decoration::get_content_pos()
{
    return {border_width, topbar_height + border_width};
    // return {-border_width, - (topbar_height + border_width) };
}

i2 WL_Decoration::content_to_window_dims(i2 content_dims)
{
    return  {   content_dims.x + border_width * 2, 
                content_dims.y + topbar_height + border_width * 2 };
}

void WL_Decoration::draw()
{
    SWR::Renderer renderer {surface.pixel_buffer};

    // PX32 edge_color = 0x00000000;
    // PX32 edge_color = 0x05000000;
    PX32 edge_color = 0x22000000;
    PX32 edge_line_color = 0x33333333;

    renderer.clear(edge_color);

    if(!maximized)
    {
        // bottom
        renderer.draw_line_float_lerp({border_width-1, border_width-1}, {surface.pixel_buffer.w - border_width, border_width-1}, edge_line_color);
        // Top
        renderer.draw_line_float_lerp({border_width-1, surface.pixel_buffer.h-border_width}, {surface.pixel_buffer.w - border_width, surface.pixel_buffer.h-border_width}, edge_line_color);
        // left
        renderer.draw_line_float_lerp({border_width-1, border_width}, {border_width-1, surface.pixel_buffer.h-border_width}, edge_line_color);
        // right
        renderer.draw_line_float_lerp({surface.pixel_buffer.w - border_width, border_width}, {surface.pixel_buffer.w - border_width, surface.pixel_buffer.h-border_width}, edge_line_color);
    }

    // Draw top bar
    PX32 topbar_color = 0xFF777777;
    renderer.draw_rectangle(    {border_width, surface.pixel_buffer.h - border_width - topbar_height}, 
                                {surface.pixel_buffer.w  - border_width, surface.pixel_buffer.h  - border_width}, 
                                topbar_color                                                                        );

    // close button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 30, surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w  - border_width, surface.pixel_buffer.h  - border_width}, 
                                0xFFAA4444                                                                                     );
    // Maximize/Restore button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 60, surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w - border_width - 30, surface.pixel_buffer.h  - border_width}, 
                                0xFFAAAA44                                                                                     );
    // Minimize button
    renderer.draw_rectangle(    {surface.pixel_buffer.w - border_width - 90,    surface.pixel_buffer.h - border_width - 30}, 
                                {surface.pixel_buffer.w - border_width - 60,    surface.pixel_buffer.h  - border_width}, 
                                0xFF44AA44                                                                                     );

    // Menu button
    renderer.draw_rectangle(    {border_width +30,    surface.pixel_buffer.h - border_width - 30}, 
                                {border_width ,    surface.pixel_buffer.h  - border_width}, 
                                0xFF555555                                                          );
    // renderer.draw_rectangle({0, 0}, {30, 30}, 0xFFAA4444);
}




int WL_Surface::get_size_pixels() { return pixel_buffer.w * pixel_buffer.h; };
int WL_Surface::get_size_bytes() { return pixel_buffer.w * pixel_buffer.h * 4; };



void WL_Surface::init(struct wl_compositor* compositor, struct wl_shm_pool* pool, int offset, Wayland* state)
{
    surface = wl_compositor_create_surface(compositor);

    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, format);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, state); 
    // wl_surface_attach(surface, buffer, 0, 0);
}

void WL_Surface::create_new_buffer(struct wl_shm_pool* pool, int offset, Wayland* state)
{
    buffer = wl_shm_pool_create_buffer(pool, offset, pixel_buffer.w, pixel_buffer.h, pixel_buffer.w*4, format);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, state);
    // wl_surface_attach(surface, buffer, 0, 0);
}

void WL_Surface::create_subsurface(struct wl_subcompositor* subcompositor, struct wl_surface* parent)
{
    // subsurface = wl_subcompositor_get_subsurface(subcompositor, parent, surface);
    subsurface = wl_subcompositor_get_subsurface(subcompositor, surface, parent);

    if (!subsurface)
    {
        fprintf(stderr, "Failed to create subsurface.\n");
        return;
    }

    wl_subsurface_set_position(subsurface, pos.x, pos.y);
}

void WL_Surface::clear(PX32 color)
{
    for(uint i = 0; i < get_size_bytes(); i++)
    {
        (pixel_buffer.buf)[i] = color;
    }
}

void WL_Surface::commit()
{
    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage_buffer(surface, 0, 0, pixel_buffer.w, pixel_buffer.h);
    wl_surface_commit(surface);
    surface_is_busy = true;
}






void WL_OpenGL::init(struct wl_compositor* compositor, i2 _content_dims, i2 _surface_pos, Wayland* state)
{
    
    surface.pixel_buffer = PixelBuffer{nullptr, _content_dims.x, _content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    surface.pos = _surface_pos;
    // egl.pos = {0, 0};

    surface.surface = wl_compositor_create_surface(compositor);
    surface.create_subsurface(state->core.subcompositor, state->surfaces.decorations.surface.surface);
    // state->egl.init(state->core.display, egl.surface, egl.pixel_buffer.size());
    state->surfaces.opengl.egl.init(state->core.display, surface.surface, _content_dims);
}

void WL_OpenGL::full_reload_from_content_size(i2 new_content_dims, i2 content_pos, Wayland* wayland)
{
    surface.pixel_buffer = PixelBuffer{nullptr, new_content_dims.x, new_content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    surface.pos = content_pos;

    egl.reload_with_new_content_size(surface.surface, new_content_dims);

    // Print::ln("Opengl full reload");

    wl_subsurface_set_position(surface.subsurface, content_pos.x, content_pos.y);
    // wl_surface_damage_buffer(surface.surface, 0, 0, surface.pixel_buffer.w, surface.pixel_buffer.h);
    // wl_surface_commit(surface.surface);
    // surface.surface_is_busy = true;
}


void WL_SoftwareRend::init(struct wl_compositor* compositor, i2 _content_dims, i2 _monitor_dims, i2 _surface_pos, Wayland* state)
{
    int monitor_size_bytes = _monitor_dims.x * _monitor_dims.y * 4;
    pool.init(monitor_size_bytes + 100000);


    surface.pixel_buffer = PixelBuffer{(PX32*)pool.data, _content_dims.x, _content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    surface.pos = _surface_pos;
    // surface.pixel_buffer.buf = (PX32*) get_buffer_pointer(WL_Surface::Content);

    SWR::Renderer renderer;
    renderer.set_buffer(surface.pixel_buffer);
    renderer.clear(0xFF000000);
    surface.init(state->core.compositor, pool.pool, 0, state);
    surface.create_subsurface(state->core.subcompositor, state->surfaces.decorations.surface.surface);
}


void WL_SoftwareRend::full_reload_from_content_size(i2 new_content_dims, i2 content_pos, Wayland* wayland)
{
    surface.pixel_buffer = PixelBuffer{(PX32*) pool.data, new_content_dims.x, new_content_dims.y, PX32F::ARGB, PixelBuffer::Top};
    surface.pos = content_pos;
    wl_subsurface_set_position(surface.subsurface, surface.pos.x, surface.pos.y);

    // SWR::Renderer renderer;
    // renderer.set_buffer(surface.pixel_buffer);
    // renderer.clear(0xFF000000);

    wl_buffer_destroy(surface.buffer);
    surface.create_new_buffer(pool.pool, 0, wayland);
    surface.commit();
}


void WL_Surfaces::init(i2 _content_dims, i2 _monitor_dims, WaylandRenderingType _rendering_type, Wayland* state)
{   
    render_type = _rendering_type;
    
    decorations.init(state->core.compositor, content_to_window_dims(_content_dims), _monitor_dims, state);

    if(render_type == WaylandRenderingType::Buffer)
        swr.init(state->core.compositor, _content_dims, _monitor_dims, decorations.get_content_pos(), state);
    else if(render_type == WaylandRenderingType::OpenGL)
        opengl.init(state->core.compositor, _content_dims, decorations.get_content_pos(), state);
}


void WL_Surfaces::full_reload_from_content_size(i2 new_content_dims, Wayland* wayland)
{
    window_size_change_detected = false;

    decorations.full_reload_from_content_size(new_content_dims, wayland);
    // decorations.surface.commit();
    // wl_surface_commit(decorations.surface.surface);

    // i2 new_content_dims = window_to_content_dims(size_change_dims);
    i2 new_content_pos = decorations.get_content_pos();
    
    if(render_type == WaylandRenderingType::Buffer)
        swr.full_reload_from_content_size(new_content_dims, new_content_pos, wayland);
    else if(render_type == WaylandRenderingType::OpenGL)
        opengl.full_reload_from_content_size(new_content_dims, new_content_pos, wayland);

}





i2  WL_Surfaces::content_to_window_dims(i2 content_dims)
{
    return  {   content_dims.x + decorations.border_width * 2, 
                content_dims.y + decorations.topbar_height + decorations.border_width * 2 };
}
i2  WL_Surfaces::window_to_content_dims(i2 window_dims)
{
    return  {   window_dims.x - decorations.border_width * 2, 
                window_dims.y - decorations.topbar_height - decorations.border_width * 2 };
}


int WL_Surfaces::get_all_surface_buffer_sizes_in_bytes()
{
    return (decorations.surface.get_size_bytes() + swr.surface.get_size_bytes() + opengl.surface.get_size_bytes());
}

int WL_Surfaces::get_offset(WL_Surface::Type _surface_type)
{
    switch (_surface_type)
    {
    case WL_Surface::Window:
        return 0;
        break;
    case WL_Surface::Content:
        return get_offset(WL_Surface::Window) + decorations.surface.get_size_bytes();
        break;
    // case WL_Surface::Test:
    //     return get_offset(WL_Surface::Content) + content.get_size_bytes();
    //     break;
    
    default:
        break;
    }

    return 0;
}


void* WL_Surfaces::get_buffer_pointer(WL_Surface::Type surface_type)
{
    if(render_type != WaylandRenderingType::Buffer && surface_type == WL_Surface::Content)
        Print::ln("WARN: Trying to access the rendering buffer when not using Software rendering buffer!");

    // return ((uint8_t*)pool.data) + get_offset(surface_type);
    return swr.pool.data;
}


i2 WL_Surfaces::get_content_dims()
{
    if(render_type == WaylandRenderingType::Buffer)
        return {swr.surface.pixel_buffer.w, swr.surface.pixel_buffer.h};
    else
        return {opengl.surface.pixel_buffer.w, opengl.surface.pixel_buffer.h};
}

struct wl_surface* WL_Surfaces::get_content_surface()
{
    struct wl_surface* return_surface = nullptr;

    if(render_type == WaylandRenderingType::Buffer)
        return_surface = swr.surface.surface;
    else if(render_type == WaylandRenderingType::OpenGL)
        return_surface = opengl.surface.surface;
    
    return return_surface;
}



void WL_Surfaces::commit()
{
    // decorations.surface.commit(); // NOTE:: This triggered a lot of window-release events, leading to stuttering. Only call 'wl_surface_commit' instead!
    wl_surface_commit(decorations.surface.surface);

    if(render_type == WaylandRenderingType::Buffer)
    {
        swr.surface.commit();
    }
    else if(render_type == WaylandRenderingType::OpenGL)
    {
        opengl.egl.swap_buffers();

        wl_surface_damage_buffer(opengl.surface.surface, 0, 0, opengl.surface.pixel_buffer.w, opengl.surface.pixel_buffer.h);
        wl_surface_commit(opengl.surface.surface);
        opengl.surface.surface_is_busy = false;
    }
    // test.commit();
    // egl.commit();
}



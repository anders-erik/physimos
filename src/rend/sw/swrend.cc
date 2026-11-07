
#include "lib/print.hh"
#include "lib/minmax.hh"

#include "math/line.hh"

#include "swrend.hh"



void SWR::Renderer::draw_point(i2 _p, PX32 _px)
{
    *(buffer.at(_p.x, _p.y)) = _px;
}


/** Draw pixelated line using kx + m where pixels are drawn using step sizes related to the k-value to generate the intermediate pixels when the slope is large. */
void SWR::Renderer::draw_line(i2 _p1, i2 _p2, PX32 _px)
{

    // Vertical line
    if(_p1.x == _p2.x)
    {
        uint x = _p1.x;
        uint y_min = _p1.y > _p2.y ? _p2.y : _p1.y;
        uint y_max = _p1.y > _p2.y ? _p1.y : _p2.y;

        for(uint y = y_min; y < y_max; y++)
            *(buffer.at(x, y)) = _px;
    }

    // Horizontal line
    if(_p1.y == _p2.y)
    {
        uint y = _p1.y;
        uint x_min = _p1.x > _p2.x ? _p2.x : _p1.x;
        uint x_max = _p1.x > _p2.x ? _p1.x : _p2.x;

        for(uint x = x_min; x < x_max; x++)
            *(buffer.at(x, y)) = _px;
    }

    // enforce p1 to have the lower x-value
    // This was done to keep the algorithm intact, which relies on left to right drawing
    if(_p1.x > _p2.x)
    {
        i2 p_tmp = _p1;
        _p1 = _p2;
        _p2 = p_tmp;
    }

    Line line {{_p1.x, _p1.y}, {_p2.x, _p2.y}};

    double step_size = fabs(1/(line.k));

    for(double x = line.p1.x; x < line.p2.x; x = x + step_size)
    {
        double y = line[x];
        *(buffer.at((int)x, (int)y)) = _px;
    }
}



/** Draw pixelated line using kx + m where pixels are drawn using step sizes related to the k-value to generate the intermediate pixels when the slope is large. */
void SWR::Renderer::draw_line_float_lerp(f2 _p1, f2 _p2, PX32 _px)
{
    // Check Bounds
    if(!buffer.is_in_bounds(_p1, _p2))
    {
        Print::buf("ERROR: ");
        Print::buf("Point of 'SWR::Renderer::draw_line_float_lerp(f2 _p1, f2 _p2, PX32 _px)' not within bounds of PixelBuffer!, ");
        Print::buf("File&NO: ");
        Str file_no = Str{__FILE__} + ":" + Str::SI(__LINE__);
        Print::ln(file_no);
        return;
    }

    float Dx = std::fabs(_p2.x -_p1.x);
    float Dy = std::fabs(_p2.y -_p1.y);
    float delta_major_direction = Dx > Dy ? Dx : Dy;
    float dt_lerp = 1 / delta_major_direction;

    auto lerp = [](float t, f2 _p1, f2 _p2)
        -> f2
    {
        return f2{  _p1.x + t*(_p2.x-_p1.x),    
                    _p1.y + t*(_p2.y-_p1.y)};
    };


    for(float t = 0.0f; t < 1.0f; t = t + dt_lerp)
    {
        f2 lerp_point = lerp(t, _p1, _p2);
        *(buffer.at(std::round(lerp_point.x), std::round(lerp_point.y))) = _px;
    }
}

void SWR::Renderer::rectangle(f2 p, float w, float h, PX32 color)
{
    draw_line_float_lerp({p.x, p.y}, {p.x + w, p.y}, color);
	draw_line_float_lerp({p.x + w, p.y}, {p.x + w, p.y + h}, color);
	draw_line_float_lerp({p.x + w, p.y + h}, {p.x, p.y + h}, color);
	draw_line_float_lerp({p.x, p.y + h}, {p.x, p.y}, color);

}
void SWR::Renderer::parallelogram(f2 p, float a, float b, float h, PX32 color)
{
    draw_line_float_lerp({p.x, p.y}, {p.x + b, p.y}, color);
	draw_line_float_lerp({p.x + b, p.y}, {p.x + a + b, p.y + h}, color);
	draw_line_float_lerp({p.x + a + b, p.y + h}, {p.x + a, p.y + h}, color);
	draw_line_float_lerp({p.x + a, p.y + h}, {p.x, p.y}, color);
}
void SWR::Renderer::square(f2 p, float l, PX32 color)
{
    draw_line_float_lerp({p.x, p.y}, {p.x + l, p.y}, color);
	draw_line_float_lerp({p.x + l, p.y}, {p.x + l, p.y + l}, color);
	draw_line_float_lerp({p.x + l, p.y + l}, {p.x, p.y + l}, color);
	draw_line_float_lerp({p.x, p.y + l}, {p.x, p.y}, color);
}


void SWR::Renderer::draw_rectangle(i2 _p1, i2 _p2, PX32 _px)
{
    int x_min = Min(_p1.x, _p2.x);
    int x_max = Max(_p1.x, _p2.x);

    int y_min = Min(_p1.y, _p2.y);
    int y_max = Max(_p1.y, _p2.y);

    for(int x = x_min; x < x_max; x++)
    {
        for(int y = y_min; y < y_max; y++)
        {
            *(buffer.at(x, y)) = _px;
        }
    }

}

void SWR::Renderer::draw_triangle_no_fill(i2 _p1, i2 _p2, i2 _p3, PX32 _px)
{
    draw_line(_p1, _p2, _px);
    draw_line(_p2, _p3, _px);
    draw_line(_p3, _p1, _px);
}

int orient(i2 p1, i2 p2, i2 p3)
{
    return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

bool pixel_check(i2 _p, i2 _p1, i2 _p2, i2 _p3)
{
    int int1, int2, int3;
    bool has_neg, has_pos;

    int1 = orient(_p, _p1, _p2);
    int2 = orient(_p, _p2, _p3);
    int3 = orient(_p, _p3, _p1);

    has_neg = (int1 < 0) || (int2 < 0) || (int3 < 0);
    has_pos = (int1 > 0) || (int2 > 0) || (int3 > 0);

    return !(has_neg && has_pos);
    return true;
}

void SWR::Renderer::draw_triangle(i2 _p1, i2 _p2, i2 _p3, PX32 _px)
{
    int x_min = Min(_p1.x, _p2.x, _p3.x);
    int x_max = Max(_p1.x, _p2.x, _p3.x);

    int y_min = Min(_p1.y, _p2.y, _p3.y);
    int y_max = Max(_p1.y, _p2.y, _p3.y);

    for(int x = x_min; x < x_max; x++)
    {
        for(int y = y_min; y < y_max; y++)
        {
            if(pixel_check({x, y}, _p1, _p2, _p3))
                *(buffer.at(x, y)) = _px;
        }
    }

}

void SWR::Renderer::clear(PX32 _px)
{
    for(int i = 0; i < buffer.count_pixels(); i++)
    {
        buffer.buf[i] = _px;
    }
}


void SWR::Renderer::paste_bitmap(Bitmap& _bmp, i2 _pos)
{
    if(_bmp.get_format() != buffer.pixel_format)
    {
        Print::ln("ERROR: Tried to paste bitmap onto SWR::Buf with non-matching pixel formats.");
        throw;
    }

    for(int x = 0; x < (int)_bmp.w(); x++)
    {
        for(int y = 0; y < (int)_bmp.h(); y++)
        {   
            *(buffer.at(_pos.x + x, _pos.y + y)) = _bmp[x, y];
            // *((*this)[_pos.x + x, _pos.y + y]) = 0xffffffff;
        }
    }
}

void SWR::Renderer::paste_bitmap_with_mask(Bitmap& _bmp, i2 _bmp_pos, i2 _mask_pos, i2 _mask_size)
{
    if(_bmp.get_format() != buffer.pixel_format)
    {
        Print::ln("ERROR: Tried to paste bitmap onto SWR::Buf with non-matching pixel formats.");
        throw;
    }

    BoxIntersection1D intersection_x;
    intersection_x.set_box(_mask_pos.x, _mask_size.x, _bmp_pos.x, _bmp.w());

    BoxIntersection1D intersection_y;
    intersection_y.set_box(_mask_pos.y, _mask_size.y, _bmp_pos.y, _bmp.h());


    for(int x = 0; x < intersection_x.size_intersection; x++)
    {
        for(int y = 0; y < intersection_y.size_intersection; y++)
        {
            int x_real  =   x + _mask_pos.x + intersection_x.cutting_coord_dest;
            int y_real  =   y + _mask_pos.y + intersection_y.cutting_coord_dest;

            int x_src   =   x + intersection_x.cutting_coord_src;
            int y_src   =   y + intersection_x.cutting_coord_src;

            *( buffer.at(x_real, y_real) ) = _bmp[x_src, y_src];
            // *((*this)[_pos.x + x, _pos.y + y]) = 0xffffffff;
        }
    }
}

    
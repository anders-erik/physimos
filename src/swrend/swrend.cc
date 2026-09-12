
#include "lib/print.hh"

#include "lib/minmax.hh"

#include "swrend/line.hh"

#include "swrend.hh"



void SWR::Buf::draw_point(i2 _p, PX32 _px)
{
    if(is_in_bounds(_p))
        *((*this)[_p.x, _p.y]) = _px;
    else
        Print::ln("WARN: tried drawing point outseide bitmap bounds.");
}


/** Draw pixelated line using kx + m where pixels are drawn using step sizes related to the k-value to generate the intermediate pixels when the slope is large. */
void SWR::Buf::draw_line(i2 _p1, i2 _p2, PX32 _px)
{

    if(!is_in_bounds(_p1) || !is_in_bounds(_p2))
    {
        Print::ln("WARN: tried drawing line outside bitmap bounds.");
        return;
    }

    // Vertical line
    if(_p1.x == _p2.x)
    {
        uint x = _p1.x;
        uint y_min = _p1.y > _p2.y ? _p2.y : _p1.y;
        uint y_max = _p1.y > _p2.y ? _p1.y : _p2.y;

        for(uint y = y_min; y < y_max; y++)
            *((*this)[x, y]) = _px;
    }

    // Horizontal line
    if(_p1.y == _p2.y)
    {
        uint y = _p1.y;
        uint x_min = _p1.x > _p2.x ? _p2.x : _p1.x;
        uint x_max = _p1.x > _p2.x ? _p1.x : _p2.x;

        for(uint x = x_min; x < x_max; x++)
            *((*this)[x, y]) = _px;
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
        *((*this)[(int)x, (int)y]) = _px;
    }
}

void SWR::Buf::draw_rectangle(i2 _p1, i2 _p2, PX32 _px)
{
    int x_min = Min(_p1.x, _p2.x);
    int x_max = Max(_p1.x, _p2.x);

    int y_min = Min(_p1.y, _p2.y);
    int y_max = Max(_p1.y, _p2.y);

    for(int x = x_min; x < x_max; x++)
    {
        for(int y = y_min; y < y_max; y++)
        {
            *((*this)[x, y]) = _px;
        }
    }

}

void SWR::Buf::draw_triangle_no_fill(i2 _p1, i2 _p2, i2 _p3, PX32 _px)
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

void SWR::Buf::draw_triangle(i2 _p1, i2 _p2, i2 _p3, PX32 _px)
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
                *((*this)[x, y]) = _px;
        }
    }

}

void SWR::Buf::clear(PX32 _px)
{
    for(int i = 0; i < count_pixels(); i++)
    {
        buf[i] = _px;
    }
}


void SWR::Buf::bm_paste(Bitmap& _bmp, i2 _pos)
{
    if(_bmp.get_format() != pixel_format)
    {
        Print::ln("ERROR: Tried to paste bitmap onto SWR::Buf with non-matching pixel formats.");
        throw;
    }

    for(int x = 0; x < (int)_bmp.w(); x++)
    {
        for(int y = 0; y < (int)_bmp.h(); y++)
        {   
            *((*this)[_pos.x + x, _pos.y + y]) = _bmp[x, y];
            // *((*this)[_pos.x + x, _pos.y + y]) = 0xffffffff;
        }
    }
}
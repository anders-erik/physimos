
#include "lib/print.hh"

#include "swrend.hh"



void SWR::Buf::draw_point(i2 _p, PX32 _px)
{
    if(is_in_bounds(_p))
        *((*this)[_p.x, _p.y]) = _px;
    else
        Print::ln("WARN: tried drawing point outseide bitmap bounds.");
}

void SWR::Buf::clear(PX32 _px)
{
    for(int i = 0; i < count_pixels(); i++)
    {
        buf[i] = _px;
    }
}


void SWR::Buf::paste(Bitmap& _bmp, i2 _pos)
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
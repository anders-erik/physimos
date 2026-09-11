
#include "lib/print.hh"

#include "swrend.hh"



void SWR::Buf::draw_point(i2 _p, uint32_t _px)
{
    if(is_in_bounds(_p))
        *((*this)[_p.x, _p.y]) = _px;
    else
        Print::ln("WARN: tried drawing point outseide bitmap bounds.");
}

void SWR::Buf::clear(uint32_t _px)
{
    for(int i = 0; i < count_pixels(); i++)
    {
        buf[i] = _px;
    }
}
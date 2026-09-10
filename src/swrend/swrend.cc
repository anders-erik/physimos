
#include "lib/print.hh"

#include "swrend.hh"



void SWRend::draw_point(i2 _p, uint32_t _px)
{
    if(is_in_bounds(_p))
        *((*this)[_p.x, _p.y]) = _px;
    else
        Print::ln("WARN: tried drawing point outseide bitmap bounds.");
}
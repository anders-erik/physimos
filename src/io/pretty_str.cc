
#include "lib/err.hh"

#include "io/pretty_str.hh"
#include "pretty_str.hh"

typedef unsigned char uchar;

Str Pretty::
StrS(bool _bool)
{
    return _bool ? "true" : "false";
}


Str Pretty::
StrS(unsigned char uch)
{
    return Str::SI((long long) uch);
}

Str Pretty::
StrS(long long _integer)
{
    return Str::SI(_integer);
}

Str Pretty::
StrS(int _integer)
{
    return Str::SI(_integer);
}

Str Pretty::
StrS(const Err & err)
{
    Str str_err_s =  Str{"err_s = "}   + Err::to_str(err.severity);
    Str str_err_m =  Str{"; err_m = "} + Err::to_str(err.module);
    Str str_err_t =  Str{"; err_t = "} + Err::to_str(err.type);
    Str str_errno =  Str{"; ERRNO = "} + Str::SI(err.ERRNO);
    return  str_err_s 
            + str_err_m
            + str_err_t
            + str_errno;
}


Str Pretty::
StrM(Str& indent, bool _bool)
{
    return indent + StrS(_bool) + "\n";
}

Str Pretty::StrM(Str & indent, int _int)
{
    return indent + Str::SI(_int) + "\n";
}

Str Pretty::StrM(Str & indent, uint _uint)
{
    return indent + Str::UI(_uint) + "\n";
}

Str Pretty::StrM(Str & indent, char ch)
{
    return indent + Str::CH( ch) + "\n";
}

Str Pretty::StrM(Str & indent, double dbl)
{
    return indent + Str::FL(    (float) dbl, 
                                (uchar)2, 
                                Str::FloatRep::Fixed) + "\n";
}

Str Pretty::
StrM(Str& indent, const Err & err)
{
    Str str_err_s =  indent + Str{"err_s = "} + Err::to_str(err.severity)  + "\n";
    Str str_err_m =  indent + Str{"err_m = "} + Err::to_str(err.module)    + "\n";
    Str str_err_t =  indent + Str{"err_t = "} + Err::to_str(err.type)      + "\n";
    Str str_errno =  indent + Str{"ERRNO = "} + Str::SI(err.ERRNO)         + "\n";
    return {  str_err_s 
            , str_err_m
            , str_err_t
            , str_errno } ;
}

Str Pretty::StrM(Str & indent, Str& str)
{
    return indent + str + "\n";
}


Str Pretty::Spaces(int count)
{
    Str spaces;
    spaces.reserve(count);
    spaces.fill_alloc(' ');
    return spaces;
}

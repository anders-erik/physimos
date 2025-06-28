
#include "lib/err.hh"

#include "io/pretty_str.hh"
#include "pretty_str.hh"


Str Pretty::
StrSL(bool _bool)
{
    return _bool ? "true" : "false";
}

Str Pretty::StrML(unsigned char indent_count, bool _bool)
{
    return Pretty::Spaces(indent_count) + StrSL(_bool);
}

Str Pretty::StrSL(unsigned char uch)
{
    return Str::SI((long long) uch);
}

Str Pretty::
StrSL(long long _integer)
{
    return Str::SI(_integer);
}


Str Pretty::StrSL(int _integer)
{
    return Str::SI(_integer);
}

Str Pretty::StrSL(const Err & err)
{
    Str str_err_s =  Str{"err_s = "} + Pretty::StrSL((unsigned char)err.severity);
    Str str_err_m =  Str{"; err_m = "} + Pretty::StrSL((unsigned char)err.module);
    Str str_err_t =  Str{"; err_t = "} + Pretty::StrSL((unsigned char)err.type);
    Str str_errno =  Str{"; ERRNO = "} + Pretty::StrSL((unsigned char)err.ERRNO);
    return  str_err_s 
            + str_err_m
            + str_err_t
            + str_errno;
}

Str Pretty::StrML(unsigned char indent_count, const Err & err)
{
    Str indent = Pretty::Spaces(indent_count);

    Str str_err_s =  indent + Str{"err_s = "} + Pretty::StrSL((unsigned char)err.severity)  + "\n";
    Str str_err_m =  indent + Str{"err_m = "} + Pretty::StrSL((unsigned char)err.module)    + "\n";
    Str str_err_t =  indent + Str{"err_t = "} + Pretty::StrSL((unsigned char)err.type)      + "\n";
    Str str_errno =  indent + Str{"ERRNO = "} + Pretty::StrSL((unsigned char)err.ERRNO)     + "\n";
    return  str_err_s 
            + str_err_m
            + str_err_t
            + str_errno;
}


Str Pretty::Spaces(int count)
{
    Str spaces;
    spaces.reserve(count);
    spaces.fill_alloc(' ');
    return spaces;
}

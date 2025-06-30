#include "err.hh"
// #include <cerrno>


Str Err::to_str()
{
    Str str_err;

    str_err += Err::to_str(severity);
    str_err += Err::to_str(module);

    if(type == err_t::ERRNO)
    {
        str_err += "ERRNO=";
        str_err += Str::SI(ERRNO);
        if(ERRNO == 2)
            str_err += ": Failed to open file. ";
        // else
            // strerror_r( errno, buffer, 256 );
    }
    else
    {
        str_err += Err::to_str(type);
    }

    str_err += "Msg: ";
    str_err += message;

    return str_err;
}

Str Err::to_str(err_s s)
{
    switch (s)
    {

    case err_s::Critical    :   return "critical";
    case err_s::Error       :   return "error";
    case err_s::Warn        :   return "warn";
    default:                    return "_";

    }
}

Str Err::to_str(err_t t)
{
    switch (t)
    {

    case err_t::ERRNO    :   return {"ERRNO"};
    default:                 return "_";

    }
}

Str Err::to_str(err_m m)
{
    switch (m)
    {

    case err_m::Lib    :   return "lib";
    default:               return "_";

    }
}

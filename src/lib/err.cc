#include "err.hh"



Str Err::to_str()
{
    Str str_err;


    if(severity == err_s::Critical)
        str_err += "CRITICAL";
    else if(severity == err_s::Error)
        str_err += "err_s::Error; ";
    else if(severity == err_s::Warn)
        str_err += "WARN";
    else
        str_err += "UNKNOWN";


    if(type == err_t::ERRNO)
    {
        str_err += "ERRNO=";
        str_err += Str::SI(ERRNO);
        if(ERRNO == 2)
            str_err += ": Failed to open file. ";
    }


    str_err += "MSG: ";
    str_err += message;

    return str_err;
}
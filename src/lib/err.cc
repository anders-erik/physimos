#include "err.hh"



Str Err::err_s_to_str()
{
    if(severity == err_s::Critical)
        return "CRITICAL";
    else if(severity == err_s::Error)
        return "ERROR";
    else if(severity == err_s::Warn)
        return "WARN";
    else
        return "UNKNOWN";
}
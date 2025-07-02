#pragma once

#include "lib/res.hh"

struct Parse
{
    static float try_float(Str float_str)
    {
        try
        {
            return std::stof(float_str.to_c_str());
        }
        catch(const std::exception& e)
        {
            throw_err(__FILE__, __LINE__, e.what());
        }
    }

    static int try_int()
    {
        
    }


    static void throw_err(Str file_name, int line, Str message)
    {
        Str err_str = file_name + ", " + Str::SI(line) + ", " + message;
        throw std::runtime_error(err_str.to_c_str());
    }
};
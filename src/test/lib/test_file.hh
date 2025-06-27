
#pragma once

#include "test/test.hh"

#include "file.hh"

std::vector<UnitTestFn> cat_str = {


    []() -> UnitTestInfo 
    {
        UnitTestInfo info = {"Read /dev/null : waterfall"};

        Str path = "/dev/null";
        ResMove<Str> res_str = File::cat_as_str_core_xplat(path);

        if(res_str.has_value())
            return info.fail();

        if(!res_str.has_error())
            return info.fail();

        Err error = res_str.consume_error();
        
        bool severity = error.severity == err_s::Error;
        bool module = error.module == err_m::Lib;
        bool type = error.type == err_t::ERRNO;
        bool ERRNO = error.ERRNO == 2;
        if(!severity || !module || !type || !ERRNO)
            return info.fail();

        // println(error.message);
        
        return info.pass();
    },


};


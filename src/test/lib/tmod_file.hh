
#pragma once

#include "test/test.hh"

#include "file.hh"


UnitTestArray cat_str = {

    {   "Read /dev/null : waterfall",
    [](UnitTest& utest) -> UnitTest& 
    {
        Str path = "/dev/null";
        ResMove<Str> res_str = File::cat_as_str_core_xplat(path);

        if(res_str.has_value())
            return utest.fail();

        if(!res_str.has_error())
            return utest.fail();

        Err error = res_str.consume_error();
        
        bool severity = error.severity == err_s::Error;
        bool module = error.module == err_m::Lib;
        bool type = error.type == err_t::ERRNO;
        bool ERRNO = error.ERRNO == 2;
        if(!severity || !module || !type || !ERRNO)
            return utest.fail();

        // println(error.message);
        
        return utest.pass();
    }},


};


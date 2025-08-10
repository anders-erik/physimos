
#pragma once

#include "test/test.hh"

#include "file.hh"


UnitTestArray cat_str = {

    {"Read /dev/null : error exists",
    [](UnitTest& utest) -> void 
    {
        Str path = "/dev/null";
        ResMove<Str> res_str = File::cat_as_str_core_xplat(path);

        utest.assert_custom(   res_str.has_error(),
                        true);
    }},


    {"Read /dev/null : err == err_target",
    [](UnitTest& utest) -> void 
    {
        Str path = "/dev/null";
        ResMove<Str> res_str = File::cat_as_str_core_xplat(path);
        Err err = res_str.consume_error();

        Err err_target;
        err_target.severity = err_s::Error;
        err_target.module = err_m::Lib;
        err_target.type = err_t::ERRNO;
        err_target.ERRNO = 2;   // expect 2


        utest.assert_custom(   err, 
                        err_target  );
    }},



};


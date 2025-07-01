#pragma once

#include "lib/print.hh"
#include "lib/file.hh"

#include "io/json/json.hh"

Str data_dir = "src/test/io/json/data/";


/** Read a file relative to the testing data (`src/test/io/json/data/`)
    If error is created during call the error message is returned. */
Str
cat_json_source(Str file_path)
{
    Str absolute_path = data_dir + file_path;
    ResMove<Str> json_source_res = File::cat_as_str_core_xplat(absolute_path);
    if(json_source_res.has_error())
    {
        Print::line("Failed to cat json source.", json_source_res.consume_error().to_str());
        return "invalid_json_source";
    }
    else
        return json_source_res.consume_value();
};


/** Read and parse the json test-data file.
    If error is created during call an empty json object is returned. */
Json &&
parse_json_test_file(Str file_path)
{
    Str json_source = cat_json_source(file_path);

    ResMove<Json> jroot_res = Json::parse(json_source);
    if(jroot_res.has_error())
    {
        Print::lines("Failed to parse json source.", jroot_res.consume_error().to_str());
        return {};
    }

    return jroot_res.consume_value();
};



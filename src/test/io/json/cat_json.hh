#pragma once

#include "lib/print.hh"
#include "lib/file.hh"

#include "io/json/json.hh"

Str data_path = "src/test/io/json/data/";


/** Read a file relative to the testing data (`src/test/io/json/data/`)
    If error is created during call the error message is returned. */
Str
cat_json_source(Str file_path)
{
    Str absolute_path = data_path + file_path;
    ResMove<Str> json_source_res = File::cat_as_str_core_xplat(absolute_path);
    if(json_source_res.has_error())
    {
        Print::lines("Failed to cat json source.", json_source_res.consume_error().str());
        return "";
    }
    else
        return json_source_res.consume_value();
};


/** Read and parse the json test-data file.
    If error is created during call an empty json object is returned. */
JsonVar 
cat_and_parse_json(Str file_path)
{
    Str json_source = cat_json_source(file_path);

    ResMove<JsonVar> jroot_res = Json::parse(json_source);
    if(jroot_res.has_error())
    {
        Print::lines("Failed to parse json source.", jroot_res.consume_error().str());
        return JsonVar{};
    }

    return jroot_res.consume_value();
};
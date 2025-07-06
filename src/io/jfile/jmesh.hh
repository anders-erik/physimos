#pragma once

#include <string>

#include "lib/str.hh"
#include "lib/print.hh"

#include "lib/file.hh"
#include "io/json/json.hh"

#include "scene/mesh.hh"

struct JMesh
{
    static ResMove<Mesh> load_mesh_core(Str path)
    {
        ResMove<Str> json_source_res = File::cat_as_str_core_xplat(path);
        if(json_source_res.has_error())
        {
            Print::err({});
            return Err{};
        }

        Str json_source = json_source_res.consume_value();

        ResMove<Json> json_var_res = Json::parse(json_source);
        if(json_var_res.has_error())
        {
            Print::err({});
            return Err{};
        }


        Json json_root = json_var_res.consume_value();

        OptPtr<Json> mesh_array_opt = json_root.object_find("mesh");
        if(mesh_array_opt.is_null())
        {
            Print::ln("error jmesh: mesh null");
            return Err{};
        }
        Json* mesh_array = mesh_array_opt.get_ptr();


        Mesh mesh;

        for(auto& var : mesh_array->get_array())
        {
            auto& var_str = var.get_string();

            if(var_str == "sheet")
            {
                mesh.sheet();
            }
            else if(var_str == "center")
            {
                mesh.center();
            }
            else if(var_str == "cube")
            {
                mesh.cube_centered();
            }
        }

        
        return {MV(mesh)};
    }
};
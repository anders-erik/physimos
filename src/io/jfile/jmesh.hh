#pragma once

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

        ResMove<JsonVar> json_var_res = Json::parse(json_source);
        if(json_var_res.has_error())
        {
            Print::err({});
            return Err{};
        }

        Mesh mesh;

        JsonVar json_root = json_var_res.consume_value();

        JsonVar mesh_object_var = json_root.find_in_object("mesh");

        // Str sheet_key = Json::serialize(mesh_object.find_in_object("sheet").first);
        // j_string sheet_key = mesh_object_var.find_in_object("sheet").first;
    
        json_object_variants mesh_object = json_root.find_in_object("mesh").get_object();

        // for(auto& kv : json_root.find_in_object("mesh").get_object()) // MEMORY BREAKS HERE
        for(auto& kv : mesh_object)
        {
            // std::cout << "\"" << kv.first << "\"" << std::endl;
            // std::cout << "\"" << "sheet" << "\"" << std::endl;

            if(kv.first == "sheet")
            {
                mesh.sheet();
            }
            else if(kv.first == "center")
            {
                mesh.center();
            }
        }

        
        return {MV(mesh)};
    }
};
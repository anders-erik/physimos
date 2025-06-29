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
        ResMove<Str> json_source = File::cat_as_str_core_xplat(path);
        if(json_source.has_error())
        {
            Print::line("Err 1");
            return Err{};
        }

        ResMove<JsonVar> json_var_res = Json::parse(json_source.consume_value());
        if(json_var_res.has_error())
        {
            Print::line("Err 2");
            return Err{};
        }

        Mesh mesh;

        JsonVar json_var = json_var_res.consume_value();


        for(auto& kv : json_var.find_in_object("mesh").second.get_object())
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
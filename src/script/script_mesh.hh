#pragma once

#include "res.hh"
#include "str.hh"
#include "str_util.hh"
#include "file.hh"
#include "print.hh"
#include "arr.hh"

#include "script/parse.hh"

#include "scene/mesh.hh"

enum class MeshMethod {
    sheet,
    center,
};


struct MeshCall
{
    MeshMethod  method;
    MeshContext context;
};

struct ScriptMesh
{
    static MeshCall
    parse_line(Str line)
    {

        if(line.substr(0, 5) == "sheet")
        {
            // float w = Parse::try_float(line.substr(5, 8));
            return { MeshMethod::sheet, SheetContext{2.0f, 10} } ;
        }
        else
        {

        }

    }


    static Arr<MeshCall> 
    parse_script(Str script)
    {
        Arr<MeshCall> method_calls;

        Arr<Str> lines = StrUtil::split(script, ';');

        for(SizeArr i=0; i<lines.count(); i++)
        {
            MeshCall mesh_call = ScriptMesh::parse_line(lines[i]);

            method_calls.push_back(mesh_call);
        }

        return method_calls;
    }
    

    static ResMove<Mesh> 
    run(Str path_core)
    {
        ResMove<Str> script = File::cat_as_str_core_xplat(path_core);
        if(script.has_error())
            return {MV(script.consume_error())};

        Arr<MeshCall> method_calls = ScriptMesh::parse_script(script.consume_value());

        Mesh mesh;

        for(uint i=0; i<method_calls.count(); i++)
        {
            switch (method_calls[i].method)
            {
            case MeshMethod::sheet:
                mesh.sheet(std::get<SheetContext>(method_calls[i].context));
                break;
            
            default:
                break;
            }
        }
        return {MV(mesh)};
    }
};
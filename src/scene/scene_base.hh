
#pragma once


typedef unsigned char SID;

enum SceneType {
    D2,
    D3,
};


struct SceneBase 
{

    SID scene_id;
    Str name;
    SceneType type;


    SceneBase() {};

    SceneBase(SID id, Str name, SceneType type) 
        :   scene_id {id}, 
            name {name}, 
            type {type} 
    {
    };

    void set_name(Str new_name)
    {
        name = new_name;
    }

    Str& get_name()
    {
        return name;
    }


    bool is_2D() 
    {
        if(type == SceneType::D2)
            return true;
        
        return false;
    };
    bool is_3D() 
    {
        if(type == SceneType::D3)
            return true;
        
        return false;
    };
};

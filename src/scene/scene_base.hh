
#pragma once


typedef unsigned char SceneID;

enum SceneType {
    D2,
    D3,
};


struct SceneBase 
{

    SceneID scene_id;
    SceneType type;


    SceneBase() {};

    SceneBase(SceneID id, SceneType type) 
        :   scene_id {id}, 
            type {type} 
    {
    };

    bool is_2d() 
    {
        if(type == SceneType::D2)
            return true;
        
        return false;
    };
    bool is_3d() 
    {
        if(type == SceneType::D3)
            return true;
        
        return false;
    };
};

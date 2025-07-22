#pragma once

#include "lib/res.hh"

#include "json_types.hh"



/** Thick tag-union-like structure implemented using std::variant. */
struct Json
{
    json_type       type;
    json_variant    var;
    
    // char buf[56];
    
    constexpr
    Json(                   ) : type {json_type::null}        , var {nullptr}    {}
    Json(j_null   new_null  ) : type {json_type::null}        , var {new_null}   {}
    Json(j_bool   new_bool  ) : type {json_type::boolean}     , var {new_bool}   {}
    Json(j_int    new_int   ) : type {json_type::number_int}  , var {new_int}    {}
    Json(j_float  new_float ) : type {json_type::number_float}, var {new_float}  {}
    Json(j_string new_string) : type {json_type::string}      , var {new_string} {}
    Json(j_array  new_array ) : type {json_type::array}       , var {new_array}  {}
    Json(j_object new_object) : type {json_type::object}      , var {new_object} {}

    Json(json_type new_type)
    {
        type = new_type;
        
        switch (new_type){

        case json_type::boolean :
            var = (bool)false;
            break;

        case json_type::number_int :
            var = 0;
            break;

        case json_type::number_float :
            var = 0.0;
            break;

        case json_type::string :
            var = Str();
            break;

        case json_type::array :
            var = j_array();
            break;

        case json_type::object :
            var = j_object();
            break;
        
        default:
            var = nullptr;
            break;
        }
    };


    
    void set_bool(j_bool new_bool)
    {
        type = json_type::boolean;
        var = new_bool;
    }
    void set_int(j_int new_int)
    {
        type = json_type::number_int;
        var = new_int;
    }
    void set_float(j_float new_float)
    {
        type = json_type::number_float;
        var = new_float;
    }
    void set_string(j_string new_string)
    {
        type = json_type::string;
        var = new_string;
    }
    void set_array()
    {
        type = json_type::array;
        var = j_array();
    }
    void set_object()
    {
        type = json_type::object;
        var = j_object();
    }

    
    json_type get_type()    {return type; }

    bool is_null()          {return type == json_type::null         ; }
    bool is_bool()          {return type == json_type::boolean      ; }
    bool is_number_int()    {return type == json_type::number_int   ; }
    bool is_number_float()  {return type == json_type::number_float ; }
    bool is_string()        {return type == json_type::string       ; }
    bool is_object()        {return type == json_type::object       ; }
    bool is_array()         {return type == json_type::array        ; }



    j_bool& get_bool()
    {
        if(is_bool())
            return std::get<j_bool>(var);
        throw std::runtime_error("Error trying to get value from variant. j_bool. ");
    };
    j_int& get_int()
    {
        if(is_number_int())
            return std::get<j_int>(var);
        throw std::runtime_error("Error trying to get value from variant. j_int. ");
    };
    j_float& get_float()
    {
        if(is_number_float())
            return std::get<j_float>(var);
        throw std::runtime_error("Error trying to get value from variant. j_float. ");
    };
    j_string& get_string()
    {
        if(is_string())
            return std::get<j_string>(var);
        throw std::runtime_error("Error trying to get value from variant. j_string. ");
    };
    j_array& get_array()
    {
        if(is_array())
            return std::get<j_array>(var);
        throw std::runtime_error("Error trying to get value from variant. j_array. ");
    };
    j_object& get_object()
    {
        if(is_object())
            return std::get<j_object>(var);
        throw std::runtime_error("Error trying to get value from variant. j_object. ");
    };


    OptPtr<Json>    object_find(j_string str_to_match);
    void            object_push(const j_kv& kv);


    /** Throws on error */
    void    try_parse(const Str& json_source);
    /** Throws on error */
    Str     try_serialize();


    static ResMove<Json>    parse(const Str& json_source);
    static ResMove<Str>     serialize(const Json& json_var);
};


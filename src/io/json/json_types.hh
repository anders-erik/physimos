#pragma once

#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <variant>
#include <memory>

#include "lib/opt.hh"
#include "lib/res.hh"

#include "lib/str.hh"

struct JsonVar; // forward declare


#define QUOTATION_MARK      '\u0022'
#define SOLLIDUS            '\u002F'
#define SOLLIDUS_REVERSE    '\u005C'

#define SPACE               '\u0020'
#define TAB                 '\u0009'
#define NEW_LINE            '\u000A'
#define CARRIAGE_RETURN     '\u000D'


typedef std::string     JSS; // Json source/serialized string

typedef std::string     j_string;
typedef bool            j_bool;
typedef std::nullptr_t  j_null;
typedef double          j_float;
typedef long long int   j_int;



enum class json_type 
{
    null,
    boolean,
    number_int,
    number_float,
    string,

    object,
    array,
};


constexpr bool is_literal(json_type type)
{
    return  type == json_type::null         ||
            type == json_type::boolean      ||
            type == json_type::number_int   ||
            type == json_type::number_float ||
            type == json_type::string;
}




typedef std::pair<j_string, JsonVar>    j_kv;
typedef std::map<j_string, JsonVar>     j_object;
typedef std::vector<JsonVar>            j_array;
typedef std::variant<   j_string, 
                        j_bool,
                        j_null, 
                        j_float,
                        j_int,
                        j_array,
                        j_object>       json_variant;



/** Thick tag-union-like structure implemented using std::variant. */
struct JsonVar
{
    json_type type = json_type::null;
    json_variant variant_;
    
    // char buf[56];
    
    JsonVar() = default;
    JsonVar(j_null new_null)    : type {json_type::null}        , variant_ {new_null}   {}
    JsonVar(j_bool new_bool)    : type {json_type::boolean}     , variant_ {new_bool}   {}
    JsonVar(j_int new_int)      : type {json_type::number_int}  , variant_ {new_int}    {}
    JsonVar(j_float new_float)  : type {json_type::number_float}, variant_ {new_float}  {}
    JsonVar(j_string new_string): type {json_type::string}      , variant_ {new_string} {}
    JsonVar(j_array new_array)  : type {json_type::array}       , variant_ {new_array}  {}
    JsonVar(j_object new_object): type {json_type::object}      , variant_ {new_object} {}

    JsonVar(json_type new_type){
        type = new_type;
        
        switch (new_type){

        case json_type::boolean :
            variant_ = (bool)false;
            break;

        case json_type::number_int :
            variant_ = 0;
            break;

        case json_type::number_float :
            variant_ = 0.0;
            break;

        case json_type::string :
            variant_ = "";
            break;

        case json_type::array :
            variant_ = j_array();
            break;

        case json_type::object :
            variant_ = j_object();
            break;
        
        default:
            variant_ = nullptr;
            break;
        }
    };


    
    void set_bool(j_bool new_bool)
    {
        type = json_type::boolean;
        variant_ = new_bool;
    }
    void set_int(j_int new_int)
    {
        type = json_type::number_int;
        variant_ = new_int;
    }
    void set_float(j_float new_float)
    {
        type = json_type::number_float;
        variant_ = new_float;
    }
    void set_float(j_string new_string)
    {
        type = json_type::string;
        variant_ = new_string;
    }
    void set_array()
    {
        type = json_type::array;
        variant_ = j_array();
    }
    void set_object()
    {
        type = json_type::object;
        variant_ = j_object();
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
            return std::get<j_bool>(variant_);
        throw std::runtime_error("Error trying to get value from variant. j_bool. ");
    };
    j_int& get_int()
    {
        if(is_number_int())
            return std::get<j_int>(variant_);
        throw std::runtime_error("Error trying to get value from variant. j_int. ");
    };
    j_float& get_float()
    {
        if(is_number_float())
            return std::get<j_float>(variant_);
        throw std::runtime_error("Error trying to get value from variant. j_float. ");
    };
    j_string& get_string()
    {
        if(is_string())
            return std::get<j_string>(variant_);
        throw std::runtime_error("Error trying to get value from variant. j_string. ");
    };
    j_array& get_array()
    {
        if(is_array())
            return std::get<j_array>(variant_);
        throw std::runtime_error("Error trying to get value from variant. j_array. ");
    };
    j_object& get_object()
    {
        if(is_object())
            return std::get<j_object>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_object_variants. ");
    };


    OptPtr<JsonVar> object_find(j_string str_to_match);
    void            object_push(const j_kv& kv);


    void            parse(const Str& json_source);
    ResMove<Str>    serialize();
};



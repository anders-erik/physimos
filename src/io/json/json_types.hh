#pragma once

#include <vector>
#include <map>
#include <unordered_map>
#include <stack>
#include <iostream>
#include <fstream>
#include <string>

#include <variant>
#include <memory>

#include "lib/opt.hh"
#include "lib/res.hh"

#include "lib/str.hh"

struct Json; // forward declare


#define QUOTATION_MARK      '\u0022'
#define SOLLIDUS            '\u002F'
#define SOLLIDUS_REVERSE    '\u005C'

#define SPACE               '\u0020'
#define TAB                 '\u0009'
#define NEW_LINE            '\u000A'
#define CARRIAGE_RETURN     '\u000D'


typedef Str             j_string;
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




typedef std::pair<j_string, Json>    j_kv;
typedef std::map<j_string, Json>     j_object;
typedef std::vector<Json>            j_array;
typedef std::variant<   j_string, 
                        j_bool,
                        j_null, 
                        j_float,
                        j_int,
                        j_array,
                        j_object>       json_variant;




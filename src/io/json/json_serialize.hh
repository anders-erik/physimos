#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip> // setprecision
#include <climits> // LLONG_MAX

#include "json_types.hh"


/** They type of whitespace output of th serializer */
enum class serial_ws : int {
    minimized,
    oneline,
    new_lines,
};

/** Floating point representation format. */
enum class float_representation {
    fixed,          // 0.0 -> 0.0000000     (precision == 7)
    fixed_trimmed,  // 0.0 -> 0.0           (precision == 7)
    scientific,     // 0.0 -> 0.0000000e+00 (precision == 7)
};

struct FloatFormat {
    float_representation    rep;        // decimal or scientific
    size_t                  precision;  // Number of digits past decimal point

    FloatFormat() 
        : rep       {float_representation::scientific}
        , precision {8}
    {};

    FloatFormat(float_representation rep, size_t precision)
        : rep{rep}
        , precision {precision}
    {};
};


/** Json Serialization confguration object.  */
struct JSerialConfig 
{
    serial_ws   whitespace;
    size_t      indent_space_count;
    FloatFormat float_format;

    JSerialConfig() 
        :   whitespace {serial_ws::new_lines}
        ,   indent_space_count {4}
        ,   float_format {FloatFormat()}
    {};
    JSerialConfig(serial_ws _whitespace) 
        : whitespace {_whitespace}
    {};
    JSerialConfig(size_t _indent_space_count) 
        : indent_space_count {_indent_space_count} 
    {};
    JSerialConfig(serial_ws _whitespace, size_t _indent_space_count) 
        : whitespace {_whitespace}
        , indent_space_count {_indent_space_count}
    {};
    JSerialConfig(serial_ws _whitespace, size_t _indent_space_count, FloatFormat float_format) 
        : whitespace {_whitespace}
        , indent_space_count {_indent_space_count}
        , float_format {float_format}
    {};
};


class JsonSerializer {

    JSerialConfig   config;

    int             recursive_depth = 0; // Container depth for correct indentation

    Str     result_string = "";


    Str build_string(Json json_var);

    void try_add_new_line(Str& str);
    void try_add_indent(Str& str);
    void try_remove_trailing_comma_object(Str& str);
    void try_remove_trailing_comma_array(Str& str);
    void add_trailing_comma_to_object(Str& str);
    void add_trailing_comma_to_array(Str& str);

    static void add_spaces(Str& str, int space_count);

    /** Supports different precision and scientific notation */
    static Str float_to_json_representation(j_float float_, FloatFormat float_format);
    /** Converts a Str to is JSON equivelence. e.g. <I "mean" it..> --> <"I \"mean\" it.."> */
    static Str string_to_json_representation(Str cpp_string);

public:

    JsonSerializer() {};
    JsonSerializer(JSerialConfig config): config { config } {};

    /** Returns full json structure as string.  */
    Str serialize(const Json& json_root);



    void set_config(JSerialConfig config) {this->config = config;};
    
};

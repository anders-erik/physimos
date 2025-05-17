#pragma once

#include "physon_types.hh"


// Value containers
struct Json;

typedef std::pair<json_string, Json> json_kv_value;
typedef std::vector<Json> json_array_value;
typedef std::vector<json_kv_value> json_object_value;



union JsonValue {
    json_string     string_;
    json_bool       bool_;
    json_null       null_;
    json_float      float_;
    json_int        int_ = 0;

    json_array_value      array_;
    json_object_value     object_;
    // json_kv_value     kv_;

    JsonValue() : null_ { nullptr } {};
    JsonValue(json_bool new_bool) : bool_ {new_bool} {};
    JsonValue(json_int new_int) : int_ {new_int} {};
    JsonValue(json_float new_float) : float_ {new_float} {};
    JsonValue(json_string new_str) : string_ {new_str} {};

    ~JsonValue() {};

    // JsonValue(json_string new_str) : string_ {new_str} {};
};

struct Json
{
    JSON_TYPE type;
    JsonValue value;

    // Json(): type {JSON_TYPE::NULL_}, value { nullptr } {};
};


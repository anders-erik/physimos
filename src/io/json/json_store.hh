#pragma once

#include "physon_types.hh"


void print_store_sizes();


/** JSON tag-id structure with id used to retrieve actual data from store. Store owned by client-interface. */
struct JsonWrapper {

    int store_id = 0; // unique identifier for specific type
    JSON_TYPE type = JSON_TYPE::NONE;

    JsonWrapper () : store_id {0},type {JSON_TYPE::NONE} {};
    JsonWrapper (JSON_TYPE _type) : type {_type} {};
    JsonWrapper (int _store_id, JSON_TYPE _type) : store_id {_store_id}, type {_type} {};

    // equal if identical type and store id
    bool operator==(const JsonWrapper& other) const {
        return type == other.type && store_id == other.store_id;
    }

    bool is_bool();
};


// Wrapper containers
typedef std::pair<json_string, JsonWrapper> json_kv_wrap;
typedef std::vector<JsonWrapper>            json_array_wrap;
/** Wraps only kv_wraps */
typedef std::vector<JsonWrapper>            json_object_wrap;




/** 
    JSON data storage. 
    The json object/array vectors store the array/object tree.
 */
struct json_store {

    std::vector<json_bool>      bools;
    std::vector<json_int>       integers;
    std::vector<json_float>     floats;
    std::vector<std::string>    strings;
    

    std::vector<json_array_wrap>     arrays;
    std::vector<json_object_wrap>    objects;
    std::vector<json_kv_wrap>        kvs;


    // int add_bool(json_bool new_bool){
    //     bools.push_back(new_bool);
    //     return bools.size() - 1;
    // }
    // const json_bool& get_bool(int id){
    //     return bools[id];
    // }

    JsonWrapper new_integer(long long int value){
        integers.emplace_back(value);
        return JsonWrapper(integers.size()-1, JSON_TYPE::INTEGER);
    }
    json_int& get_integer(int id){
        return integers[id];
    }

    JsonWrapper new_float(double value){
        floats.emplace_back(value);
        return JsonWrapper(floats.size()-1, JSON_TYPE::FLOAT);
    }
    json_float& get_float(int id){
        return floats[id];
    }

    int add_string(std::string new_str){
        strings.push_back(new_str);
        return strings.size() - 1;
    }
    std::string& get_string(int id){
        return strings[id];
    }

    JsonWrapper new_array(){
        arrays.emplace_back();

        JsonWrapper array;
        array.store_id = arrays.size() - 1;
        array.type = JSON_TYPE::ARRAY;

        return array;
    }
    json_array_wrap& get_array(int id){
        return arrays[id];
    }

    JsonWrapper new_object(){
        objects.emplace_back();

        JsonWrapper object;
        object.store_id = objects.size() - 1;
        object.type = JSON_TYPE::OBJECT;

        return object;
    }
    json_object_wrap& get_object(int id){
        return objects[id];
    }
    JsonWrapper new_kv(json_string key){
        
        // Store key-string
        json_kv_wrap& kv = kvs.emplace_back();
        kv.first = key;

        JsonWrapper value (kvs.size()-1, JSON_TYPE::KV);

        return value;
    }
    json_kv_wrap& get_kv(int id){
        return kvs[id];
    }

    void clear() {
        integers.clear();
        floats.clear();
        strings.clear();
        objects.clear();
        arrays.clear();
    }

};




void print_store_sizes(){
    std::cout << " sizeof(json_string)  = "  << sizeof(json_string) << std::endl;
    std::cout << " sizeof(json_bool)    = "  << sizeof(json_bool) << std::endl;
    std::cout << " sizeof(json_null)    = "  << sizeof(json_null) << std::endl;
    std::cout << " sizeof(json_float)   = "  << sizeof(json_float) << std::endl;
    std::cout << " sizeof(json_int)     = "  << sizeof(json_int) << std::endl;
    std::cout << " sizeof(json_kv_wrap)      = "  << sizeof(json_kv_wrap) << std::endl;
    std::cout << " sizeof(json_array_wrap)   = "  << sizeof(json_array_wrap) << std::endl;
    std::cout << " sizeof(json_object_wrap)  = "  << sizeof(json_object_wrap) << std::endl;
}




struct ParserCursor {
    size_t index = 0;
    // JSON_PARSE_STATE state = JSON_PARSE_STATE::ROOT_BEFORE_VALUE;  // Keeps track of the current parsing state
    // json_element current_element;             // element cursor
    // json_element current_container;           // container cursor
    // JSON_TYPE current_container_type; // current container type (array or object)
    std::stack<JsonWrapper> container_trace_wrapper; // stack of current container level

};

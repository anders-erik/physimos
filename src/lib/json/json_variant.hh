#pragma once


#include <type_traits>

#include "physon_types.hh"



struct JsonVariant; // forward declare
enum class json_type {
    null,
    boolean,
    number_int,
    number_float,
    string,

    object,
    array,
};
typedef std::pair<json_string, JsonVariant> json_kv_variant;
typedef std::vector<json_kv_variant> json_object_variants;
typedef std::vector<JsonVariant> json_array_variants;
/** Thick tag-union-like structure implemented using std::variant. */
struct JsonVariant {
    json_type type = json_type::null;
    std::variant<   json_string, 
                    json_bool,
                    json_null, 
                    json_float,
                    json_int,
                    json_array_variants,
                    json_object_variants
                > variant_ = nullptr;
    
    JsonVariant()  {};
    JsonVariant(json_bool new_bool) : type {json_type::boolean}, variant_ {new_bool} {};
    JsonVariant(json_int new_int) : type {json_type::number_int}, variant_ {new_int} {};
    JsonVariant(json_float new_float) : type {json_type::number_float}, variant_ {new_float} {};
    JsonVariant(json_string new_string) : type {json_type::string}, variant_ {new_string} {};
    // JsonVariant(json_string& new_string) : type {json_type::string}, variant_ {new_string} {};
    // JsonVariant(json_string&& new_string) : type {json_type::string}, variant_ {new_string} {};
    JsonVariant(json_array_variants new_array) : type {json_type::array}, variant_ {new_array} {};
    JsonVariant(json_object_variants new_object) : type {json_type::object}, variant_ {new_object} {};
    JsonVariant(json_type new_type){
        type = new_type;
        
        switch (new_type){

        case json_type::boolean :
            variant_ = false;
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
            variant_ = json_array_variants();
            break;

        case json_type::object :
            variant_ = json_object_variants();
            break;
        
        default:
            variant_ = nullptr;
            break;
        }
    };


    json_kv_variant new_kv(json_string key, JsonVariant value){
        json_kv_variant kv (key, value);
        return kv;
    };



    void set_bool(json_bool new_bool) {
        type = json_type::boolean;
        variant_ = new_bool;
    }
    void set_int(json_int new_int) {
        type = json_type::number_int;
        variant_ = new_int;
    }
    void set_float(json_float new_float) {
        type = json_type::number_float;
        variant_ = new_float;
    }
    void set_float(json_string new_string) {
        type = json_type::string;
        variant_ = new_string;
    }
    void set_array() {
        type = json_type::array;
        variant_ = json_array_variants();
    }
    void set_object() {
        type = json_type::object;
        variant_ = json_object_variants();
    }

    
    bool is_null() {return type == json_type::null ? true : false; };
    bool is_boolean() {return type == json_type::boolean ? true : false; };
    bool is_number_int() {return type == json_type::number_int ? true : false; };
    bool is_number_float() {return type == json_type::number_float ? true : false; };
    bool is_string() {return type == json_type::string ? true : false; };
    bool is_object() {return type == json_type::object ? true : false; };
    bool is_array() {return type == json_type::array ? true : false; };


    json_type get_type(){return type; };
    json_bool& get_bool(){
        if(type == json_type::boolean)
            return std::get<json_bool>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_bool. ");
    };
    json_int& get_int(){
        if(type == json_type::number_int)
            return std::get<json_int>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_int. ");
    };
    json_float& get_float(){
        if(type == json_type::number_float)
            return std::get<json_float>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_float. ");
    };
    json_string& get_string(){
        if(type == json_type::string)
            return std::get<json_string>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_string. ");
    };
    json_array_variants& get_array(){
        if(type == json_type::array)
            return std::get<json_array_variants>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_array_variants. ");
    };
    json_object_variants& get_object(){
        if(type == json_type::object)
            return std::get<json_object_variants>(variant_);
        throw std::runtime_error("Error trying to get value from variant. json_object_variants. ");
    };



    void push_array(JsonVariant& var_to_push){
        if(type == json_type::array){
            json_array_variants& array = get_array();
            array.push_back(var_to_push);
            return;
        }
        
        throw std::runtime_error("Error trying to push value to non-array."); 
    }
    void push_object(json_kv_variant& kv_to_push){
        if(type == json_type::object){
            json_object_variants& object = get_object();
            object.push_back(kv_to_push);
            return;
        }
        
        throw std::runtime_error("Error trying to push value to non-object."); 
    }

};







// helper type for overloaded visitor lambdas 
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void variant_playground(){
    std::cout << std::endl;

    // JsonVariant json_var;
    // json_var.set_bool(false);

    JsonVariant json_var = true;

    json_int new_int = 123;
    JsonVariant json_var_int (new_int);

    json_string new_string_hole = "HOLE";
    JsonVariant json_var_str_hole (new_string_hole);
    JsonVariant json_var_str_hola (json_string("HOLA"));
    // JsonVariant json_var_str_como ("COMO");

    std::cout << json_var_str_hole.get_string() << std::endl;
    std::cout << json_var_str_hola.get_string() << std::endl;
    // std::cout << json_var_str_como.get_string() << std::endl;

    JsonVariant json_var_array (json_type::array);
    json_var_array.push_array(json_var_str_hola);
    // json_var_array.variant_.push_back(json_var_str_hole);

    json_string key = "kk";
    JsonVariant value = json_string("vv");
    json_kv_variant kv (key, value);
    JsonVariant object (json_type::object);
    object.push_object(kv);

    std::cout << object.get_object()[0].first << std::endl;
    std::cout << object.get_object()[0].second.get_string() << std::endl;
    

    // TYPE CONVERSIONS
    std::cout << std::endl << "TYPE CONVERSIONS" << std::endl;
    JsonVariant int_1 = json_int(1111);
    JsonVariant int_2 = json_int(2222);
    std::cout << "int_2.get_int() = " << int_2.get_int()  << std::endl;
    int_2.set_bool(true);
    std::cout << "int_2.get_bool() = " << int_2.get_bool()  << std::endl;
    int_2.set_bool(false);
    std::cout << "int_2.get_bool() = " << int_2.get_bool()  << std::endl;
    int_2.set_float(2.222f);
    std::cout << "int_2.get_float() = " << int_2.get_float()  << std::endl;
    int_2.set_array();
    std::cout << "int_2.get_array().size() = " << int_2.get_array().size()  << std::endl;
    int_2.get_array().push_back(int_1);
    std::cout << "int_2.get_array().size() = " << int_2.get_array().size()  << std::endl;
    int_2.set_int(2222);
    std::cout << "int_2.get_int() = " << int_2.get_int()  << std::endl;


    // check for any significant memory leaks when switching between int and very large array
    // Conclusion: The proper destructor seems to be autmatically called!
    JsonVariant arr = json_array_variants();
    for(size_t i = 0; i < 10; i++){
        // turn into int
        arr.set_int(1);

        // Convert to array and populate with ints
        arr.set_array();
        for(size_t j = 0; j < 10000; j++)
            arr.get_array().push_back(JsonVariant(json_int(1)));
        
    }
    
    std::cout << std::endl;
    
    std::cout << "sizeof(JsonVariant) = " << sizeof(JsonVariant) << std::endl;
    std::cout << "sizeof(json_var_array) = " << sizeof(json_var_array) << std::endl;
    std::cout << "sizeof(json_var_array.variant_) = " << sizeof(json_var_array.variant_) << std::endl;
    


    // json_var.set_bool(true);

    // json_var.variant_ = false;

    // std::cout << std::get<json_null>(json_var.variant_) << std::endl;
    
    std::visit(
        [](auto&& arg){ 
            std::cout << "in visit" << std::endl;
        }, 
        json_var.variant_
    );

    std::visit(
        overloaded{ // Requires 
            [](auto arg) { std::cout << "unknown type" << ' '; },
            [](json_null arg) { std::cout << "null" << ' ';},
            [](json_bool arg)  { std::cout << "bool" << ' '; },
            [](json_int arg) { std::cout << "int" << ' '; },
            [](json_float arg) { std::cout << "float" << ' '; },
            [](const json_string& arg) { std::cout << "string" << ' '; },
            [](json_array_variants& arg) { std::cout << "array" << ' '; },
            [](json_object_variants& arg) { std::cout << "object" << ' '; }
        },
        json_var.variant_
    );

    std::cout << std::endl;
}
#pragma once


#include <type_traits>

#include "physon_types.hh"


struct JsonVar; // forward declare
enum class json_type {
    null,
    boolean,
    bool_true,
    bool_false,
    number_int,
    number_float,
    string,

    object,
    array,
};
bool is_literal(json_type type){
    return  type == json_type::null         ||
            type == json_type::boolean      ||
            type == json_type::bool_true    ||
            type == json_type::bool_false   ||
            type == json_type::number_int   ||
            type == json_type::number_float ||
            type == json_type::string;
}

typedef std::pair<json_string, JsonVar> json_kv_variant;
typedef std::vector<json_kv_variant> json_object_variants;
typedef std::vector<JsonVar> json_array_variants;
typedef std::variant<   json_string, 
                        json_bool,
                        json_null, 
                        json_float,
                        json_int,
                        json_array_variants,
                        json_object_variants
                    > json_variant;
/** Thick tag-union-like structure implemented using std::variant. */
struct JsonVar {
    json_type type = json_type::null;
    json_variant variant_;
    
    JsonVar()  {};
    JsonVar(json_null new_null) : type {json_type::null}, variant_ {new_null} {};
    JsonVar(json_bool new_bool) : variant_ {new_bool}  { 
        type = new_bool ? json_type::bool_true : json_type::bool_false;
    };
    JsonVar(json_int new_int) : type {json_type::number_int}, variant_ {new_int} {};
    JsonVar(json_float new_float) : type {json_type::number_float}, variant_ {new_float} {};
    JsonVar(json_string new_string) : type {json_type::string}, variant_ {new_string} {};
    // JsonVar(json_string& new_string) : type {json_type::string}, variant_ {new_string} {};
    // JsonVar(json_string&& new_string) : type {json_type::string}, variant_ {new_string} {};
    JsonVar(json_array_variants new_array) : type {json_type::array}, variant_ {new_array} {};
    JsonVar(json_object_variants new_object) : type {json_type::object}, variant_ {new_object} {};
    JsonVar(json_type new_type){
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


    json_kv_variant new_kv(json_string key, JsonVar value){
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



    void push_to_array(JsonVar& var_to_push){
        if(type == json_type::array){
            json_array_variants& array = get_array();
            array.push_back(var_to_push);
            return;
        }
        
        throw std::runtime_error("Error trying to push value to non-array."); 
    }
    void push_to_array(JsonVar&& var_to_push){
        if(type == json_type::array){
            json_array_variants& array = get_array();
            array.push_back(var_to_push);
            return;
        }
        
        throw std::runtime_error("Error trying to push value to non-array."); 
    }
    json_kv_variant& emplace_kv(json_string key, JsonVar value){

        if(type != json_type::object)
            throw std::runtime_error("Can't emplace KV when JsonVar is not an object.");

        json_object_variants& object = get_object();

        json_kv_variant& kv = object.emplace_back(  );
        kv.first = key;
        kv.second = value;
        
        return kv;
    }
    void push_to_object(const json_kv_variant& kv_to_push){
        if(type == json_type::object){
            json_object_variants& object = get_object();
            object.push_back(kv_to_push);
            return;
        }
        
        throw std::runtime_error("Error trying to push kv to non-object."); 
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

    // JsonVar json_var;
    // json_var.set_bool(false);

    JsonVar json_var = true;

    json_int new_int = 123;
    JsonVar json_var_int (new_int);

    json_string new_string_hole = "HOLE";
    JsonVar json_var_str_hole (new_string_hole);
    JsonVar json_var_str_hola (json_string("HOLA"));
    // JsonVar json_var_str_como ("COMO");

    std::cout << json_var_str_hole.get_string() << std::endl;
    std::cout << json_var_str_hola.get_string() << std::endl;
    // std::cout << json_var_str_como.get_string() << std::endl;

    JsonVar json_var_array (json_type::array);
    json_var_array.push_to_array(json_var_str_hola);
    // json_var_array.variant_.push_back(json_var_str_hole);

    json_string key = "kk";
    JsonVar value = json_string("vv");
    json_kv_variant kv (key, value);
    JsonVar object (json_type::object);
    object.push_to_object(kv);

    std::cout << object.get_object()[0].first << std::endl;
    std::cout << object.get_object()[0].second.get_string() << std::endl;
    

    // TYPE CONVERSIONS
    std::cout << std::endl << "TYPE CONVERSIONS" << std::endl;
    JsonVar int_1 = json_int(1111);
    JsonVar int_2 = json_int(2222);
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
    JsonVar arr = json_array_variants();
    for(size_t i = 0; i < 10; i++){
        // turn into int
        arr.set_int(1);

        // Convert to array and populate with ints
        arr.set_array();
        for(size_t j = 0; j < 10000; j++)
            arr.get_array().push_back(JsonVar(json_int(1)));
        
    }
    
    std::cout << std::endl;




    // variant_.emplace(variant(json_float(0.0)));
    // variant_.emplace(0);

    
    
    std::cout << "sizeof(JsonVar) = " << sizeof(JsonVar) << std::endl;
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


    // VARIANT METHODS EXPLORATION

    // json_int intint = 123;
    json_variant variant_int (json_int(123));
    json_variant variant_float (json_float(1.23));

    std::cout << "std::get<json_int>(variant_int) = " << std::get<json_int>(variant_int) << std::endl;
    variant_int.swap(variant_float);
    std::cout << "std::get<json_float>(variant_int) = " << std::get<json_float>(variant_int) << std::endl;
    // variant_int.emplace( json_variant(json_int(123)) );
    // variant_int.emplace( 123 );
    // variant_int.emplace( json_int(1) );
    // variant_int.emplace( intint );
    variant_int.emplace<4>(123);
    // variant_int = json_variant::emplace<json_int>;
    // variant_int.emplace( std::in_place_type<json_int> );
    // std::cout << "std::get<json_int>(variant_int) = " << std::get<json_int>(variant_int) << std::endl;

    variant_int = json_variant(json_int(123));
    std::cout << std::get<4>(variant_int) << std::endl;
    variant_int = 321;
    std::cout << std::get<json_int>(variant_int) << std::endl;
    std::cout << *std::get_if<json_int>(&variant_int) << std::endl;
    using T = std::decay_t< decltype(variant_int) >;
    std::cout << "std::is_same_v<T, json_variant> = " << std::is_same_v<T, json_variant> << std::endl;
    
    
    std::cout << "variant_int.index() = " << variant_int.index() << std::endl;
    std::cout << "variant_int.valueless_by_exception() = " << variant_int.valueless_by_exception() << std::endl;


    std::cout << std::endl;

    // NESTED JSON TESTS BY HAND
    {
        JsonVar root_arr  = json_array_variants();
        JsonVar intintint = json_int(123);
        JsonVar arrarrarr = json_array_variants();
        arrarrarr.push_to_array(intintint);
        JsonVar objobjobj = json_object_variants();
        json_kv_variant kvkvkv_1 = std::pair<json_string, JsonVar>("keykey1", json_string("Valval1"));
        json_kv_variant kvkvkv_2 ("keykey2", json_int(2));
        json_kv_variant kvkvkv_3 {"keykey3", json_int(3)};
        // json_kv_variant kvkvkv_4 = ("keykey2", json_int(4)); // es a no wok
        json_kv_variant kvkvkv_5 = {"keykey5", json_int(5)};
        objobjobj.push_to_object(kvkvkv_1);
        objobjobj.push_to_object(kvkvkv_2);
        objobjobj.push_to_object(kvkvkv_3);
        objobjobj.push_to_object(kvkvkv_5);
        objobjobj.push_to_object( {"keykey6", json_int(6)} );

        root_arr.push_to_array(intintint);              // lvalue
        root_arr.push_to_array(json_int(456)); // rvalue
        root_arr.push_to_array(arrarrarr); // rvalue
        root_arr.push_to_array(objobjobj);

        std::cout << sizeof(JsonVar) << std::endl;
        std::cout << sizeof(root_arr) << std::endl;
        std::cout << sizeof(root_arr.get_array()) << std::endl;
        std::cout << root_arr.get_array().size() << std::endl;
        std::cout << root_arr.get_array().capacity() << std::endl;
        

        // for(JsonVar var : root_arr.get_array()){
        //     std::cout << int(var.get_type()) << std::endl;
        // }

        
        
    
    }
    
}
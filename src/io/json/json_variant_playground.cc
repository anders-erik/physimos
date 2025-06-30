
#include <optional>
#include <type_traits>

#include "json_types.hh"







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

    j_int new_int = 123;
    JsonVar json_var_int (new_int);

    j_string new_string_hole = "HOLE";
    JsonVar json_var_str_hole (new_string_hole);
    JsonVar json_var_str_hola (j_string("HOLA"));
    // JsonVar json_var_str_como ("COMO");

    // std::cout << json_var_str_hole.get_string() << std::endl;
    // std::cout << json_var_str_hola.get_string() << std::endl;
    // std::cout << json_var_str_como.get_string() << std::endl;

    JsonVar json_var_array (json_type::array);
    json_var_array.get_array().push_back(json_var_str_hola);
    // json_var_array.variant_.push_back(json_var_str_hole);

    j_string key = "kk";
    JsonVar value = j_string("vv");
    j_kv kv (key, value);
    JsonVar object (json_type::object);
    object.object_push(kv);

    // std::cout << object.get_object().map[0].str << std::endl;
    // std::cout << object.get_object().map[0].var.get_string() << std::endl;
    

    // TYPE CONVERSIONS
    std::cout << std::endl << "TYPE CONVERSIONS" << std::endl;
    JsonVar int_1 = j_int(1111);
    JsonVar int_2 = j_int(2222);
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
    JsonVar arr = j_array();
    for(size_t i = 0; i < 10; i++){
        // turn into int
        arr.set_int(1);

        // Convert to array and populate with ints
        arr.set_array();
        for(size_t j = 0; j < 10000; j++)
            arr.get_array().push_back(JsonVar(j_int(1)));
        
    }
    
    std::cout << std::endl;




    // variant_.emplace(variant(j_float(0.0)));
    // variant_.emplace(0);

    
    
    std::cout << "sizeof(JsonVar) = " << sizeof(JsonVar) << std::endl;
    std::cout << "sizeof(json_var_array) = " << sizeof(json_var_array) << std::endl;
    std::cout << "sizeof(json_var_array.variant_) = " << sizeof(json_var_array.variant_) << std::endl;

    // json_var.set_bool(true);

    // json_var.variant_ = false;

    // std::cout << std::get<j_null>(json_var.variant_) << std::endl;
    
    std::visit(
        [](auto&& arg){ 
            std::cout << "in visit" << std::endl;
        }, 
        json_var.variant_
    );

    std::visit(
        overloaded{ // Requires 
            [](auto arg) { std::cout << "unknown type" << ' '; },
            [](j_null arg) { std::cout << "null" << ' ';},
            [](j_bool arg)  { std::cout << "bool" << ' '; },
            [](j_int arg) { std::cout << "int" << ' '; },
            [](j_float arg) { std::cout << "float" << ' '; },
            [](const j_string& arg) { std::cout << "string" << ' '; },
            [](j_array& arg) { std::cout << "array" << ' '; },
            [](j_object& arg) { std::cout << "object" << ' '; }
        },
        json_var.variant_
    );

    std::cout << std::endl;


    // VARIANT METHODS EXPLORATION

    // j_int intint = 123;
    json_variant variant_int (j_int(123));
    json_variant variant_float (j_float(1.23));

    std::cout << "std::get<j_int>(variant_int) = " << std::get<j_int>(variant_int) << std::endl;
    variant_int.swap(variant_float);
    std::cout << "std::get<j_float>(variant_int) = " << std::get<j_float>(variant_int) << std::endl;
    // variant_int.emplace( json_variant(j_int(123)) );
    // variant_int.emplace( 123 );
    // variant_int.emplace( j_int(1) );
    // variant_int.emplace( intint );
    variant_int.emplace<4>(123);
    // variant_int = json_variant::emplace<j_int>;
    // variant_int.emplace( std::in_place_type<j_int> );
    // std::cout << "std::get<j_int>(variant_int) = " << std::get<j_int>(variant_int) << std::endl;

    variant_int = json_variant(j_int(123));
    std::cout << std::get<4>(variant_int) << std::endl;
    variant_int = 321;
    std::cout << std::get<j_int>(variant_int) << std::endl;
    std::cout << *std::get_if<j_int>(&variant_int) << std::endl;
    using T = std::decay_t< decltype(variant_int) >;
    std::cout << "std::is_same_v<T, json_variant> = " << std::is_same_v<T, json_variant> << std::endl;
    
    
    std::cout << "variant_int.index() = " << variant_int.index() << std::endl;
    std::cout << "variant_int.valueless_by_exception() = " << variant_int.valueless_by_exception() << std::endl;


    std::cout << std::endl;

    // NESTED JSON TESTS BY HAND
    {
        JsonVar root_arr  = j_array();
        JsonVar intintint = j_int(123);
        JsonVar arrarrarr = j_array();
        arrarrarr.get_array().push_back(intintint);
        JsonVar objobjobj = j_object();
        j_kv kvkvkv_1 = {"keykey1", j_string("Valval1")};
        j_kv kvkvkv_2 ("keykey2", j_int(2));
        j_kv kvkvkv_3 {"keykey3", j_int(3)};
        // j_kv kvkvkv_4 = ("keykey2", j_int(4)); // es a no wok
        j_kv kvkvkv_5 = {"keykey5", j_int(5)};
        objobjobj.object_push(kvkvkv_1);
        objobjobj.object_push(kvkvkv_2);
        objobjobj.object_push(kvkvkv_3);
        objobjobj.object_push(kvkvkv_5);
        objobjobj.object_push( {"keykey6", j_int(6)} );

        root_arr.get_array().push_back(intintint);              // lvalue
        root_arr.get_array().push_back(j_int(456)); // rvalue
        root_arr.get_array().push_back(arrarrarr); // rvalue
        root_arr.get_array().push_back(objobjobj);

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
#include <string>
#include <iostream>

#include <type_traits>

#include "examples/config_shape.hh"

#include "physon.hh"
#include "physon_types.hh"
#include "physon_tests.hh"

std::string load_file(std::string path) {

    std::ifstream file(path); // Open the file
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << path << std::endl;
        throw std::runtime_error("Failed to open the file");
    }

    // Read file content into a string
    std::string file_contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();


    return file_contents;
}




// helper type for overloaded visitor lambdas 
template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide (not needed as of C++20)
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

void variant_playground(){
    std::cout << std::endl;

    // json_variant_wrap json_var;
    // json_var.set_bool(false);

    json_variant_wrap json_var = true;

    json_int new_int = 123;
    json_variant_wrap json_var_int (new_int);

    json_string new_string_hole = "HOLE";
    json_variant_wrap json_var_str_hole (new_string_hole);
    json_variant_wrap json_var_str_hola (json_string("HOLA"));
    // json_variant_wrap json_var_str_como ("COMO");

    std::cout << json_var_str_hole.get_string() << std::endl;
    std::cout << json_var_str_hola.get_string() << std::endl;
    // std::cout << json_var_str_como.get_string() << std::endl;

    json_variant_wrap json_var_array (json_type::array);
    json_var_array.push_array(json_var_str_hola);
    // json_var_array.variant_.push_back(json_var_str_hole);

    json_string key = "kk";
    json_variant_wrap value = json_string("vv");
    json_kv_variant kv (key, value);
    json_variant_wrap object (json_type::object);
    object.push_object(kv);

    std::cout << object.get_object()[0].first << std::endl;
    std::cout << object.get_object()[0].second.get_string() << std::endl;
    
    
    std::cout << "sizeof(json_variant_wrap) = " << sizeof(json_variant_wrap) << std::endl;
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


int main (int argc, char **argv) {

    std::cout << "Hello, Physon!" << std::endl;

    bool conformance_test_flag = false;

    if(conformance_test_flag){

        JsonTest::test_conformance();
        JsonTest::test_serialization();
        
        return 0;
    }


    std::string _json_string = load_file("data/name_literals_array.json");

    // Shapes
    // std::string _json_string = load_file("data/shapes.json");
    // Shape config;
    // ConfigShape shape_config {_json_string};
    // std::vector<Shape>& shapes = shape_config.load_shapes();
    // for(Shape shape : shapes){
    //     shape.print();
    // }


    Physon physon (_json_string);

    physon.parse();

    physon.print_original();
    // physon.print_tokens();
    // print_type_sizes();
    
    // physon.build_string(physon.root_wrapper);

    std::cout << std::endl << physon.stringify() << std::endl;
    

    variant_playground();

    return 0;
}
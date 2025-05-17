#include <string>
#include <iostream>

#include "examples/config_shape.hh"

#include "physon.hh"
#include "physon_types.hh"
#include "physon_tests.hh"

#include "json_store.hh"
#include "json_variant.hh"
#include "json_union.hh"



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




int main (int argc, char **argv) {

    std::cout << "Main JSON" << std::endl << std::endl;
    

    enum class json_flag {
        test,
        variant,
        config,
        misc,
    } flag = json_flag::test;



    if(flag == json_flag::test){

        JsonTest::test_conformance();
        JsonTest::test_serialization();
        
    }
    else if(flag == json_flag::variant){

        variant_playground();

    }
    else if(flag == json_flag::config){

        // Shapes
        std::string _json_string = load_file("data/shapes.json");
        Shape config;
        ConfigShape shape_config {_json_string};
        std::vector<Shape>& shapes = shape_config.load_shapes();
        for(Shape shape : shapes){
            shape.print();
        }

    }
    else if(flag == json_flag::misc){


        std::string _json_string = load_file("data/name_literals_array.json");

        Physon physon (_json_string);

        physon.parse();

        physon.print_original();
        // physon.print_tokens();
        // print_store_sizes();
        
        // physon.build_string(physon.root_wrapper);

        std::cout << std::endl << physon.stringify() << std::endl;

    }





    return 0;
}
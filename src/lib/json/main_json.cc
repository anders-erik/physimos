#include <string>
#include <iostream>

#include "examples/jphile_shape.hh"

#include "physon_types.hh"

#include "physon.hh"
#include "physon_tests.hh"
#include "json_store.hh"

#include "json.hh"
#include "json_test.hh"

// #include "json_union.hh"



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
        json,
        json_test,
        json_config,
        test,
        variant,
        config,
        beyond_ascii,
        misc,
    } flag = json_flag::json_test;


    if(flag == json_flag::json || flag == json_flag::json_test || flag == json_flag::json_config){

        if(flag == json_flag::json){

            std::string _json_string;

            // _json_string = load_file("data/literal_names_array.json");
            // _json_string = load_file("data/name_literals_nested_array.json");
            _json_string = load_file("data/string_array.json");
            // _json_string = load_file("data/numbers.json");
            // _json_string = load_file("data/object_nested.json");
            // _json_string = load_file("data/object.json");
            // _json_string = load_file("data/shapes.json");


            Json json (_json_string);
            json.lex();
            json.print_tokens();
            json.parse();
            std::string serialized_json = json.serialize();
            std::cout << serialized_json << std::endl;
            
        }
        else if(flag == json_flag::json_test){

            JsonTest::test_conformance();
            JsonTest::test_serialization();

        }
        else if(flag == json_flag::json_config){

            Phile file ("src/lib/json/data/shapes.json");
            JPhileShape shape_config (file);

        }

    }
    else if(flag == json_flag::test){

        PhysonTest::test_conformance();
        PhysonTest::test_serialization();
        
    }
    else if(flag == json_flag::variant){

        variant_playground();

        // ARRAY
        JsonVar root_array = json_array_variants();
        root_array.push_to_array(true);
        root_array.push_to_array(false);
        root_array.push_to_array(json_null(nullptr));

        JsonSerializer serializer;
        std::cout << serializer.serialize(root_array) << std::endl;

        // OBJECT
        JsonVar root_obj = json_object_variants();
        root_obj.emplace_kv("key1", json_int(123));
        root_obj.emplace_kv("key2", json_int(234));
        // nested object
        json_kv_variant& kv = root_obj.emplace_kv("key3", json_object_variants());
        JsonVar& kv_value = kv.second;
        kv_value.emplace_kv("k1", json_int(555));

        // PRINT OBJECTS
        std::string obj_str;

        serializer.set_config( { serial_ws::minimized, 0 } );
        obj_str = serializer.serialize(root_obj);
        std::cout << obj_str << std::endl;

        serializer.set_config( { serial_ws::oneline, 4 } );
        obj_str = serializer.serialize(root_obj);
        std::cout << obj_str << std::endl;

        serializer.set_config( { serial_ws::new_lines, 4 } );
        obj_str = serializer.serialize(root_obj);
        std::cout << obj_str << std::endl;
        

    }
    else if(flag == json_flag::config){

        // Shapes
        // std::string _json_string = load_file("data/shapes.json");
        // Shape config;
        // ConfigShape shape_config {_json_string};
        // std::vector<Shape>& shapes = shape_config.load_shapes();
        // for(Shape shape : shapes){
        //     shape.print();
        // }

    }
    if(flag == json_flag::beyond_ascii){


        // CHINESE MOON START
        std::cout << "Chinese : moon" << std::endl;

        std::string chinese_moon = load_file("data/non_ascii_strings/chinese_moon.json");
        std::cout << chinese_moon << std::endl;

        std::cout << "chinese_moon.length() = " << chinese_moon.length() << std::endl;
        std::cout << "chinese_moon.size()   = " << chinese_moon.size()   << std::endl;
        

        // This loops through each char
        // BUT if there is no space, then the output is proper UTF-8 in vscode terminal

        std::cout << "Individual basic_string bytes = ";
        for(char ch : chinese_moon)
            std::cout << "'" << ch <<"'" << ", ";
        std::cout << std::endl;
        std::cout << "basic_string bytes joined     = ";
        for(char ch : chinese_moon)
            std::cout << ch;
        std::cout << std::endl;

        std::cout << std::endl;
        // CHINESE MOON END

        
        // EXTENDED ASCII - Not UTF8 compatible
        unsigned char bits = 0b11111111;
        std::cout << int(bits) << std::endl;

        unsigned char bits_ä = 0b11100100;
        unsigned char ä = 0xe4; // ä
        std::cout << int(bits_ä) << std::endl;
        std::cout << int(ä) << std::endl;
        std::cout << ä << std::endl;
        // EXTENDED ASCII
        
    }
    else if(flag == json_flag::misc){


        std::string _json_string = load_file("data/literal_names_array.json");

        Physon physon (_json_string);

        physon.parse();

        physon.print_original();
        // physon.print_tokens();
        // print_store_sizes();
        
        // physon.build_string(physon.root_wrapper);

        std::cout << std::endl << physon.stringify() << std::endl;

        JsonSerializer serializer;
        std::string serialized_str = serializer.serialize(physon.root_wrapper, physon.store);
        std::cout << std::endl << serialized_str << std::endl << std::endl;
        
    }





    return 0;
}
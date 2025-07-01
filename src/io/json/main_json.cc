#include <string>
#include <iostream>

// #include "examples/jfile_shape.hh"

#include "json_types.hh"

#include "json_lexer.hh"
#include "json_parser.hh"
#include "json_serialize.hh"


#include "json.hh"

#include "test/io/json/tlib_json.hh"

void variant_playground();

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

using RunFlag = uint;
const RunFlag RF_NEW           = 0x0001;
const RunFlag RF_TCLIB         = 0x0002 + RF_NEW;
const RunFlag RF_TEST_OLD      = 0x0004 + RF_NEW;
const RunFlag RF_JSHAPE        = 0x0008 + RF_NEW;
const RunFlag RF_TEST_OLDOLD   = 0x0010;
const RunFlag RF_VARIANT       = 0x0020;
const RunFlag RF_CONFIG        = 0x0040;
const RunFlag RF_BEYOND_ASCII  = 0x0080;
const RunFlag RF_MISC          = 0x0100;



std::string json_data_path    = "src/test/io/json/data/";


int main (int argc, char **argv) {

    std::cout << "Main JSON" << std::endl << std::endl;
    

    RunFlag run_flag = RF_NEW;
    // RunFlag run_flag = RF_TCLIB;
    // RunFlag run_flag = RF_VARIANT;



    if(run_flag & RF_NEW){

        if(run_flag == RF_NEW){

            Str jsource;
            // _json_string = load_file("data/literal_names_array.json");
            // _json_string = load_file("data/name_literals_nested_array.json");
            // _json_string = load_file(json_data_path + "strings/string_array.json");
            // _json_string = load_file(json_data_path + "non_valid_parser/non_ascii_string.json");
            // _json_string = load_file(json_data_path + "non_valid_lexer/unclosed_string.json");
            // jsource = File::cat_as_str_core_xplat(data_dir + "misc/mesh_minimal.json").consume_value();
            // _json_string = load_file("data/numbers.json");
            // _json_string = load_file("data/object_nested.json");
            // _json_string = load_file("data/object.json");
            // _json_string = load_file("data/shapes.json");

            // jsource = File::cat_as_str_core_xplat(data_dir + "primitives/string.json").consume_value();
            // jsource = File::cat_as_str_core_xplat(data_dir + "primitives/object.json").consume_value();
            // jsource = File::cat_as_str_core_xplat(data_dir + "integers/integers.json").consume_value();
            jsource = File::cat_as_str_core_xplat(data_dir + "misc/shapes.json").consume_value();
            // jsource = File::cat_as_str_core_xplat(data_dir + "misc/name_sort_bug.json").consume_value();

            // JsonVar json_v;
            // json_v.parse(jsource.to_c_str());
            // ResMove<Str> str = json_v.serialize();
            // Print::line(str.consume_value());

            // jsource = "asdlkasj";

            Json j_var;
            j_var.try_parse(jsource);

            ResMove<Str>  s1_res  = Json::serialize(j_var);
            Str  s1 = s1_res.consume_value();
            Print::line("serialize1: \n", s1);

            // j_var.try_parse(s1);
            // ResMove<Str>  serialize2  = Json::serialize(j_var);

            // ResMove<JsonVar>    p1  = Json::parse(jsource);
            // ResMove<Str>        s1  = Json::serialize(p1.consume_value());
            // // ResMove<JsonVar>    p2  = Json::parse(s1);
            // // Str                 s2  = Json::serialize(p2.consume_value());

            // if(s1 != serialize2.consume_value())
            // {
            //     Print::line("serialize2: \n", serialize2.consume_value());
            // }



            // ResMove<JsonVar> json_res = Json::parse(jsource);
            // if(json_res.has_error())
            //     Print::err(json_res.consume_error());

            // JsonVar json_var = json_res.consume_value();

            // Str serialized = Json::serialize(json_var);
            // Print::line(serialized);
            

            // Json json (_json_string);
            // json.lex();
            // json.print_tokens();
            // json.parse();
            // std::string serialized_json = json.serialize();
            // std::cout << serialized_json << std::endl;
            
        }
        else if(run_flag == RF_TEST_OLD){

            // JsonTest::test_conformance();
            // JsonTest::test_serialization();

        }
        else if(run_flag == RF_JSHAPE){

            // File file ("src/lib/json/data/shapes.json");
            // JFileShape shape_config (file);

            // JFileShape shape_config (json_data_path + "src/lib/json/data/shapes.json");

            // Shape line = { {0.03, 0.2}, {11.01, 12.343444}};
            // std::string shape_string = JFileShape::serialize_shape(line);
            // std::cout << shape_string << std::endl;
            

        }
        else if(run_flag == RF_TCLIB)
        {
            tclib_json.run();
            tclib_json.print_result();
            Print::line("");
        }

    }
    else if(run_flag == RF_TEST_OLDOLD){

        // PhysonTest::test_conformance();
        // PhysonTest::test_serialization();
        
    }
    else if(run_flag == RF_VARIANT){

        Json var;
        var.var = "hello";
        // std::get<j_string>{var.variant_}
        // std::cout << std::get<j_string>(var.variant_) << '\n';
        // std::cout << std::get<j_float>(var.variant_) << '\n';

        variant_playground();

        // ARRAY
        Json root_array = j_array();
        root_array.get_array().push_back(true);
        root_array.get_array().push_back(false);
        root_array.get_array().push_back(j_null(nullptr));

        JsonSerializer serializer;
        // std::cout << serializer.serialize(root_array) << std::endl;

        // OBJECT
        Json root_obj = j_object();
        root_obj.get_object().emplace("key1", j_int(123));
        root_obj.get_object().emplace("key2", j_int(234));
        // nested object
        // auto& kv = root_obj.object_push({"key3", j_object()});
        // JsonVar& kv_value = kv.second;
        // kv_value.get_object().emplace("k1", j_int(555));

        // PRINT OBJECTS
        // std::string obj_str;

        // serializer.set_config( { serial_ws::minimized, 0 } );
        // obj_str = serializer.serialize(root_obj);
        // std::cout << obj_str << std::endl;

        // serializer.set_config( { serial_ws::oneline, 4 } );
        // obj_str = serializer.serialize(root_obj);
        // std::cout << obj_str << std::endl;

        // serializer.set_config( { serial_ws::new_lines, 4 } );
        // obj_str = serializer.serialize(root_obj);
        // std::cout << obj_str << std::endl;
        

    }
    else if(run_flag == RF_CONFIG){

        // Shapes
        // std::string _json_string = load_file("data/shapes.json");
        // Shape config;
        // ConfigShape shape_config {_json_string};
        // std::vector<Shape>& shapes = shape_config.load_shapes();
        // for(Shape shape : shapes){
        //     shape.print();
        // }

    }
    if(run_flag == RF_BEYOND_ASCII){


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
    else if(run_flag == RF_MISC){


        std::string _json_string = load_file("data/literal_names_array.json");

        // Physon physon (_json_string);

        // physon.parse();

        // physon.print_original();
        // // physon.print_tokens();
        // // print_store_sizes();
        
        // // physon.build_string(physon.root_wrapper);

        // std::cout << std::endl << physon.stringify() << std::endl;

        // JsonSerializer serializer;
        // std::string serialized_str = serializer.serialize(physon.root_wrapper, physon.store);
        // std::cout << std::endl << serialized_str << std::endl << std::endl;
        
    }





    return 0;
}
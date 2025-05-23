#include <string>
#include <iostream>

#include "lib/dir.hh"

#include "physon_types.hh"
#include "json.hh"
#include "json_serialize.hh"



struct JsonTest {

    static void test_conformance(); /** Throws error on invalid json. */
    static void test_serialization(); /** 2 parse-serialize cycles without change. Only applies to valid json. */

private:

    /** Parse-serialize-parse-serialize. Tests parser-serializer chain on already confirmed valid json strings.  */
    static void psps(std::string file_path_str); 

    static void print_test_verbose(bool success, std::string test_name, std::string file_name, std::string msg);
    static void conforms_not(std::string& file_path_str);

    /** Throws if file  */
    static void conforms(std::string file_path_str);
    // static void conforms_not(std::string file_path_str);

    static std::string load_file(std::string path);
};


void JsonTest::test_conformance(){

    std::cout << "\n        CONFORMANCE TESTS : VAILD JSON" << std::endl;
    conforms("data/null.json");
    conforms("data/true.json");
    conforms("data/false.json");

    conforms("data/literal_names_array.json");
    conforms("data/name_literals_nested_array.json");
    conforms("data/string_array.json");
    conforms("data/object.json");
    conforms("data/object_nested.json");

    conforms("data/integer.json");
    conforms("data/integers.json");
    conforms("data/numbers.json");
    conforms("data/numbers_2.json");

    conforms("data/penpaper.json");
    conforms("data/widget.json");

    conforms("data/shapes.json");

    std::cout << std::endl;

    std::cout << "\n        CONFORMANCE TESTS : INVAILD JSON" << std::endl;

    Dir dir {};
    std::vector<std::string> paths;



    // LEXING
    std::cout << "Lexer" << std::endl;

    dir.new_path("data/non_valid_lexer");
    paths = dir.get_file_paths_with_extension("json");
    for (std::string& path : paths)
        conforms_not(path);
    


    // PARSING
    std::cout << "\nParser" << std::endl;

    dir.new_path("data/non_valid_parser");
    paths = dir.get_file_paths_with_extension("json");
    for (std::string& path : paths)
        conforms_not(path);
    

    std::cout << std::endl;
};


void JsonTest::print_test_verbose(
    bool success,
    std::string test_name,
    std::string file_name,
    std::string msg
){
    std::string status;

    if(success)
        status = "PASS";
    else
        status = "FAIL";
    
    std::string log_string =            status + \
                                "\n     Test: " + test_name + \
                                "\n     File: " + file_name + \
                                "\n     Msg:  " + msg;

    std::cout << log_string << std::endl;
    
}

/** Make sure it throws */
void JsonTest::conforms_not(std::string& file_path_str){

    // Try read file
    Phile phile {file_path_str};
    std::string json_source = phile.copy_as_string_cwd();
    if(!phile.successful_read){
        print_test_verbose(
            "XX", 
            "Invalid Json", 
            file_path_str, 
            "Failed to read file"
        );
        return;
    }


    try
    {
        Json json (json_source);
        json.lex_parse();

        print_test_verbose("XX", "Invalid Json", file_path_str, "Successfully parsed invalid json.");
        
    }
    catch(const std::exception& e)
    {   
        // TODO :Add json error message
        print_test_verbose("OO", "Invalid Json", file_path_str, "< json error message goes here >");
    }
}



void JsonTest::test_serialization(){

    std::cout << std::endl << "    SERIALIZATION TESTS" << std::endl;

    bool single_file = false;

    if(single_file){
        psps("data/string_array.json");
    }
    else {
        psps("data/null.json");
        psps("data/true.json");
        psps("data/false.json");

        psps("data/literal_names_array.json");
        psps("data/name_literals_nested_array.json");
        psps("data/string_array.json");
        psps("data/object.json");
        psps("data/object_nested.json");

        psps("data/integer.json");
        psps("data/integers.json");
        psps("data/numbers.json");
        psps("data/numbers_2.json");

        psps("data/penpaper.json");
        psps("data/widget.json");

        psps("data/shapes.json");
    }

    std::cout << std::endl;
}


void JsonTest::psps(std::string file_path_str){

    enum class PSPS_STATE {
        PARSE_1  = 0,
        SERIAL_1 = 1,
        PARSE_2  = 2,
        SERIAL_2 = 3,
        EQUALITY = 4,
    } state = PSPS_STATE::PARSE_1;

    // If reading file fails, the actual test is not performed
    std::string json_data;
    try
    {
        json_data = load_file(file_path_str);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
        return;
    }

    try
    {
        Json json (json_data);
        JsonSerializer serializer;

        // PS 1
        state = PSPS_STATE::PARSE_1;
        JsonVar parse_1 = json.lex_parse();

        state = PSPS_STATE::SERIAL_1;
        std::string serial_1 = serializer.serialize(parse_1);


        // PS 2
        json.set_json_source(serial_1);

        state = PSPS_STATE::PARSE_2;
        JsonVar parse_2 = json.lex_parse();

        state = PSPS_STATE::SERIAL_2;
        std::string serial_2 = serializer.serialize(parse_2);


        // EQUALITY
        state = PSPS_STATE::EQUALITY;
        if(serial_1 != serial_2)
            throw std::runtime_error("Type: Serial_1 != Serial_2.");

        std::cout << "Conformance test OK : " << file_path_str << std::endl;

    }
    catch(const std::exception& e)
    {
        std::cout << "Exception thrown during serialization testing.\n";
        std::cout << "    State : " << int(state) << std::endl;
        std::cout << "    file : " << file_path_str << std::endl;

        std::cout << "    " << e.what() << '\n';
    }

}


/** Make sure it does not throw */
void JsonTest::conforms(std::string file_path_str){

    // If reading file fails, the actual test is not performed
    std::string json_data;
    try
    {
        json_data = load_file(file_path_str);
    }
    catch(const std::exception& e)
    {
        std::cout << e.what() << '\n';
        return;
    }

    try
    {
        Json json (json_data);
        // Physon physon (json_data);
        // physon.parse();
        json.lex_parse();
        std::cout << "  Conformance test OK : " << file_path_str << std::endl;
        
    }
    catch(const std::exception& e)
    {
        std::cout << "  Conformance test FAILED : " << file_path_str << std::endl;

        std::cout << e.what() << '\n';
    }
    
}

// /** Make sure it throws */
// void JsonTest::conforms_not(std::string file_path_str){


//     // If reading file fails, the actual test is not performed
//     std::string json_data;
//     try
//     {
//         json_data = load_file(file_path_str);
//     }
//     catch(const std::exception& e)
//     {
//         std::cout << e.what() << '\n';
//     }

//     try
//     {
//         Json json (json_data);
//         json.lex_parse();
//         std::cout << "  Conformance test Failed : " << file_path_str << std::endl;
        
//     }
//     catch(const std::exception& e)
//     {
//         std::cout << "  Conformance test OK : " << file_path_str << std::endl;
//     }
// }



std::string JsonTest::load_file(std::string path) {

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
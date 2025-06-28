#include <string>
#include <iostream>

#include "physon.hh"
#include "physon_types.hh"
#include "json_serialize.hh"



struct PhysonTest {

    static void test_conformance(); /** Throws error on invalid json. */
    static void test_serialization(); /** 2 parse-serialize cycles without change. Only applies to valid json. */

private:

    /** Parse-serialize-parse-serialize. Tests parser-serializer chain on already confirmed valid json strings.  */
    static void psps(std::string file_path_str); 

    /** Throws if file  */
    static void conforms(std::string file_path_str);
    static void conforms_not(std::string file_path_str);

    static std::string load_file(std::string path);
};



void PhysonTest::test_conformance(){

    std::cout << std::endl << "    CONFORMANCE TESTS : VAILD JSON" << std::endl;
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


    std::cout << std::endl << "    CONFORMANCE TESTS : INVAILD JSON" << std::endl;
    conforms_not("data/non_valid_json/empty.json");
    conforms_not("data/non_valid_json/ws.json");
    conforms_not("data/non_valid_json/unclosed_string.json");
    conforms_not("data/non_valid_json/numbers_invalid.json");

};



void PhysonTest::test_serialization(){

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
}


void PhysonTest::psps(std::string file_path_str){

    enum class PSPS_STATE {
        PARSE_1  = 0,
        SERIAL_1 = 1,
        PARSE_2  = 2,
        SERIAL_2 = 3,
    } state = PSPS_STATE::PARSE_1;

    try
    {
        std::string json_data = load_file(file_path_str);
        Physon physon (json_data);
        JsonSerializer serializer;

        physon.parse();

        state = PSPS_STATE::SERIAL_1;

        // std::string serialized_1 = physon.stringify();
        std::string serialized_1 = serializer.serialize(physon.root_wrapper, physon.store);

        physon.content = serialized_1; // TODO : turn this into a method call, reseting the internal json structure
        
        // std::cout << json_data << std::endl;
        // std::cout << serialized_1 << std::endl;


        state = PSPS_STATE::PARSE_2;

        physon.parse();

        state = PSPS_STATE::SERIAL_2;

        // std::string serialized_2 = physon.stringify();
        std::string serialized_2 = serializer.serialize(physon.root_wrapper, physon.store);

        // std::cout << serialized_1 << std::endl;
        // std::cout << serialized_2 << std::endl;
        
        if(serialized_1 == serialized_2)
            std::cout << "Serilization test OK : " << file_path_str << std::endl;
        else
            throw std::runtime_error("Serialization test Error. \nSerialized 1 = <" + serialized_1 + "> \nSerialized 2 = <" + serialized_2 + ">.");
        
    }
    catch(const std::exception& e)
    {
        std::cout << "Exception thrown during serialization testing.";
        std::cout << "State : " << int(state) << std::endl;
        std::cout << "file : " << file_path_str << std::endl;

        std::cout << e.what() << '\n';
    }
}


/** Make sure it does not throw */
void PhysonTest::conforms(std::string file_path_str){

    std::string json_data;

    // If reading file fails, the actual test is not performed
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
        Physon physon (json_data);
        physon.parse();
        std::cout << "Conformance test OK : " << file_path_str << std::endl;
        
    }
    catch(const std::exception& e)
    {
        std::cout << "Conformance test FAILED : " << file_path_str << std::endl;

        std::cout << e.what() << '\n';
    }
    
}

/** Make sure it throws */
void PhysonTest::conforms_not(std::string file_path_str){
    try
    {
        std::string json_data = load_file(file_path_str);
        Physon physon (json_data);
        physon.parse();
        std::cout << "Conformance test Failed : " << file_path_str << std::endl;
        
    }
    catch(const std::exception& e)
    {
        std::cout << "Conformance test OK : " << file_path_str << std::endl;

        // std::cout << e.what() << '\n';
    }
}



std::string PhysonTest::load_file(std::string path) {

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
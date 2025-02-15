#ifndef PLIB_TOML_HH
#define PLIB_TOML_HH

#include <string>
#include <vector>

namespace plib {

    struct TOML_Value;
    struct TOML_KV;
    struct TOML_Table;


    typedef pstring                     TOML_String;
    typedef std::vector<TOML_Value>     TOML_Array;


    typedef enum TOML_ValueType {
        EMPTY,
        TABLE,
        STRING,
        ARRAY,
    } TOML_ValueType;


    typedef struct TOML_Value {
        TOML_ValueType type = TOML_ValueType::EMPTY;

        TOML_String string;
        TOML_Array  array;

        void set_value(TOML_String string);
        void set_value(TOML_Array array);
        

        TOML_Value() {};
    } TOML_Value;


    typedef struct TOML_KV {
        TOML_String key;
        TOML_Value  value;
    } TOML_KV;


    typedef struct TOML_Table {
        std::string name;
        std::vector<TOML_KV> kvs;
    } TOML_Table;


    // A parsed toml file
    // Format support notes:
    // Types: STRING, ARRAY
    // Other: Only single line KV pairs
    typedef class TOML {
        public:
            TOML_Table*                 root_table = nullptr;
            std::vector<TOML_Table*>    tables;


            // Load full TOML file from unverified string path
            void            load(std::string file_spath);

            // returns a table object with name matching argument.
            TOML_Table*     find_table(TOML_String table_name);

            TOML_Table*     parse_table(std::vector<std::string> lines);
            TOML_KV         parse_line(std::string line);

            TOML_Array      parse_single_line_array(pstring array_str);

            TOML_Value      parse_toml_value(std::string value_str);

            TOML() {};
    } TOML;


}



#endif


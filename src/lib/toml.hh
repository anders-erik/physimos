#ifndef PLIB_TOML_HH
#define PLIB_TOML_HH

#include <string>
#include <vector>

namespace plib {

    struct TOML_Table;
    struct TOML_KV;
    class TOML_Value;

    typedef pstring                     TOML_String;
    typedef std::vector<TOML_Value>     TOML_Array;

    /**
     * Supported TOML Types:
     *  - STRING
     *  - ARRAY
     */
    typedef enum TOML_ValueType {
        STRING,
        ARRAY,
    } TOML_ValueType;


    /**
     *   A parsed toml file
     *   Notes:
     *   Supported Types: STRING, ARRAY
     *   Other: Only single line KV pairs
     */
    typedef class TOML {
    public:
        /**
         *   A list of all tables in a loaded toml file.
         *   All KV pairs before first table label is referred to as "ROOT_TABLE" and is always located at 'tables[0]'
         */
        std::vector<TOML_Table*>    tables;

        /**
         *  Load full TOML file from unverified string path
         */
        void            load(std::string file_spath);

        /**
         * Returns a table object with name matching argument.
         * table_name = "ROOT_TABLE" || "" will return the root table.
         * @return Match or nullptr
         */
        TOML_Table* find_table(TOML_String table_name);

        /**
         *  Simple check during line parsing to see if a new table line is present.
         */
        bool            is_table_label_line(std::string line);

        TOML() {};
    } TOML;


    /**
     * Toml table identified by [table_name] in document and 'contains' a list of KV pairs.
     */
    typedef struct TOML_Table {
        std::string name;
        std::vector<TOML_KV> kvs;

        // Parse
        bool     parse_line_and_push_kv(std::string line);

        TOML_Table(std::vector<std::string> lines);
    } TOML_Table;


    /**
     *  A Generic toml data object that will hold any of the supported TOML types as specified in the 'TOML_ValueType' enum.
     */
    typedef class TOML_Value {
        public:
            TOML_ValueType type;

            TOML_String string;
            TOML_Array  array;

        public:
            /** Might remove in favor of private & immutable primitive values */
            void set_value(TOML_String string);
            /** Might remove in favor of private & immutable primitive values */
            void set_value(TOML_Array array);

            /** Only single line arrays currently supported. */
            TOML_Array      parse_array_single_line(pstring array_str);

            // Trimmed string to be parsed as a toml value
            TOML_Value(pstring value_string);
            TOML_Value() {};
    } TOML_Value;


    /**
     * A pair of toml values. The key is always a string, and the value can hold any ofe the supported toml value types.
     */
    typedef struct TOML_KV {
        TOML_String key;
        TOML_Value  value;

        TOML_KV(TOML_String _key, TOML_Value _value);
        TOML_KV() {};
    } TOML_KV;


}



#endif


#ifndef PLIB_TOML_HH
#define PLIB_TOML_HH

#include <string>
#include <vector>

namespace plib {

    /**
     * Specs: https://toml.io/en/v1.0.0
     * 
     * TODO:
     *  - escape characters in strings
     *  - int and float data types
     *  - boolean
     *  - multiline array
     *  - comments
     *  - better error handling when searching parsed structure
     * 
    ***/


    struct TOML_Table;
    struct TOML_KV;
    struct TOML_Array;
    class TOML_Value;

    typedef std::string     TOML_String;
    typedef long            TOML_Int;
    typedef double          TOML_Float;

    /**
     * Supported TOML Types:
     *  - STRING
     *  - ARRAY
     */
    typedef enum TOML_ValueType {
        EMPTY,
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

    
        // New methods to implement
    public:
        
    private:
        void parse(std::string toml_string);
        void get_line_type(std::string toml_line);

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
        TOML_Table* find_table(std::string table_name);

        /**
         *  Simple check during line parsing to see if a new table line is present.
         */
        bool            is_table_label_line(std::string line);

        /**
         * Search loaded tables and returns table with matching toml table.
         * If no match is found the returned table is the root table wchich always exists.
         */
        TOML_Table& operator[](std::string table_name);

        TOML() {};
    } TOML;


    /**
     * Toml table identified by [table_name] in document and 'contains' a list of KV pairs.
     */
    typedef struct TOML_Table {
        TOML_String name;
        std::vector<TOML_KV> kvs;

        /**
         * Returns true if successful parse. False on errors.
         * Currently 3 types of lines : empty, table label, or KVs which are identified using a '='
         */
        bool        parse_line_and_push_kv(TOML_String line);

        /**
         * Search table for KV object with key matching argument.
         */
        TOML_KV     operator[](TOML_String key_name);

        TOML_Table(std::vector<TOML_String> lines);
    } TOML_Table;


    typedef struct TOML_Array {
        std::vector<TOML_Value> vector;

        /**
         * Search table for TOML_Values equal the parsed argument string.  
         */
        TOML_Value      operator[](TOML_String value_string);
        /** Compares the type and value of each containing TOML_Value. WARN: probably unsafe iterating. */
        bool      operator==(TOML_Array& rhs);

        /**
         * Get underlying std::vector of TOML_Values.
         */
        std::vector<TOML_Value>& get_vector();
        
        TOML_Array(TOML_String array_str);
        TOML_Array() : vector({}) {};
    } TOML_Array;

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

            /** Compares the type and primitive value. */
            bool      operator==(TOML_Value& rhs);

            // Trimmed string to be parsed as a toml value
            TOML_Value(TOML_String value_string);
            TOML_Value() : type(TOML_ValueType::EMPTY) {};
    } TOML_Value;


    /**
     * A pair of toml values. The key is always a string, and the value can hold any ofe the supported toml value types.
     */
    typedef struct TOML_KV {
        TOML_String key;
        TOML_Value  value;

        TOML_KV(TOML_String _key, TOML_Value _value);
        TOML_KV() : key("_"), value() {};
    } TOML_KV;


}



#endif


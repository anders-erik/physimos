#ifndef PLIB_TOML_HH
#define PLIB_TOML_HH

#include <string>
#include <vector>

namespace plib {
    typedef enum TOML_Type {
        EMPTY,
        TABLE,
        STRING,
        ARRAY,
    } TOML_Type;

    typedef struct TOML_Property {
        TOML_Type type;
        std::string name;
        bool is_table;

        std::string string;
        std::vector<TOML_Property> array;
    } TOML_Property;

    // A parsed toml file
    // Format support notes:
    // Types: STRING, ARRAY
    // Other: Only single line properties
    typedef class TOML {
        public:
            std::vector<TOML_Property> root_properties;

            // Load full TOML file from unverified string path
            void load(std::string file_spath);
            // return a root property from loaded file.
            // Is an ARRAY if found, EMPTY if not found.
            TOML_Property find_root_property(std::string property_key);

            // Internal

            // Returns the
            TOML_Property parse_line(std::string line);
            TOML_Property parse_array(std::string array_str);

            TOML() {};
    } TOML;

    std::string fs_cat(std::string path_std_string);
    std::vector<unsigned char>& fs_cat_bin(std::string path_std_string);

}



#endif


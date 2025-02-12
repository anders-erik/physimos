

#include "string.hh"

namespace plib {

    bool cstr_is_empty_or_null(char* cstr){

        // null
        if (cstr == NULL)
            return true;

        // empty
        if (cstr[0] == '\0')
            return true;

        return false;
    }

}


namespace plib::std_string {

    // returns a vector of the resulting string sections.
    // e.g. "hello there" -> {"hello", "there"} with delimiter " ".
    std::vector<std::string> split(std::string str, char delimiter) {

        std::stringstream str_stream(str);
        std::string section;

        std::vector<std::string> returnVector;

        while (std::getline(str_stream, section, delimiter))
            returnVector.push_back(section);

        return returnVector;

    }

}

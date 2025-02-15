#include <string>
#include <iostream>

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

    // Removes whitspace in place using std::isspace().
    void trim(std::string& str) {
        // std::cout << str << ", size=[" << str.size() << ']' << std::endl;

        size_t original_size = str.size();

        if (original_size == 0)
            return;
        
        
        size_t start_whitespace_count = 0;
        size_t removed_back_count = 0;

        // pop unitl last non white-space char found
        for (int i = original_size-1; i > 0; --i) {

            if (!std::isspace(str[i]))
                break;
            str.pop_back();
            removed_back_count++;
        }


        // find first non white-space
        for (; start_whitespace_count < str.size(); ++start_whitespace_count) {
            if (!std::isspace(str[start_whitespace_count]))
                break;
        }

        // no whitespace at beginning of string
        if(start_whitespace_count == 0)
            return;


        // shift string left equal to front whitespace width
        for (size_t i = 0; i < str.size() - start_whitespace_count; ++i) {
            str[i] = str[i + start_whitespace_count];
        }
        // pop the above shifted width
        for (; start_whitespace_count > 0; --start_whitespace_count){
            str.pop_back();
        }
        
        // std::cout << str << ", size=[" << str.size() << ']' << std::endl;
    }

    // returns a vector of the resulting string sections.
    // e.g. "hello there" -> {"hello", "there"} with delimiter " ".
    // Returns empty vector if string.size() == 0.
    // Returns vector of size 1 containing original string if delimiter not found.
    std::vector<std::string> split(std::string str, char delimiter) {

        std::stringstream str_stream(str);
        std::string section;

        std::vector<std::string> returnVector {};

        if(str.size() == 0)
            return returnVector;

        while (std::getline(str_stream, section, delimiter))
            returnVector.push_back(section);

        return returnVector;

    }

}

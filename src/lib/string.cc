

#include "string.hh"

namespace plib {

    std::vector<std::string> split_str(std::string str, char delimiter){

        std::stringstream str_stream(str);
        std::string section;

        std::vector<std::string> returnVector;

        while (std::getline(str_stream, section, delimiter))
            returnVector.push_back(section);

        return returnVector;
        
    }


}

